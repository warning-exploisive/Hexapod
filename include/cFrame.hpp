//cFrame.hpp

#ifndef FRAME_HPP
#define FRAME_HPP

#include <memory>
#include "cContainer.hpp"
#include "cSingleton.hpp"

class cFrame : public sf::Drawable, public cSingleton
{
private:
    sf::RectangleShape m_shape;
    std::vector<std::unique_ptr<cContainer>> m_containers;
private:
    void draw(sf::RenderTarget & target, sf::RenderStates states)const;
protected:
    cFrame(const sf::Vector2u & windowSize);
public:
    ~cFrame() {};
    static cFrame & getInstance(const sf::Vector2u & windowSize)
    {
        static cFrame instance(windowSize);
        return instance;
    }

    void processEvent(const sf::Event & event);
    void addContainer(std::unique_ptr<cContainer> container);
    void clearContainer(cContainer * container_ptr);
};

#endif // FRAME_HPP
