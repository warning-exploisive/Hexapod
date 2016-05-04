// cSpiderRobotState.cpp

#include <iostream>
#include "constants.hpp"
#include "conversions.hpp"
#include "cSpiderRobotState.hpp"
#include "matrix.hpp"

cSpiderRobotState::cSpiderRobotState():
m_courseAngle(0.0),
m_rollAngle(0.0),
m_trimAngle(0.0)
{
    //set angle(degrees) offsets SC0 on BASE
    m_degOffsetSC0 = {SC0_1_DEG_OFFSET, SC0_2_DEG_OFFSET, SC0_3_DEG_OFFSET,
                      SC0_4_DEG_OFFSET, SC0_5_DEG_OFFSET, SC0_6_DEG_OFFSET};
    //set miliseconds offsets
    m_msOffset = {L1_Q1_MS_OFFSET, L1_Q2_MS_OFFSET, L1_Q3_MS_OFFSET,
                  L2_Q1_MS_OFFSET, L2_Q2_MS_OFFSET, L2_Q3_MS_OFFSET,
                  L3_Q1_MS_OFFSET, L3_Q2_MS_OFFSET, L3_Q3_MS_OFFSET,
                  L4_Q1_MS_OFFSET, L4_Q2_MS_OFFSET, L4_Q3_MS_OFFSET,
                  L5_Q1_MS_OFFSET, L5_Q2_MS_OFFSET, L5_Q3_MS_OFFSET,
                  L6_Q1_MS_OFFSET, L6_Q2_MS_OFFSET, L6_Q3_MS_OFFSET};
    //set bodyStateVector
    m_bodyStateVector = {0.0, 0.0, 0.0};
    //define robot lenght vectors
    m_legs[LEG_1].L0_vectorSC0 = {L0_1 * sin(degToRad(SC0_1_DEG_OFFSET - L0_1_DEG_OFFSET)),
                                  L0_1 * cos(degToRad(SC0_1_DEG_OFFSET - L0_1_DEG_OFFSET)), 0.0};
    m_legs[LEG_2].L0_vectorSC0 = {L0_2 * sin(degToRad(SC0_2_DEG_OFFSET - L0_2_DEG_OFFSET)),
                                  L0_2 * cos(degToRad(SC0_2_DEG_OFFSET - L0_2_DEG_OFFSET)), 0.0};
    m_legs[LEG_3].L0_vectorSC0 = {L0_3 * sin(degToRad(SC0_3_DEG_OFFSET - L0_3_DEG_OFFSET)),
                                  L0_3 * cos(degToRad(SC0_3_DEG_OFFSET - L0_3_DEG_OFFSET)), 0.0};
    m_legs[LEG_4].L0_vectorSC0 = {L0_4 * sin(degToRad(SC0_4_DEG_OFFSET - L0_4_DEG_OFFSET)),
                                  L0_4 * cos(degToRad(SC0_4_DEG_OFFSET - L0_4_DEG_OFFSET)), 0.0};
    m_legs[LEG_5].L0_vectorSC0 = {L0_5 * sin(degToRad(SC0_5_DEG_OFFSET - L0_5_DEG_OFFSET)),
                                  L0_5 * cos(degToRad(SC0_5_DEG_OFFSET - L0_5_DEG_OFFSET)), 0.0};
    m_legs[LEG_6].L0_vectorSC0 = {L0_6 * sin(degToRad(SC0_6_DEG_OFFSET - L0_6_DEG_OFFSET)),
                                  L0_6 * cos(degToRad(SC0_6_DEG_OFFSET - L0_6_DEG_OFFSET)), 0.0};
    m_L1_vector = {L1X, L1Y, L1Z};
    m_L2_vector = {L2X, L2Y, - L2Z};
    m_L3_vector = {L3X, L3Y, L3Z};
    //define full rotation matrix
    m_fullRotation_1_0_left = rotationMatrixZ(0.0) *
                              compositeRotationMatrix(ALPHA_1_LEFT, BETTA_1_LEFT, GAMMA_1_LEFT);
    m_fullRotation_1_0_right = rotationMatrixZ(0.0) *
                               compositeRotationMatrix(ALPHA_1_RIGHT, BETTA_1_RIGHT, GAMMA_1_RIGHT);
    m_fullRotation_2_1 = rotationMatrixZ(0.0) * compositeRotationMatrix(ALPHA_2, BETTA_2, GAMMA_2);
    m_fullRotation_3_2 = rotationMatrixZ(0.0) * compositeRotationMatrix(ALPHA_3, BETTA_3, GAMMA_3);

    for (int leg = 0; leg < LEGS; ++leg)
    {
        //define baseToZero rotation matrix
        findBaseToZero(leg);
        //set currentState
        for (int joint = 0; joint < JOINTS; ++joint)
            m_legs[leg].currentState[joint] = 0.0;
        //calculate currentStateVector
        if (leg == LEG_1 || leg == LEG_2 || leg == LEG_3)
            m_legs[leg].currentStateVector = m_fullRotation_1_0_left * (m_L1_vector + m_fullRotation_2_1 *
                                             (m_L2_vector + m_fullRotation_3_2 * m_L3_vector));
        else if (leg == LEG_4 || leg == LEG_5 || leg == LEG_6)
            m_legs[leg].currentStateVector = m_fullRotation_1_0_right * (m_L1_vector + m_fullRotation_2_1 *
                                             (m_L2_vector + m_fullRotation_3_2 * m_L3_vector));
        //find calculatedState (solve IK)
        custom::vector transferVector = {0.0, 0.0, 0.0};
        solveInverseKinematics(transferVector, leg);
    }
}

