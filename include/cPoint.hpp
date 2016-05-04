//cPoint.hpp

#ifndef POINT_HPP
#define POINT_HPP

#include "cWidget.hpp"

class cPoint : public cWidget
{
private:
    enum eStatus {None = 0, Hover};
    int m_status;
    sf::CircleShape m_circle;
    sf::CircleShape m_outlineCircle;
private:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const;
public:
    cPoint();
    explicit cPoint(const sf::Vector2f & position);
    cPoint(const sf::Vector2f & position, const float & radius);
    virtual ~cPoint() {};

    void setPointColor(const sf::Color & color);

    virtual sf::Vector2f getSize()const override;
    virtual void processEvent(const sf::Event & event, const sf::Vector2f & containerPosition)override;
    virtual void updateShape()override;
};

#endif // POINT_HPP
