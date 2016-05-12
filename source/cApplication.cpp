//cApplication.cpp

#include <memory>
#include "cApplication.hpp"

#include "matrix.hpp"
#include "conversions.hpp"

cApplication::cApplication():
m_window(sf::VideoMode(640, 480), "Hexapod control", sf::Style::Close),
m_mainMenu(cFrame::getInstance(m_window.getSize())),
m_serial(cPololuSerial::getInstance()),
m_commandQueue(cCommandQueue::getInstance(m_serial)),
m_spider(cSpiderRobotState::getInstance())
{
    initGui();

    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        if (leg == LEG_1)
            m_spider.solveInverseKinematics(defaultPositionVector1, leg);
        else if (leg == LEG_2)
            m_spider.solveInverseKinematics(defaultPositionVector2, leg);
        else if (leg == LEG_3)
            m_spider.solveInverseKinematics(defaultPositionVector3, leg);
        else if (leg == LEG_4)
            m_spider.solveInverseKinematics(defaultPositionVector4, leg);
        else if (leg == LEG_5)
            m_spider.solveInverseKinematics(defaultPositionVector5, leg);
        else if (leg == LEG_6)
            m_spider.solveInverseKinematics(defaultPositionVector6, leg);
        addOneLegCommand_setTarget(channel, leg);
        ++leg;
    }
}

void cApplication::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(m_window.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update();
        }
        render();
    }
}

void cApplication::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed: m_window.close();
                                    break;
            default: break;
        }
        m_mainMenu.processEvent(event);
    }
}
void cApplication::update()
{
    if(not m_commandQueue.IsEmpty() && not m_serial.isMoving())
        if(m_status)
            m_commandQueue.runCommand();
}

void cApplication::render()
{
    m_window.draw(m_mainMenu);
    m_window.display();
}

void cApplication::initGui()
{
    std::unique_ptr<cContainer> controlContainer(new cContainer());
    controlContainer->setPosition(controlContainerPosition);
    controlContainer->setSize(controlContainerSize);

    std::unique_ptr<cContainer> viewContainer(new cContainer());
    viewContainer->setPosition(viewContainerPosition);
    viewContainer->setSize(viewContainerSize);

    std::unique_ptr<cTextButton> ON(new cTextButton("ON"));
    ON->setFillColor(greenColor);
    ON->setPosition(ONPosition);

    std::unique_ptr<cTextButton> OFF(new cTextButton("OFF"));
    OFF->setFillColor(redColor);
    OFF->setDisabled();
    OFF->setPosition(OFFPosition);

    std::unique_ptr<cTextButton> START(new cTextButton("START"));
    START->setFillColor(greenColor);
    START->setDisabled();
    START->setPosition(StartPosition);

    std::unique_ptr<cTextButton> STOP(new cTextButton("STOP"));
    STOP->setFillColor(redColor);
    STOP->setDisabled();
    STOP->setPosition(StopPosition);

    ON->onPressed = [ON_ptr = ON.get(), OFF_ptr = OFF.get(), START_ptr = START.get(), this]
                    (const sf::Event & event, cButton & ONButton)
    {
        m_status = eStatus::On;

        ON_ptr->setDisabled();
        OFF_ptr->setEnabled();
        START_ptr->setEnabled();

        //ON and set speed
        addCommands_setSpeed(speed);
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            m_spider.solveInverseKinematics(zeroVector, leg);
            addOneLegCommand_setTarget(channel, leg);
            ++leg;
        }
        //stand up
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            m_spider.solveInverseKinematics(defaultStandUpVector, leg);
            addOneLegCommand_setTarget(channel, leg);
            ++leg;
        }
    };

    OFF->onPressed = [OFF_ptr = OFF.get(), ON_ptr = ON.get(), START_ptr = START.get(), this]
                     (const sf::Event & event, cButton & OFFButton)
    {
        m_status = eStatus::Off;

        OFF_ptr->setDisabled();
        ON_ptr->setEnabled();
        START_ptr->setDisabled();

        //sit down
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            m_spider.solveInverseKinematics(defaultSitDownVector, leg);
            addOneLegCommand_setTarget(channel, leg);
            ++leg;
        }
        //shut down
        addShutDownCommand();
    };

    START->onPressed = [START_ptr = START.get(), STOP_ptr = STOP.get(), OFF_ptr = OFF.get(), this,
                        vc_ptr = viewContainer.get()](const sf::Event & event, cButton & StartButton)
    {
        START_ptr->setDisabled();
        OFF_ptr->setDisabled();
        STOP_ptr->setEnabled();

        initSplineView(vc_ptr);
        // walk sequence
        walk();
    };
    STOP->onPressed = [STOP_ptr = STOP.get(), START_ptr = START.get(), OFF_ptr = OFF.get(), this,
                       vc_ptr = viewContainer.get()](const sf::Event & event, cButton & StopButton)
    {
        STOP_ptr->setDisabled();
        START_ptr->setEnabled();
        OFF_ptr->setEnabled();

        m_mainMenu.clearContainer(vc_ptr);
        cSpline * temp_ptr = m_spline.release();
        delete temp_ptr;

        m_controlVertices.clear();
        m_commandQueue.clearQueue();
    };

    controlContainer->addWidget(std::move(ON));
    controlContainer->addWidget(std::move(OFF));
    controlContainer->addWidget(std::move(START));
    controlContainer->addWidget(std::move(STOP));

    m_mainMenu.addContainer(std::move(controlContainer));
    m_mainMenu.addContainer(std::move(viewContainer));
}

void cApplication::initSplineView(cContainer * viewContainer)
{
    custom::vector v = {0.0, 0.0, 0.0};
    m_controlVertices.push_back(v);
    v = {0.0, 100.0, 0.0};
    m_controlVertices.push_back(v);
    v = {100.0, 100.0, 0.0};
    m_controlVertices.push_back(v);

    //check Control Vertices
    if (m_controlVertices.empty())
    {
        m_controlVertices.push_back(zeroVector);
        m_controlVertices.push_back(zeroVector);
    }
    else if(m_controlVertices.size() == 1)
    {
        m_controlVertices.push_back(zeroVector);
    }

    for (auto i : m_controlVertices)
    {
        std::unique_ptr<cPoint> point(new cPoint(sf::Vector2f(i[X] + viewContainerSize.x / 2.f,
                                                 viewContainerSize.y / 2.f - i[Y]),
                                                 circleRadius + 5.f));
        point->setPointColor(redColor);
        viewContainer->addWidget(std::move(point));
    }

    m_spline = std::move(std::unique_ptr<cSpline>(new cSpline(m_controlVertices)));

    custom::pointList points;
    points = m_spline->getList();
    for (auto i : points)
    {
        std::unique_ptr<cPoint> point(new cPoint(sf::Vector2f(i[X] + viewContainerSize.x / 2.f,
                                                 viewContainerSize.y / 2.f - i[Y])));
        viewContainer->addWidget(std::move(point));
    }
}
