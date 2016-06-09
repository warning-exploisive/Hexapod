// TacticLevel.cpp

#include "cApplication.hpp"
#include "matrix.hpp"
#include "conversions.hpp"
void cApplication::walk()
{
    //find delta_courseAngles and delta_lenghts for each point
    custom::pointList pointList = m_spline->getList();
    int listSize = pointList.size();
    std::cout << "listSize = " << listSize << std::endl;

    std::vector<double> delta_courseAngles;
    std::vector<double> delta_lenghts;
    double d_angle = 0.0, sum_angle = 0.0, full_angle = 0.0;
    custom::vector temp = {0.0, 0.0, 0.0};

    for (int i = 0; i < listSize - 2; ++i)
    {
        temp = pointList[i + 1] - pointList[i];
        full_angle = atan2(-temp[X], temp[Y]);
        d_angle = full_angle - sum_angle;
        sum_angle += d_angle;
        delta_courseAngles.push_back(radToDeg(d_angle));
        delta_lenghts.push_back(findAbsValue(temp));
    }
    temp = pointList[pointList.size() - 1] - m_controlVertices[m_controlVertices.size() - 2];
    full_angle = atan2(-temp[X], temp[Y]);
    d_angle = full_angle - sum_angle;
    sum_angle += d_angle;
    delta_courseAngles.push_back(radToDeg(d_angle));
    delta_lenghts.push_back(findAbsValue(pointList[pointList.size() - 1] - pointList[pointList.size() - 2]));

    //add help points
    if(listSize != 1)
    {
        if((listSize - 1) % 2)
        {
            delta_courseAngles.push_back(0.0);
            delta_lenghts.push_back(0.0);
            delta_courseAngles.push_back(0.0);
            delta_lenghts.push_back(0.0);
        }
        else
        {
            delta_courseAngles.push_back(0.0);
            delta_lenghts.push_back(0.0);
        }
        delta_courseAngles.push_back(0.0);
        delta_lenghts.push_back(0.0);
    }
    delta_courseAngles.push_back(0.0);
    delta_lenghts.push_back(0.0);

    //*////////////////////////////////////////////////////////
    for(unsigned int i = 0; i < delta_courseAngles.size(); ++i)
    {
        std::cout << "angle" << i + 1 << ": " << delta_courseAngles[i] << std::endl;
    }
    for(unsigned int i = 0; i < delta_lenghts.size(); ++i)
    {
        std::cout << "lenght" << i + 1 << ": " << delta_lenghts[i] << std::endl;
    }
    std::cout << "sum_A = "<< radToDeg(sum_angle) << std::endl;
    //*//////////////////////////////////////////////////////////

    //check angles value for valid sequence
    if (abs(delta_courseAngles[0]) > HALF_CIRCLE / 2.0)
    //sector III and IV
    {
        delta_courseAngles[0] = sign(delta_courseAngles[0]) * (abs(delta_courseAngles[0]) - HALF_CIRCLE);
        for(unsigned int i = 0; i < delta_lenghts.size(); ++i)
            delta_lenghts[i] = - delta_lenghts[i];
    }
    if (abs(delta_courseAngles[0]) > 5.0)
    {
        // rotate on the spot and update init angle
        int counter = (int)(delta_courseAngles[0] / rotateOnTheSpotAngle);
        std::cout << "count = " << counter << std::endl;
        delta_courseAngles[0] -= rotateOnTheSpotAngle * counter;
        rotateOnTheSpot(counter);
    }
    std::cout << "d_c_0 = " << delta_courseAngles[0]<< std::endl;

    //remove loops on spline
    for(unsigned int inner = 0; inner < delta_courseAngles.size(); ++inner)
        if((abs(delta_courseAngles[inner]) > HALF_CIRCLE / 2.0) && (abs(delta_courseAngles[inner]) <= HALF_CIRCLE))
            delta_courseAngles[inner] = (double)sign(delta_courseAngles[inner]) * (abs(delta_courseAngles[inner]) - HALF_CIRCLE);
        else if(abs(delta_courseAngles[inner]) > HALF_CIRCLE)
            delta_courseAngles[inner] = (double)sign(delta_courseAngles[inner]) * (abs(delta_courseAngles[inner]) - HALF_CIRCLE * 2.0);
    //*////////////////////////////////////////////////////////
    for(unsigned int i = 0; i < delta_courseAngles.size(); ++i)
    {
        std::cout << "angle" << i + 1 << ": " << delta_courseAngles[i] << std::endl;
    }
    for(unsigned int i = 0; i < delta_lenghts.size(); ++i)
    {
        std::cout << "lenght" << i + 1 << ": " << delta_lenghts[i] << std::endl;
    }
    std::cout << "sum_A = "<< radToDeg(sum_angle) << std::endl;
    //*//////////////////////////////////////////////////////////

    //run walk
    int leg, counter;
    for(unsigned int inner = 0; inner < delta_courseAngles.size() - 2; ++inner)
    {
        if((inner % 3) == 0)
        {
            if(delta_courseAngles[inner] > EPS)
            {
                //left side
                counter = 0;
                leg = 0;
            }
            else
            {
                //right side
                counter = 0;
                leg = 3;
            }
        }
        walkSequence(delta_courseAngles[inner], delta_courseAngles[inner + 1], delta_courseAngles[inner + 2],
                     delta_lenghts[inner], delta_lenghts[inner + 1], delta_lenghts[inner + 2], leg);
        ++counter;
        ++leg;
    }
}

