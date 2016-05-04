//cPoint.cpp

#include "cPoint.hpp"
#include "cGui.hpp"

#include <iostream>

cPoint::cPoint():
m_status(eStatus::None)
{
    m_position = {- circleRadius, - circleRadius};
    m_circle.setRadius(circleRadius);
    m_circle.setFillColor(blackColor);
    m_outlineCircle.setRadius(circleRadius + 2.f);
    m_outlineCircle.setFillColor(whiteColor);
    updateShape();
}

cPoint::cPoint(const sf::Vector2f & position):
m_status(eStatus::None)
{
    m_position.x = position.x - circleRadius;
    m_position.y = position.y - circleRadius;
    m_circle.setRadius(circleRadius);
    m_circle.setFillColor(blackColor);
    m_outlineCircle.setRadius(circleRadius + 2.f);
    m_outlineCircle.setFillColor(whiteColor);
    updateShape();
}

cPoint::cPoint(const sf::Vector2f & position, const float & radius):
m_status(eStatus::None)
{
    m_position.x = position.x - radius;
    m_position.y = position.y - radius;
    m_circle.setRadius(radius);
    m_circle.setFillColor(blackColor);
    m_outlineCircle.setRadius(radius + 2.f);
    m_outlineCircle.setFillColor(whiteColor);
    updateShape();
}

void cPoint::setPointColor(const sf::Color & color)
{
    m_circle.setFillColor(color);
}

void cPoint::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
    if(m_status == eStatus::Hover)
    {
        target.draw(m_outlineCircle, states);
        target.draw(m_circle, states);
    }
    else
    {
        target.draw(m_circle, states);
    }
}

sf::Vector2f cPoint::getSize()const
{
    return sf::Vector2f(m_circle.getRadius() * 2.f, m_circle.getRadius() * 2.f);
}

void cPoint::processEvent(const sf::Event & event, const sf::Vector2f & containerPosition)
{
    if (event.type == sf::Event::MouseMoved)
    {
        const sf::Vector2f pos = m_position + containerPosition;
        const sf::Vector2f size = getSize();
        sf::FloatRect rect;
        rect.left = pos.x;
        rect.top = pos.y;
        rect.width = size.x;
        rect.height = size.y;
        m_status = eStatus::None;
        const sf::Vector2f mousePosition(event.mouseMove.x,event.mouseMove.y);
        if(rect.contains(mousePosition))
            m_status = eStatus::Hover;
    }
}

void cPoint::updateShape()
{
    m_circle.setPosition(m_position.x, m_position.y);
    m_outlineCircle.setPosition(m_position.x - 2.f, m_position.y - 2.f);
}
