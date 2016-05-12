//cGui.hpp

#ifndef GUI_HPP
#define GUI_HPP

#include "SFML/Graphics.hpp"
#include "cFrame.hpp"
#include "cContainer.hpp"
#include "cTextButton.hpp"
#include "cLabel.hpp"
#include "cPoint.hpp"

//frame rate
const sf::Time m_timePerFrame = sf::seconds(1.f / 60.f);

//colors
const sf::Color clearColor = sf::Color(139, 137, 137),
                containerColor = sf::Color(211, 211, 211),
                disabledColor = sf::Color(190, 190, 190),
                redColor = sf::Color(220, 0, 0),
                greenColor = sf::Color(50, 205, 50),
                blackColor = sf::Color::Black,
                whiteColor = sf::Color::White,
                pressedColor = sf::Color::Green,
                releasedColor = sf::Color::Blue;
//widgets position
const sf::Vector2f controlContainerPosition {10.f, 20.f},
                   controlContainerSize {170.f, 440.f},
                   viewContainerPosition {190.f, 20.f},
                   viewContainerSize {440.f, 440.f},
                   ONPosition = {10.f, 10.f},
                   OFFPosition = {58.5f, 10.f},
                   StartPosition = {10.f, 100.f},
                   StopPosition = {87.5f, 100.f};

//spline parameters
constexpr float circleRadius = 10.f;

//Text parameters
constexpr int defaultFontSize = 20;

#endif // GUI_HPP
