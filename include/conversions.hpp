//conversions.hpp

#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

#define _USE_MATH_DEFINES
#include <cmath>

#define ZERO_DEG_IN_MS 1500.0
#define ONE_DEG_IN_MS 1000.0 / 90.0
#define HALF_CIRCLE 180.0
#define ZERO 0.0

//перевод из градусов в милисекунды ШИМ
inline int degToMiliseconds(double degAngle) {return (int)(ZERO_DEG_IN_MS + degAngle * ONE_DEG_IN_MS);}
//перевод из радиан в градусы
inline double radToDeg(double radAngle) {return radAngle * HALF_CIRCLE / M_PI;}
//перевод из градусов в радианы
inline double degToRad(double degAngle) {return degAngle * M_PI / HALF_CIRCLE;}
//возвращает знак числа
inline int sign(double number) {return (number > ZERO) - (number < ZERO);}

#endif // CONVERSIONS_HPP
