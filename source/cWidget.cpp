//cWidget.cpp

#include "cWidget.hpp"

cWidget::cWidget()
{
    m_font.loadFromFile("/home/rezak/projects/hexapod/source/Ubuntu-L.ttf");
}

void cWidget::setPosition(const sf::Vector2f & position)
{
    m_position = position;
}

const sf::Vector2f & cWidget::getPosition()const
{
    return m_position;
}

void cWidget::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}
