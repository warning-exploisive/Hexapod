//cFrame.cpp

#include "cFrame.hpp"
#include "cGui.hpp"
#include <iterator>

cFrame::cFrame(const sf::Vector2u & windowSize)
{
    m_shape.setSize(sf::Vector2f((float)windowSize.x, (float)windowSize.y));
    m_shape.setFillColor(clearColor);
};

void cFrame::draw(sf::RenderTarget & target, sf::RenderStates states)const
{
    target.draw(m_shape, states);
    for(auto & container : m_containers)
        target.draw(*container, states);
}

void cFrame::processEvent(const sf::Event & event)
{
    sf::Vector2f position {0.f, 0.f};
    for(auto & container : m_containers)
        container->processEvent(event, position);
}

void cFrame::addContainer(std::unique_ptr<cContainer> container)
{
    m_containers.emplace_back(std::move(container));
}

void cFrame::clearContainer(cContainer * container_ptr)
{
    for (auto & container : m_containers)
        if(container.get() == container_ptr)
            container_ptr->clearContainer();
}
