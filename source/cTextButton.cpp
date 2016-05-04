//cTextButton.cpp

#include "cTextButton.hpp"
#include "cGui.hpp"

cTextButton::cTextButton(const std::string & text):
m_label(text)
{
    m_shape.setFillColor(disabledColor);
    updateShape();
}

void cTextButton::updateShape()
{
    sf::Vector2f labelSize = m_label.getSize();
    unsigned int fontSize = m_label.getFontSize();
    m_shape.setSize(sf::Vector2f(labelSize.x + fontSize, labelSize.y + fontSize));
    m_label.setPosition(fontSize / 2.f, labelSize.y - fontSize / 2.f);
}

void cTextButton::setDisabled()
{
    m_state = eState::Disabled;
    m_shape.setFillColor(disabledColor);
}

void cTextButton::setEnabled()
{
    m_state = eState::Enabled;
    m_shape.setFillColor(m_fillColor);
}

void cTextButton::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
    states.transform.translate(m_position);
    target.draw(m_shape, states);
    target.draw(m_label, states);
}

void cTextButton::onMouseEntered()
{
    m_label.setTextColor(whiteColor);
}
void cTextButton::onMouseLeft()
{
    m_label.setTextColor(blackColor);
}

void cTextButton::setText(const std::string & text)
{
    m_label.setText(text);
}

void cTextButton::setFontSize(unsigned int fontSize)
{
    m_label.setFontSize(fontSize);
}

void cTextButton::setTextColor(const sf::Color & color)
{
    m_label.setTextColor(color);
}

void cTextButton::setFillColor(const sf::Color & color)
{
    m_fillColor = color;
    m_shape.setFillColor(m_fillColor);
}

sf::Vector2f cTextButton::getSize()const
{
    sf::FloatRect rectangle = m_shape.getGlobalBounds();
    return sf::Vector2f(rectangle.width, rectangle.height);
}
