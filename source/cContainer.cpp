//cContainer.cpp

#include "cContainer.hpp"
#include "cGui.hpp"

cContainer::cContainer()
{
    setFillColor(containerColor);
}

void cContainer::processEvent(const sf::Event & event, const sf::Vector2f & framePosition)
{
    for (auto & widget : m_widgets)
        widget->processEvent(event, framePosition + m_position);
}

void cContainer::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
    states.transform.translate(m_position);
    target.draw(m_shape, states);
    for (auto & widget : m_widgets)
        target.draw(*widget, states);
}
void cContainer::setSize(const sf::Vector2f & size)
{
    m_shape.setSize(size);
}

void cContainer::setFillColor(const sf::Color & color)
{
    m_shape.setFillColor(color);
}

sf::Vector2f cContainer::getSize()const
{
    sf::FloatRect rectangle = m_shape.getGlobalBounds();
    return sf::Vector2f(rectangle.width, rectangle.height);
}

void cContainer::addWidget(std::unique_ptr<cWidget> widget)
{
    m_widgets.emplace_back(std::move(widget));
}

void cContainer::clearContainer()
{
    m_widgets.clear();
}