void cSpiderRobotState::solveInverseKinematics(custom::vector & transferVector, const int & leg)
{
    //find CalculatedStateVector
    m_legs[leg].calculatedStateVector = m_legs[leg].currentStateVector + findVectorSC0(transferVector, leg);
    solveInverseKinematics(leg);
}

void cSpiderRobotState::solveInverseKinematics(const int & leg)
{
    //find full Q1 rotation angle
    double Q1_alpha;
    if (leg == LEG_1 || leg == LEG_2 || leg == LEG_3)
        //left side,legs 1, 2, 3
        Q1_alpha = radToDeg(atan2(m_legs[leg].calculatedStateVector[X], m_legs[leg].calculatedStateVector[Y]));
    else if (leg == LEG_4 || leg == LEG_5 || leg == LEG_6)
        //right side,legs 4, 5, 6
        Q1_alpha = - radToDeg(atan2(m_legs[leg].calculatedStateVector[X], m_legs[leg].calculatedStateVector[Y]));

	double alpha = radToDeg(asin((L1Z - L2Z)/(sqrt(m_legs[leg].calculatedStateVector[X] *
                   m_legs[leg].calculatedStateVector[X] + m_legs[leg].calculatedStateVector[Y] *
                   m_legs[leg].calculatedStateVector[Y]))));
	double Q1 = Q1_alpha - alpha;

	//find local coordinates of leg peak
	double localStateX;
	double localStateY;
	if (leg == LEG_1 || leg == LEG_2 || leg == LEG_3)
    {
        //left side,legs 1, 2, 3
        localStateX = m_legs[leg].calculatedStateVector[X] - (cos(degToRad(m_legs[leg].currentState[Q_1])) *
                      (L1Z - L2Z) - L1Y * sin(degToRad(m_legs[leg].currentState[Q_1])));
        localStateY = m_legs[leg].calculatedStateVector[Y] - (L1Y * cos(degToRad(m_legs[leg].currentState[Q_1])) +
                      sin(degToRad(m_legs[leg].currentState[Q_1])) * (L1Z - L2Z));
    }
    else if (leg == LEG_4 || leg == LEG_5 || leg == LEG_6)
    {
        //right side,legs 4, 5, 6
        localStateX = m_legs[leg].calculatedStateVector[X] - (- cos(degToRad(Q1)) *
                      (L1Z - L2Z) - L1Y * sin(degToRad(Q1)));
        localStateY = m_legs[leg].calculatedStateVector[Y] - (L1Y * cos(degToRad(Q1)) -
                      sin(degToRad(Q1)) * (L1Z - L2Z));
    }
    //find position of the local zero
    double localX = sqrt(localStateX * localStateX + localStateY * localStateY);
    double localY = m_legs[leg].calculatedStateVector[Z];
	//intersection point coordinates
	double intersectionX = 0.0;
	double intersectionY = 0.0;
    //find intersection X, Y
	if ((abs(localX) < EPS) && (abs(localY) < EPS))
		std::cout << "ERROR: concentric circles" << std::endl;
	else
	{
		//coefficients of the equation of the line
		double A = -2.0 * localX;
		double B = -2.0 * localY;
		double C = localX * localX + localY * localY + L2Y * L2Y - L3Z * L3Z;
		//find coordinates of the intersection
		double x0 = -A*C/(A * A + B * B);
		double y0 = -B*C/(A * A + B * B);
		if (C * C > L2Y * L2Y * (A * A + B * B) + EPS)
			//no intersection
			std::cout << "ERROR: no intersection" << std::endl;
		else if (abs(C * C - L2Y * L2Y * (A * A + B * B)) < EPS)
		{
			//1 intersection(tangent)
			intersectionX = x0;
			intersectionY = y0;
		}
		else
		{
			//2 intersections
			double D = L2Y * L2Y - C * C / (A * A + B * B);
			double mult = sqrt (D / (A * A + B * B));
			double Ax, Ay, Bx, By;
			Ax = x0 + B * mult;
			Bx = x0 - B * mult;
			Ay = y0 - A * mult;
			By = y0 + A * mult;
			//selection intersection point
			if (Ay > By)
			{
				intersectionX = Ax;
				intersectionY = Ay;
			}
			else
			{
				intersectionX = Bx;
				intersectionY = By;
			}
		}
	}
    //Q2, Q3 angle calculation, based on local coordinates of the intersection point
    double Q2, Q3;
    if (leg == LEG_1 || leg == LEG_2 || leg == LEG_3)
    {
        //left side,legs 1, 2, 3
        Q2 = radToDeg(atan2(intersectionY, intersectionX));
        Q3 = radToDeg(atan2(intersectionY - localY, localX - intersectionX)) + Q2;
    }
    else if (leg == LEG_4 || leg == LEG_5 || leg == LEG_6)
    {
        //right side,legs 4, 5, 6
        Q2 = radToDeg(atan2( - intersectionY, intersectionX));
        Q3 = radToDeg(atan2(intersectionY - localY, localX - intersectionX)) - Q2;
    }

    m_legs[leg].calculatedState[Q_1] = Q1;
    m_legs[leg].calculatedState[Q_2] = Q2;
    m_legs[leg].calculatedState[Q_3] = Q3;

    translationToMs(leg);
    swapState(leg);
}

