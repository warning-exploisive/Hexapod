//cApplication.hpp

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "cSingleton.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include "cSpiderRobotState.hpp"
#include "cPololuSerial.hpp"
#include "cSpline.hpp"
#include "cCommandQueue.hpp"

#include "cGui.hpp"

class cApplication : public cSingleton
{
private:
    sf::RenderWindow m_window;
    cFrame & m_mainMenu;

    enum eStatus {None = 0, On, Off};
    unsigned int m_status = eStatus::None;

    cPololuSerial & m_serial;
    cCommandQueue & m_commandQueue;
    cSpiderRobotState & m_spider;
    std::unique_ptr<cSpline> m_spline;

    const int m_speed = 10;
private:
    void processEvents();
    void update();
    void render();

    void initGui();
    void initSplineView(cContainer * viewContainer);
protected:
    cApplication();
public:
    ~cApplication() {};
    static cApplication & getInstance()
    {
        static cApplication instance;
        return instance;
    }
    void run();
};

#endif // APPLICATION_HPP