void cApplication::rotateOnTheSpot(const int & counter)
{
    for(int i = abs(counter); i > 0; --i)
    {
        m_spider.rotateBody(0.0, 0.0, sign(counter) * rotateOnTheSpotAngle);
        addCommands_setTarget();
        for(int leg = 0; leg < LEGS; ++leg)
            step(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, leg);
    }
}

void cApplication::step(const double & delta_courseAngle_1, const double & delta_courseAngle_2,
                        const double & delta_courseAngle_3, const double & delta_lenght_1,
                        const double & delta_lenght_2, const double & delta_lenght_3, const int & leg)
{
    custom::vector transferVector = m_spider.simpleStep(delta_courseAngle_1,
                                                        delta_courseAngle_2,
                                                        delta_courseAngle_3,
                                                        delta_lenght_1,
                                                        delta_lenght_2,
                                                        delta_lenght_3,
                                                        leg);
    custom::vector transferVector_up = {transferVector[X] / 2.0,
                                        transferVector[Y] / 2.0,
                                        transferVector[Z] + platformHeight};
    custom::vector transferVector_down = {transferVector[X] / 2.0,
                                          transferVector[Y] / 2.0,
                                          transferVector[Z] - platformHeight};
    m_spider.solveInverseKinematics(transferVector_up, leg);
    addCommands_setSpeed(speed * 2);
    addCommands_setTarget();
    m_spider.solveInverseKinematics(transferVector_down, leg);
    addCommands_setSpeed(speed);
    addCommands_setTarget();
}

void cApplication::addCommands_setTarget()
{
    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        addOneLegCommand_setTarget(channel, leg);
        ++leg;
    }
}

void cApplication::addOneLegCommand_setTarget(const unsigned char & channel, const unsigned char & leg)
{
    m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, m_spider.getQ1(leg));
    m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, m_spider.getQ2(leg));
    m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, m_spider.getQ3(leg));
}

void cApplication::addShutDownCommand()
{
    for (unsigned char channel = 0x00; channel <= 0x05; ++channel)
    {
        m_commandQueue.addCommand(&cPololuSerial::setTarget, channel, 0x00);
        m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x06, 0x00);
        m_commandQueue.addCommand(&cPololuSerial::setTarget, channel + 0x0C, 0x00);
    }
}

void cApplication::addCommands_setSpeed(const int & speed)
{
    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        m_commandQueue.addCommand(&cPololuSerial::setSpeed, channel, speed);
        m_commandQueue.addCommand(&cPololuSerial::setSpeed, channel + 0x06, speed);
        m_commandQueue.addCommand(&cPololuSerial::setSpeed, channel + 0x0C, speed);
        ++leg;
    }
}

void cApplication::walkSequence(const double & delta_courseAngle_1, const double & delta_courseAngle_2,
                                const double & delta_courseAngle_3, const double & delta_lenght_1,
                                const double & delta_lenght_2, const double & delta_lenght_3, const int & leg)
{
    //rotate
    m_spider.rotateBody(0.0, 0.0, delta_courseAngle_1);
    addCommands_setTarget();
    //move
    m_spider.moveBody({0.0, delta_lenght_1, 0.0});
    addCommands_setTarget();
    //step
    step(delta_courseAngle_1, delta_courseAngle_2, delta_courseAngle_3,
         delta_lenght_1, delta_lenght_2, delta_lenght_3, leg);
    step(delta_courseAngle_1, delta_courseAngle_2, delta_courseAngle_3,
         delta_lenght_1, delta_lenght_2, delta_lenght_3, LEGS - leg - 1);
}
