//cLabel.hpp

#ifndef LABEL_HPP
#define LABEL_HPP

#include "cWidget.hpp"

class cLabel : public cWidget
{
private:
    sf::Text m_text;
private:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const override;
public:
    cLabel(const std::string & text);
    virtual ~cLabel() {};

    void setText(const std::string & text);
    void setFontSize(unsigned int fontSize);
    void setTextColor(const sf::Color & textColor);
    unsigned int getFontSize()const;
    virtual sf::Vector2f getSize()const override;

    virtual void processEvent(const sf::Event & event, const sf::Vector2f & parentPosition)override {};
    virtual void updateShape()override {};
};

#endif // LABEL_HPP
