//cSpline.hpp

#ifndef SPLINE_HPP
#define SPLINE_HPP

#include "constants.hpp"

class cSpline
{
private:
    const double m_distance = splineDistance;
    const double m_tolerance = splineTolerance;
    custom::pointList m_controlPoints;
    custom::pointList m_splineList;
private:
    double findCoordinate(const double & t, const double & p0, const double & p1,
                                            const double & p2, const double & p3);
    custom::vector findPoint(const double & t, const custom::vector & p0, const custom::vector & p1,
                                               const custom::vector & p2, const custom::vector & p3);
public:
    explicit cSpline(const custom::pointList & controlPoints);
    ~cSpline() {};

    const custom::pointList & getList()const {return m_splineList;}
};

#endif // SPLINE_HPP
