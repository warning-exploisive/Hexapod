//cSpline.cpp

#include "cSpline.hpp"
#include "matrix.hpp"

cSpline::cSpline(const custom::pointList & controlPoints):
m_controlPoints(controlPoints)
{
    custom::vector p0, p1, p2, p3;
    for(unsigned int i = 0; i < m_controlPoints.size() - 1; i += 3)
    {
        p0 = m_controlPoints[i];
        if (m_controlPoints.size() - 1 - i >= 3)
        {
            p1 = m_controlPoints[i + 1];
            p2 = m_controlPoints[i + 2];
            p3 = m_controlPoints[i + 3];
        }
        else if (m_controlPoints.size() - 1 - i == 2)
        {
            p1 = m_controlPoints[i + 1];
            p2 = m_controlPoints[i + 2];
            p3 = m_controlPoints[i + 2];
        }
        else if (m_controlPoints.size() - 1 - i == 1)
        {
            p1 = m_controlPoints[i + 1];
            p2 = m_controlPoints[i + 1];
            p3 = m_controlPoints[i + 1];
        }

        int stateFlag = 0;
        double t = 0.0;
        double d_t = 0.001f;
        double absDistance, preAbsDistance;
        custom::vector currentPoint, nextPoint, tempPoint;
        //check bezier path
        if (m_splineList.empty())
            currentPoint = p0;
        else
            currentPoint = m_splineList[m_splineList.size() - 1];
        if (m_splineList.empty())
            m_splineList.push_back(currentPoint);
        else if (findAbsValue(currentPoint - m_splineList[m_splineList.size() - 1]) > m_distance / 3.0)
            m_splineList.push_back(currentPoint);
        t += d_t;
        //build bezier path
        while (t < 1.f)
        {
            nextPoint = findPoint(t, p0, p1, p2, p3);
            absDistance = findAbsValue(nextPoint - currentPoint);
            if (absDistance <= m_distance + m_tolerance && absDistance >= m_distance - m_tolerance)
            {
                currentPoint = nextPoint;
                m_splineList.push_back(currentPoint);
                t += d_t;
            }
            else if (absDistance >= m_distance + m_tolerance) //larger
            {
                if (stateFlag != -1)//was no less
                {
                    stateFlag = 1;
                    t -= d_t;
                }
                else//was less
                {
                    t -= d_t;
                    tempPoint = findPoint(t, p0, p1, p2, p3);
                    preAbsDistance = findAbsValue(nextPoint - tempPoint);
                    double propotion = 1.0 - (absDistance - m_distance) / preAbsDistance;
                    d_t *= propotion;
                    t += d_t;
                }
            }
            else if (absDistance <= m_distance - m_tolerance) //less
            {
                if (stateFlag != 1)//was no larger
                {
                    stateFlag = -1;
                    t += d_t;
                }
                else//was larger
                {
                    t += d_t;
                    tempPoint = findPoint(t, p0, p1, p2, p3);
                    preAbsDistance = findAbsValue(nextPoint - tempPoint);
                    double propotion = (m_distance - absDistance) / preAbsDistance;
                    t -= d_t;
                    d_t *= propotion;
                    t += d_t;
                }
            }
        }
    }
    //check finish point for abs distanse
    if (findAbsValue(controlPoints[controlPoints.size() - 1] - m_splineList[m_splineList.size() - 1]) > m_tolerance)
        m_splineList.push_back(controlPoints[controlPoints.size() - 1]);
}

double cSpline::findCoordinate(const double & t, const double & p0, const double & p1,
                                                 const double & p2, const double & p3)
{
    return (1.0 - t) * (1.0 - t) * (1.0 - t) * p0 +
            3.0 * t * (1.0 - t) * (1.0 -t) * p1 +
            3.0 * t * t * (1.0 - t) * p2 + t * t * t * p3;
}

custom::vector cSpline::findPoint(const double & t, const custom::vector & p0, const custom::vector & p1,
                                                    const custom::vector & p2, const custom::vector & p3)
{
    custom::vector vector_out = {0};
    for(int inner = 0; inner < AXIS; ++inner)
        vector_out[inner] = findCoordinate(t, p0[inner], p1[inner], p2[inner], p3[inner]);
    return vector_out;
}
