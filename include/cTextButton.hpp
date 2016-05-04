//cTextButton.hpp

#ifndef TEXTBUTTON_HPP
#define TEXTBUTTON_HPP

#include "cButton.hpp"
#include "cLabel.hpp"

class cTextButton : public cButton
{
    private:
        sf::RectangleShape m_shape;
        cLabel m_label;
        sf::Color m_fillColor;
    private:
        virtual void draw(sf::RenderTarget & target, sf::RenderStates states)const override;
        virtual void onMouseEntered()override;
        virtual void onMouseLeft()override;
    public:
        cTextButton(const std::string & text);
        virtual ~cTextButton() {};

        void setText(const std::string & text);
        void setFontSize(unsigned int fontSize);
        void setTextColor(const sf::Color & color);
        void setFillColor(const sf::Color & color);
        virtual sf::Vector2f getSize()const override;

        virtual void updateShape()override;
        virtual void setDisabled()override;
        virtual void setEnabled()override;
};

#endif // TEXTBUTTON_HPP