void cSpiderRobotState::findBaseToZero(const int & leg)
{
    m_legs[leg].baseToZero = trasposeMatrix(compositeRotationMatrix(0.0, 0.0, m_degOffsetSC0[leg] - radToDeg(M_PI_2)));
}

custom::vector cSpiderRobotState::findVectorSC0(const custom::vector & transferVector, const int & leg)
{
	custom::vector v_SCO_out = {0};
	v_SCO_out = m_legs[leg].baseToZero * transferVector;
    return v_SCO_out;
}

void cSpiderRobotState::translationToMs(const int & leg)
{
    for (int joint = 0; joint < JOINTS; ++joint)
        if ((leg == LEG_4 || leg == LEG_5 || leg == LEG_6) && (joint == Q_3 || joint == Q_1))
            m_legs[leg].calculatedMsState[joint] = 4 * (degToMiliseconds( - m_legs[leg].calculatedState[joint]) +
                                                   m_msOffset[leg][joint]);
        else
            m_legs[leg].calculatedMsState[joint] = 4 * (degToMiliseconds(m_legs[leg].calculatedState[joint]) +
                                                   m_msOffset[leg][joint]);
}

void cSpiderRobotState::swapState(const int & leg)
{
    m_legs[leg].currentState = m_legs[leg].calculatedState;
    m_legs[leg].currentStateVector = m_legs[leg].calculatedStateVector;
}

