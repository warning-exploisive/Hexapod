//cPololuSerial.hpp

#ifndef POLOLUSERIAL_HPP
#define POLOLUSERIAL_HPP

#include "boost/asio/serial_port.hpp"
#include "cSingleton.hpp"

class cPololuSerial : public cSingleton
{
private:
    std::unique_ptr<boost::asio::io_service> m_io_ptr;
    std::unique_ptr<boost::asio::serial_port> m_port_ptr;
    const char * m_device = "/dev/ttyACM0";
protected:
    cPololuSerial();
public:
    ~cPololuSerial();
    static cPololuSerial & getInstance()
    {
        static cPololuSerial instance;
        return instance;
    }

    void setTarget(unsigned char channel, int target);
    void shutDown(unsigned char channel);
    void setSpeed(unsigned char channel, int speed);
    int getPosition(unsigned char channel);
    int isMoving();
};

#endif // POLOLUSERIAL_HPP
