//cSpiderRobotState.hpp

#ifndef SPIDERROBOTSTATE_HPP
#define SPIDERROBOTSTATE_HPP

#include "constants.hpp"
#include "cSingleton.hpp"

struct sLeg
{
    custom::matrix baseToZero;
    custom::generalizedCoordinates currentState;
    custom::generalizedCoordinates calculatedState;
    custom::generalizedMsCoordinates calculatedMsState;
    custom::vector currentStateVector;
    custom::vector calculatedStateVector;
    custom::vector L0_vectorSC0;
};

class cSpiderRobotState : public cSingleton
{
private:
    double m_courseAngle, m_rollAngle, m_trimAngle;
    custom::msOffsets m_msOffset;
    custom::offsets m_degOffsetSC0;
    sLeg m_legs[LEGS];
    custom::vector m_L1_vector, m_L2_vector, m_L3_vector;
    custom::matrix m_fullRotation_1_0_left, m_fullRotation_1_0_right,
                   m_fullRotation_2_1, m_fullRotation_3_2;
    custom::vector m_bodyStateVector;
private:
    void solveInverseKinematics(const int & leg);
    void findBaseToZero(const int & leg);
    custom::vector findVectorSC0(const custom::vector & transferVector, const int & leg);
    void translationToMs(const int & leg);
    void swapState(const int & leg);
    void simpleBodyRotation(const double & deltaTrimAngle, const double & deltaRollAngle, const double & deltaCourseAngle);
protected:
    cSpiderRobotState();
public:
    ~cSpiderRobotState() {};
    static cSpiderRobotState & getInstance()
    {
        static cSpiderRobotState instance;
        return instance;
    }
    void solveInverseKinematics(custom::vector & transferVector, const int & leg);
    double getQ1(int leg) const {return m_legs[leg].calculatedMsState[Q_1];};
    double getQ2(int leg) const {return m_legs[leg].calculatedMsState[Q_2];};
    double getQ3(int leg) const {return m_legs[leg].calculatedMsState[Q_3];};
    void rotateBody(const double & deltaTrimAngle, const double & deltaRollAngle, const double & deltaCourseAngle);
    void reverseRotateBody(const double & deltaTrimAngle, const double & deltaRollAngle, const double & deltaCourseAngle);
    void moveBody(const custom::vector & transferVector);
    void moveToHome();
    void rotateToHome();
};

#endif // SPIDERROBOTSTATE_HPP
