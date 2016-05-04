#include "constants.hpp"
#include "conversions.hpp"
#include "cPololuSerial.hpp"
#include "cSpiderRobotState.hpp"
#include <unistd.h>
#include <iostream>
#include <list>
#include "matrix.hpp"
#include "cSpline.hpp"
#include "cApplication.hpp"

int main(void)
{

    cApplication & application = cApplication::getInstance();
    application.run();

    /*
    cPololuSerial & serial = cPololuSerial::getInstance();
    cSpiderRobotState & spider = cSpiderRobotState::getInstance();
    custom::vector transferVector1, transferVector2, transferVector3, transferVector4, transferVector5,transferVector6;
    //ON
    int speed = 10;
    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        serial.setSpeed(channel, speed);
        serial.setSpeed(channel + 0x06, speed);
        serial.setSpeed(channel + 0x0C, speed);
        ++leg;
    }
    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        transferVector1 = {50.0, -50.0, 0.0};
        transferVector2 = {50.0, 0.0, 0.0};
        transferVector3 = {50.0, 50.0, 0.0};
        transferVector4 = {-50.0, -50.0, 0.0};
        transferVector5 = {-50.0, 0.0, 0.0};
        transferVector6 = {-50.0, 50.0, 0.0};
        if (leg == LEG_1)
            spider.solveInverseKinematics(transferVector1, leg);
        else if (leg == LEG_2)
            spider.solveInverseKinematics(transferVector2, leg);
        else if (leg == LEG_3)
            spider.solveInverseKinematics(transferVector3, leg);
        else if (leg == LEG_4)
            spider.solveInverseKinematics(transferVector4, leg);
        else if (leg == LEG_5)
            spider.solveInverseKinematics(transferVector5, leg);
        else if (leg == LEG_6)
            spider.solveInverseKinematics(transferVector6, leg);

        serial.setTarget(channel, spider.getQ1(leg));
        serial.setTarget(channel + 0x06, spider.getQ2(leg));
        serial.setTarget(channel + 0x0C, spider.getQ3(leg));
        ++leg;
    }
    while(serial.isMoving());
    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        transferVector1 = {0.0, 0.0, -50.0};
        transferVector2 = {0.0, 0.0, -50.0};
        transferVector3 = {0.0, 0.0, -50.0};
        transferVector4 = {0.0, 0.0, -50.0};
        transferVector5 = {0.0, 0.0, -50.0};
        transferVector6 = {0.0, 0.0, -50.0};
        if (leg == LEG_1)
            spider.solveInverseKinematics(transferVector1, leg);
        else if (leg == LEG_2)
            spider.solveInverseKinematics(transferVector2, leg);
        else if (leg == LEG_3)
            spider.solveInverseKinematics(transferVector3, leg);
        else if (leg == LEG_4)
            spider.solveInverseKinematics(transferVector4, leg);
        else if (leg == LEG_5)
            spider.solveInverseKinematics(transferVector5, leg);
        else if (leg == LEG_6)
            spider.solveInverseKinematics(transferVector6, leg);

        serial.setTarget(channel, spider.getQ1(leg));
        serial.setTarget(channel + 0x06, spider.getQ2(leg));
        serial.setTarget(channel + 0x0C, spider.getQ3(leg));
        ++leg;
    }
    while(serial.isMoving());
    //END ON


    double trim = 0.0, roll = 0.0, phi = 0.0;
    custom::vector moveVector = {0.0, 0.0, 0.0};

    char ch;
    std::cout << "c - circle move body\n"
                 "m - move body\n"
                 "r - rotate body\n"
                 "b - reverse rotate body\n"
                 "t - toHome\n"
                 "l - move leg\n"
                 "CTRL+D - exit" << std::endl;
    while (std::cin >> ch)
    {
        switch(ch)
        {
        case 'm':
            //move body
            double XX, YY, ZZ;
            while(std::cin.get() == '\n')
            {
                std::cout << "enter coordinates: ";
                std::cin >> XX >> YY >> ZZ;
                std::cin.get();
                moveVector = {XX, YY, ZZ};
                spider.moveBody(moveVector);
                for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
                {
                    serial.setTarget(channel, spider.getQ1(leg));
                    serial.setTarget(channel + 0x06, spider.getQ2(leg));
                    serial.setTarget(channel + 0x0C, spider.getQ3(leg));
                    ++leg;
                }
                while(serial.isMoving());
                std::cout << "press enter for continue: ";
            }
            break;
        case 'r':
            //rotate body
            while(std::cin.get() == '\n')
            {
                std::cout << "enter trim, roll, course angles: ";
                std::cin >> trim >> roll >> phi;
                std::cin.get();
                spider.rotateBody(trim, roll, phi);
                for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
                {
                    serial.setTarget(channel, spider.getQ1(leg));
                    serial.setTarget(channel + 0x06, spider.getQ2(leg));
                    serial.setTarget(channel + 0x0C, spider.getQ3(leg));
                    ++leg;
                }
                while(serial.isMoving());
                std::cout << "press enter for continue: ";
            }
            break;
        case 'b':
            //reverse rotate body
            while(std::cin.get() == '\n')
            {
                std::cout << "enter trim, roll, course angles: ";
                std::cin >> trim >> roll >> phi;
                std::cin.get();
                spider.reverseRotateBody(trim, roll, phi);
                for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
                {
                    serial.setTarget(channel, spider.getQ1(leg));
                    serial.setTarget(channel + 0x06, spider.getQ2(leg));
                    serial.setTarget(channel + 0x0C, spider.getQ3(leg));
                    ++leg;
                }
                while(serial.isMoving());
                std::cout << "press enter for continue: ";
            }
            break;
        case 't':
            //to home
            while(std::cin.get() == '\n')
            {
                spider.moveToHome();
                for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
                {
                    serial.setTarget(channel, spider.getQ1(leg));
                    serial.setTarget(channel + 0x06, spider.getQ2(leg));
                    serial.setTarget(channel + 0x0C, spider.getQ3(leg));
                    ++leg;
                }
                while(serial.isMoving());
                spider.rotateToHome();
                for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
                {
                    serial.setTarget(channel, spider.getQ1(leg));
                    serial.setTarget(channel + 0x06, spider.getQ2(leg));
                    serial.setTarget(channel + 0x0C, spider.getQ3(leg));
                    ++leg;
                }
                while(serial.isMoving());
                std::cout << "press enter for continue: ";
            }
            break;
        case 'l':
            //move leg
            double XXX, YYY, ZZZ;
            while(std::cin.get() == '\n')
            {
                std::cout << "enter coordinates: ";
                std::cin >> XXX >> YYY >> ZZZ;
                std::cin.get();
                moveVector = {XXX, YYY, ZZZ};
                spider.solveInverseKinematics(moveVector, LEG_2);
                unsigned char channel = 0x01;
                serial.setTarget(channel, spider.getQ1(LEG_2));
                serial.setTarget(channel + 0x06, spider.getQ2(LEG_2));
                serial.setTarget(channel + 0x0C, spider.getQ3(LEG_2));
                std::cout << "press enter for continue: ";
            }
            break;
        default: break;
        }
        std::cout << "c - circle move body\n"
                     "m - move body\n"
                     "r - rotate body\n"
                     "b - reverse rotate body\n"
                     "t - toHome\n"
                     "l - move leg\n"
                     "CTRL+D - exit" << std::endl;
    }

    //OFF
    for (unsigned char channel = 0x00, leg = 0; channel <= 0x05; ++channel)
    {
        transferVector1 = {0.0, 0.0, 50.0};
        transferVector2 = {0.0, 0.0, 50.0};
        transferVector3 = {0.0, 0.0, 50.0};
        transferVector4 = {0.0, 0.0, 50.0};
        transferVector5 = {0.0, 0.0, 50.0};
        transferVector6 = {0.0, 0.0, 50.0};
        if (leg == LEG_1)
            spider.solveInverseKinematics(transferVector1, leg);
        else if (leg == LEG_2)
            spider.solveInverseKinematics(transferVector2, leg);
        else if (leg == LEG_3)
            spider.solveInverseKinematics(transferVector3, leg);
        else if (leg == LEG_4)
            spider.solveInverseKinematics(transferVector4, leg);
        else if (leg == LEG_5)
            spider.solveInverseKinematics(transferVector5, leg);
        else if (leg == LEG_6)
            spider.solveInverseKinematics(transferVector6, leg);

        serial.setTarget(channel, spider.getQ1(leg));
        serial.setTarget(channel + 0x06, spider.getQ2(leg));
        serial.setTarget(channel + 0x0C, spider.getQ3(leg));
        ++leg;
    }
    while(serial.isMoving());

    for (unsigned char channel = 0x00; channel <= 0x11; ++channel)
    {
        serial.shutDown(channel);
    }
    //END OFF
    /*/
}
