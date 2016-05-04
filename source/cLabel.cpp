//cLabel.cpp

#include "cLabel.hpp"

cLabel::cLabel(const std::string & text)
{
    m_text.setFont(m_font);
    setText(text);
    setFontSize(20);
    setTextColor(sf::Color::Black);
}

void cLabel::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    states.transform.translate(m_position);
    target.draw(m_text, states);
}

void cLabel::setText(const std::string & text)
{
    m_text.setString(text);
}

void cLabel::setFontSize(unsigned int chSize)
{
    m_text.setCharacterSize(chSize);
}

void cLabel::setTextColor(const sf::Color & textColor)
{
    m_text.setColor(textColor);
}

unsigned int cLabel::getFontSize()const
{
    return m_text.getCharacterSize();
}

sf::Vector2f cLabel::getSize()const
{
    sf::FloatRect rectangle = m_text.getGlobalBounds();
    return sf::Vector2f(rectangle.width, rectangle.height);
}

