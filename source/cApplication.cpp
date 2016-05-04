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
        custom::vector transferVector1 = {50.0, -50.0, 0.0};
        custom::vector transferVector2 = {50.0, 0.0, 0.0};
        custom::vector transferVector3 = {50.0, 50.0, 0.0};
        custom::vector transferVector4 = {-50.0, -50.0, 0.0};
        custom::vector transferVector5 = {-50.0, 0.0, 0.0};
        custom::vector transferVector6 = {-50.0, 50.0, 0.0};
        if (leg == LEG_1)
            m_spider.solveInverseKinematics(transferVector1, leg);
        else if (leg == LEG_2)
            m_spider.solveInverseKinematics(transferVector2, leg);
        else if (leg == LEG_3)
            m_spider.solveInverseKinematics(transferVector3, leg);
        else if (leg == LEG_4)
            m_spider.solveInverseKinematics(transferVector4, leg);
        else if (leg == LEG_5)
            m_spider.solveInverseKinematics(transferVector5, leg);
        else if (leg == LEG_6)
            m_spider.solveInverseKinematics(transferVector6, leg);

        m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
        m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
        m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
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

        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            m_commandQueue.addCommand(&cPololuSerial::setSpeed, channel, m_speed);
            m_commandQueue.addCommand(&cPololuSerial::setSpeed, channel + 0x06, m_speed);
            m_commandQueue.addCommand(&cPololuSerial::setSpeed, channel + 0x0C, m_speed);
            ++leg;
        }
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            custom::vector transferVector1 = {0.0, 0.0, 0.0};
            custom::vector transferVector2 = {0.0, 0.0, 0.0};
            custom::vector transferVector3 = {0.0, 0.0, 0.0};
            custom::vector transferVector4 = {0.0, 0.0, 0.0};
            custom::vector transferVector5 = {0.0, 0.0, 0.0};
            custom::vector transferVector6 = {0.0, 0.0, 0.0};
            if (leg == LEG_1)
                m_spider.solveInverseKinematics(transferVector1, leg);
            else if (leg == LEG_2)
                m_spider.solveInverseKinematics(transferVector2, leg);
            else if (leg == LEG_3)
                m_spider.solveInverseKinematics(transferVector3, leg);
            else if (leg == LEG_4)
                m_spider.solveInverseKinematics(transferVector4, leg);
            else if (leg == LEG_5)
                m_spider.solveInverseKinematics(transferVector5, leg);
            else if (leg == LEG_6)
                m_spider.solveInverseKinematics(transferVector6, leg);

            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
            ++leg;
        }
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            custom::vector transferVector1 = {0.0, 0.0, -50.0};
            custom::vector transferVector2 = {0.0, 0.0, -50.0};
            custom::vector transferVector3 = {0.0, 0.0, -50.0};
            custom::vector transferVector4 = {0.0, 0.0, -50.0};
            custom::vector transferVector5 = {0.0, 0.0, -50.0};
            custom::vector transferVector6 = {0.0, 0.0, -50.0};
            if (leg == LEG_1)
                m_spider.solveInverseKinematics(transferVector1, leg);
            else if (leg == LEG_2)
                m_spider.solveInverseKinematics(transferVector2, leg);
            else if (leg == LEG_3)
                m_spider.solveInverseKinematics(transferVector3, leg);
            else if (leg == LEG_4)
                m_spider.solveInverseKinematics(transferVector4, leg);
            else if (leg == LEG_5)
                m_spider.solveInverseKinematics(transferVector5, leg);
            else if (leg == LEG_6)
                m_spider.solveInverseKinematics(transferVector6, leg);

            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
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

        m_spider.moveToHome();
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
            ++leg;
        }
        m_spider.rotateToHome();
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
            ++leg;
        }
        for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
        {
            custom::vector transferVector1 = {0.0, 0.0, 50.0};
            custom::vector transferVector2 = {0.0, 0.0, 50.0};
            custom::vector transferVector3 = {0.0, 0.0, 50.0};
            custom::vector transferVector4 = {0.0, 0.0, 50.0};
            custom::vector transferVector5 = {0.0, 0.0, 50.0};
            custom::vector transferVector6 = {0.0, 0.0, 50.0};
            if (leg == LEG_1)
                m_spider.solveInverseKinematics(transferVector1, leg);
            else if (leg == LEG_2)
                m_spider.solveInverseKinematics(transferVector2, leg);
            else if (leg == LEG_3)
                m_spider.solveInverseKinematics(transferVector3, leg);
            else if (leg == LEG_4)
                m_spider.solveInverseKinematics(transferVector4, leg);
            else if (leg == LEG_5)
                m_spider.solveInverseKinematics(transferVector5, leg);
            else if (leg == LEG_6)
                m_spider.solveInverseKinematics(transferVector6, leg);

            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
            ++leg;
        }
        //shut down
        for (unsigned char channel = 0x00; channel <= 0x05; ++channel)
        {
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, 0x00);
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, 0x00);
            m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, 0x00);
        }
    };

    START->onPressed = [START_ptr = START.get(), STOP_ptr = STOP.get(), OFF_ptr = OFF.get(), this,
                        vc_ptr = viewContainer.get()](const sf::Event & event, cButton & StartButton)
    {
        START_ptr->setDisabled();
        OFF_ptr->setDisabled();
        STOP_ptr->setEnabled();

        initSplineView(vc_ptr);
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
    custom::pointList controlPoints;
    custom::vector v = {0.0, 0.0, 0.0};
    controlPoints.push_back(v);
    v = {0.0, 110.0, 0.0};
    controlPoints.push_back(v);
    v = {0.0, 220.0, 0.0};
    controlPoints.push_back(v);
    v = {220.0, 220.0, 0.0};
    controlPoints.push_back(v);
    
    for (auto i : controlPoints)
    {
        std::unique_ptr<cPoint> point(new cPoint(sf::Vector2f(i[X] + viewContainerSize.x / 2.f,
                                                              viewContainerSize.y / 2.f - i[Y]),
                                                 circleRadius + 5.f));
        point->setPointColor(redColor);
        viewContainer->addWidget(std::move(point));
    }

    m_spline = std::move(std::unique_ptr<cSpline>(new cSpline(controlPoints)));

    controlPoints = m_spline->getList();
    for (auto i : controlPoints)
    {
        std::unique_ptr<cPoint> point(new cPoint(sf::Vector2f(i[X] + viewContainerSize.x / 2.f,
                                                              viewContainerSize.y / 2.f - i[Y])));
        viewContainer->addWidget(std::move(point));
    }
}
