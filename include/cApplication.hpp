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
    custom::pointList m_controlVertices;

private:
    void processEvents();
    void update();
    void render();

    void initGui();
    void initSplineView(cContainer * viewContainer);

    // tactic level

    void addCommands_setTarget();
    void addOneLegCommand_setTarget(const unsigned char & channel, const unsigned char & leg);
    void addShutDownCommand();
    void addCommands_setSpeed(const int & speed);
    void walkSequence(const double & delta_courseAngle_1, const double & delta_courseAngle_2,
                      const double & delta_courseAngle_3, const double & delta_lenght_1,
                      const double & delta_lenght_2, const double & delta_lenght_3, const int & leg);
    void step(const double & delta_courseAngle_1, const double & delta_courseAngle_2,
              const double & delta_courseAngle_3, const double & delta_lenght_1,
              const double & delta_lenght_2, const double & delta_lenght_3, const int & leg);
    void rotateOnTheSpot(const int & counter);
    void walk();

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
