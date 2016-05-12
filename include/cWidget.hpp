//cWidget.hpp

#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "SFML/Graphics.hpp"

class cWidget : public sf::Drawable
{
private:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const = 0;
protected:
    sf::Vector2f m_position;
    sf::Font m_font;
public:
    cWidget();
    virtual ~cWidget() {};

    void setPosition(const sf::Vector2f & position);
    void setPosition(const float & x, const float & y);
    const sf::Vector2f & getPosition()const;
    virtual sf::Vector2f getSize()const = 0;

    virtual void processEvent(const sf::Event & event, const sf::Vector2f & containerPosition) = 0;
    virtual void updateShape() = 0;
};

#endif // WIDGET_HPP
