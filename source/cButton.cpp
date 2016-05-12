//cButton.cpp

#include "cButton.hpp"

cButton::cButton():
m_status(eStatus::None),
m_state(eState::Enabled),
onPressed(defaultFunc),
onReleased(defaultFunc)
{}

cButton::FuncType cButton::defaultFunc = [](const sf::Event &, cButton &)->void{};

void cButton::processEvent(const sf::Event & event, const sf::Vector2f & containerPosition)
{
    if(event.type == sf::Event::MouseButtonPressed)
    {
        const sf::Vector2f pos = m_position + containerPosition;
        const sf::Vector2f size = getSize();
        sf::FloatRect rect;
        rect.left = pos.x;
        rect.top = pos.y;
        rect.width = size.x;
        rect.height = size.y;
        if(rect.contains(event.mouseButton.x,event.mouseButton.y))
        {
            if (m_state == eState::Enabled)
            {
                m_state = eState::Pressed;
                onPressed(event, *this);
            }
            else if (m_state == eState::Pressed)
            {
                m_state = eState::Enabled;
                onReleased(event, *this);
            }
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        const sf::Vector2f pos = m_position + containerPosition;
        const sf::Vector2f size = getSize();
        sf::FloatRect rect;
        rect.left = pos.x;
        rect.top = pos.y;
        rect.width = size.x;
        rect.height = size.y;
        int old_status = m_status;
        m_status = eStatus::None;
        const sf::Vector2f mousePosition(event.mouseMove.x,event.mouseMove.y);
        if(rect.contains(mousePosition))
            m_status = eStatus::Hover;
        if((old_status & eStatus::Hover) and not (m_status & eStatus::Hover))
            onMouseLeft();
        else if(not (old_status & eStatus::Hover) and (m_status & eStatus::Hover))
            onMouseEntered();
    }
 }

void cButton::setDisabled()
{
    m_state = eState::Disabled;
}
void cButton::setEnabled()
{
    m_state = eState::Enabled;
}

