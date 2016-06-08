//constatnts.hpp

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <array>
#include <vector>

//matrix & vectors parameters
constexpr int AXIS = 3;			    //number of axis
constexpr int LEGS = 6;			    //number of legs
constexpr int JOINTS = 3;			//number of joints

//index
constexpr unsigned int X = 0,
                       Y = 1,
                       Z = 2,
                       Q_1 = 0,
                       Q_2 = 1,
                       Q_3 = 2,
                       LEG_1 = 0,
                       LEG_2 = 1,
                       LEG_3 = 2,
                       LEG_4 = 3,
                       LEG_5 = 4,
                       LEG_6 = 5;

//define zero
constexpr double EPS = 1E-06;

//geometry of robot
constexpr double ALPHA_1_LEFT = 0.0,
                 BETTA_1_LEFT = 90.0,
                 GAMMA_1_LEFT = 0.0,
                 ALPHA_1_RIGHT = 0.0,
                 BETTA_1_RIGHT = -90.0,
                 GAMMA_1_RIGHT = 0.0,
                 ALPHA_2 = 0.0,
                 BETTA_2 = 0.0,
                 GAMMA_2 = 0.0,
                 ALPHA_3 = -90.0,
                 BETTA_3 = 0.0,
                 GAMMA_3 = 0.0;

//legs lenght
constexpr double L1X = 0.0,
                 L1Y = 29.0,
                 L1Z = 25.0,
                 L2X = 0.0,
                 L2Y = 85.0,
                 L2Z = 22.0,
                 L3X = 0.0,
                 L3Y = 0.0,
                 L3Z = 121.0,
                 L0_1 = 85.0,
                 L0_2 = 64.0,
                 L0_3 = 85.0,
                 L0_4 = 85.0,
                 L0_5 = 64.0,
                 L0_6 = 85.0;

//miliseconds angle offsets
constexpr double L1_Q1_MS_OFFSET = -130.0,
                 L1_Q2_MS_OFFSET = -350.0,
                 L1_Q3_MS_OFFSET = -800.0,
                 L2_Q1_MS_OFFSET = -45.0,
                 L2_Q2_MS_OFFSET = -600.0,
                 L2_Q3_MS_OFFSET = -790.0,
                 L3_Q1_MS_OFFSET = -10.0,
                 L3_Q2_MS_OFFSET = -230.0,
                 L3_Q3_MS_OFFSET = -815.0,
                 L4_Q1_MS_OFFSET = 50.0,
                 L4_Q2_MS_OFFSET = 290.0,
                 L4_Q3_MS_OFFSET = 610.0,
                 L5_Q1_MS_OFFSET = 50.0,
                 L5_Q2_MS_OFFSET = 410.0,
                 L5_Q3_MS_OFFSET = 700.0,
                 L6_Q1_MS_OFFSET = -105.0,
                 L6_Q2_MS_OFFSET = 320.0,
                 L6_Q3_MS_OFFSET = 500.0;

//angle(degrees) offset SC0 on BASE
constexpr double SC0_1_DEG_OFFSET = 143.9,
                 SC0_2_DEG_OFFSET = 180.0,
                 SC0_3_DEG_OFFSET = -143.9,
                 SC0_4_DEG_OFFSET = 36.1,
                 SC0_5_DEG_OFFSET = 0.0,
                 SC0_6_DEG_OFFSET = -36.1;

//degree offset for lenght from BASE to SC0
constexpr double L0_1_DEG_OFFSET = 117.5,
                 L0_2_DEG_OFFSET = 180.0,
                 L0_3_DEG_OFFSET = -117.5,
                 L0_4_DEG_OFFSET = 62.5,
                 L0_5_DEG_OFFSET = 0.0,
                 L0_6_DEG_OFFSET = -62.5;

//custom types
namespace custom
{
    typedef std::array<std::array<double, AXIS>, AXIS> matrix;
    typedef std::array<double, AXIS> vector;
    typedef std::array<double, JOINTS> generalizedCoordinates;
    typedef std::array<int, JOINTS> generalizedMsCoordinates;
    typedef std::array<std::array<double, JOINTS>, LEGS> msOffsets;
    typedef std::array<double, LEGS> offsets;
    typedef std::vector<custom::vector> pointList;
} // namespace custom

// walking constants
constexpr int speed = 20;
constexpr double platformHeight = 50.0,
                 splineDistance = 10.0,
                 splineTolerance = 1E-3,
                 rotateOnTheSpotAngle = 5.0;

//states constants(vectors)
constexpr custom::vector zeroVector = {0.0, 0.0, 0.0},
                         nullPositionVector = {-3.0, 235.0, 0.0},
                         defaultPositionVector1 = {50.0, -30.0, 0.0},
                         defaultPositionVector2 = {50.0, 0.0, 0.0},
                         defaultPositionVector3 = {50.0, 30.0, 0.0},
                         defaultPositionVector4 = {-50.0, -30.0, 0.0},
                         defaultPositionVector5 = {-50.0, 0.0, 0.0},
                         defaultPositionVector6 = {-50.0, 30.0, 0.0},
                         defaultStandUpVector = {0.0, 0.0, -platformHeight},
                         defaultSitDownVector = {0.0, 0.0, platformHeight};

#endif // CONSTANTS_HPP
