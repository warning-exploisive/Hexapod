//cButton.hpp

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "cWidget.hpp"

class cButton : public cWidget
{
private:
    enum eStatus {None = 0, Hover};
    int m_status;
protected:
    enum eState {Enabled = 0, Disabled, Pressed};
    int m_state;
protected:
    virtual void onMouseEntered() {};
    virtual void onMouseLeft() {};
public:
    cButton();
    virtual ~cButton() {};

    using FuncType = std::function<void(const sf::Event & event, cButton & self)>;
    static FuncType defaultFunc;
    FuncType onPressed;
    FuncType onReleased;

    virtual void processEvent(const sf::Event & event, const sf::Vector2f & containerPosition)override;
    virtual void setDisabled();
    virtual void setEnabled();
};

#endif // BUTTON_HPP
