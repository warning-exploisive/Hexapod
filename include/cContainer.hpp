//cContainer.hpp

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <memory>
#include "cWidget.hpp"

class cContainer : public cWidget
{
private:
    std::vector<std::unique_ptr<cWidget>> m_widgets;
    sf::RectangleShape m_shape;
private:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const override;
public:
    cContainer();
    virtual ~cContainer() {};

    void setSize(const sf::Vector2f & size);
    void setFillColor(const sf::Color & color);
    virtual sf::Vector2f getSize()const override;

    virtual void processEvent(const sf::Event & event, const sf::Vector2f & framePosition)override;
    virtual void updateShape()override {};

    void addWidget(std::unique_ptr<cWidget> widget);
    void clearContainer();
};

#endif // CONTAINER_HPP
