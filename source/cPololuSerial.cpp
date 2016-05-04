//cPololuSerial.cpp - implementation of cPololuSerial class

#include <boost/asio/serial_port.hpp>
#include <boost/asio.hpp>
#include <assert.h>
#include <unistd.h>
#include "cPololuSerial.hpp"

cPololuSerial::cPololuSerial():
m_port_ptr(nullptr)
{
    m_io_ptr = std::move(std::unique_ptr<boost::asio::io_service>(new boost::asio::io_service));
    m_port_ptr = std::move(std::unique_ptr<boost::asio::serial_port>(new boost::asio::serial_port(*m_io_ptr)));
    m_port_ptr->open(m_device);
    if (m_port_ptr == nullptr)
    {
        assert(m_port_ptr);
        m_port_ptr.get()->close();
    }
    m_port_ptr->set_option(boost::asio::serial_port_base::baud_rate(115200));
    m_port_ptr->set_option(boost::asio::serial_port_base::character_size(8));
    m_port_ptr->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    m_port_ptr->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    m_port_ptr->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
}

cPololuSerial::~cPololuSerial()
{
    m_port_ptr.get()->close();
}

void cPololuSerial::setTarget(unsigned char channel, int target)
{
    unsigned char protocol[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
    boost::asio::write(*m_port_ptr, boost::asio::buffer(protocol, sizeof(protocol)));
    usleep(1000);
}

void cPololuSerial::shutDown(unsigned char channel)
{
    unsigned char protocol[] = {0x84, channel, 0x00, 0x00};
    boost::asio::write(*m_port_ptr, boost::asio::buffer(protocol, sizeof(protocol)));
    usleep(1000);
}

void cPololuSerial::setSpeed(unsigned char channel, int speed)
{
    unsigned char protocol[] = {0x87, channel, speed & 0x7F, speed >> 7 & 0x7F};
    boost::asio::write(*m_port_ptr, boost::asio::buffer(protocol, sizeof(protocol)));
    usleep(1000);
}

int cPololuSerial::getPosition(unsigned char channel)
{
    unsigned char protocol[] = {0x90, channel};
    boost::asio::write(*m_port_ptr, boost::asio::buffer(protocol, sizeof(protocol)));
    unsigned char response[2];
    assert(boost::asio::read(*m_port_ptr,boost::asio::buffer(response, 2)) == 2);
    return response[0] + 256 * response[1];
}

int cPololuSerial::isMoving()
{
    unsigned char protocol[] = {0x93};
    boost::asio::write(*m_port_ptr, boost::asio::buffer(protocol, sizeof(protocol)));
    unsigned char response[1];
    assert(boost::asio::read(*m_port_ptr,boost::asio::buffer(response, 1)));
    return response[0];
}