void cSpiderRobotState::moveBody(const custom::vector & transferVector)
{
    m_bodyStateVector += transferVector;
    std::cout << "X = " << m_bodyStateVector[X] << " Y = " << m_bodyStateVector[Y] << " Z = "
              << m_bodyStateVector[Z] << std::endl;
    for (int leg = 0; leg < LEGS; ++leg)
    {
        //find calculatedStateVector in SC0
        m_legs[leg].calculatedStateVector = m_legs[leg].currentStateVector - findVectorSC0(transferVector, leg);
        solveInverseKinematics(leg);
    }
}

void cSpiderRobotState::simpleBodyRotation(const double & deltaTrimAngle,
                                           const double & deltaRollAngle,
                                           const double & deltaCourseAngle)
{
    //find subtraction vector in SC0
    for (int leg = 0; leg < LEGS; ++leg)
    {
        //find LO_vector in BASE
        custom::vector R_1 = compositeRotationMatrix(m_trimAngle, m_rollAngle, 0.0) *
                             trasposeMatrix(m_legs[leg].baseToZero) * m_legs[leg].L0_vectorSC0;
        //rotate and find new L0
        custom::vector R_2 = compositeRotationMatrix(deltaTrimAngle, deltaRollAngle, deltaCourseAngle) * R_1;
        //find new calculated state
        m_legs[leg].calculatedStateVector =
            trasposeMatrix(compositeRotationMatrix(0.0, 0.0, deltaCourseAngle)) *
            (m_legs[leg].currentStateVector - findVectorSC0(R_2 - R_1, leg));
        solveInverseKinematics(leg);
    }
    m_courseAngle += deltaCourseAngle;
    m_rollAngle += deltaRollAngle;
    m_trimAngle += deltaTrimAngle;
    //find body state vector after rotation
    m_bodyStateVector = trasposeMatrix(compositeRotationMatrix(deltaTrimAngle, deltaRollAngle, deltaCourseAngle)) *
                        m_bodyStateVector;
}

void cSpiderRobotState::rotateBody(const double & deltaTrimAngle,
                                   const double & deltaRollAngle,
                                   const double & deltaCourseAngle)
{
    simpleBodyRotation(deltaTrimAngle, 0.0, 0.0);
    simpleBodyRotation(0.0, deltaRollAngle, 0.0);
    simpleBodyRotation(0.0, 0.0, deltaCourseAngle);
    std::cout << "T = " << m_trimAngle << " R = " << m_rollAngle << " C = "
              << m_courseAngle << std::endl;
}

void cSpiderRobotState::reverseRotateBody(const double & deltaTrimAngle,
                                          const double & deltaRollAngle,
                                          const double & deltaCourseAngle)
{
    simpleBodyRotation(0.0, 0.0, deltaCourseAngle);
    simpleBodyRotation(0.0, deltaRollAngle, 0.0);
    simpleBodyRotation(deltaTrimAngle, 0.0, 0.0);
    std::cout << "T = " << m_trimAngle << " R = " << m_rollAngle << " C = "
              << m_courseAngle << std::endl;
}

void cSpiderRobotState::moveToHome()
{
    moveBody(-m_bodyStateVector);
}

void cSpiderRobotState::rotateToHome()
{
    reverseRotateBody(- m_trimAngle, - m_rollAngle, - m_courseAngle);
}
