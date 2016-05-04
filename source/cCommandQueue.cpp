//cCommandQueue.cpp

#include "constants.hpp"
#include "cCommandQueue.hpp"

cCommandQueue::cCommandQueue(cPololuSerial & serial):
m_serial(serial)
{};

void cCommandQueue::addCommand(void(cPololuSerial::*command)(unsigned char channel, int target),
                               unsigned char channel, int target)
{
    m_commandList.push_back(command);
    m_channels.push_back(channel);
    m_targets.push_back(target);
}

void cCommandQueue::runCommand()
{
    for(int counter = 0; counter < LEGS * JOINTS; ++counter)
    {
        void(cPololuSerial::*command)(unsigned char channel, int target) = m_commandList.front();
        (m_serial.*command)(m_channels.front(), m_targets.front());
        m_commandList.pop_front();
        m_channels.pop_front();
        m_targets.pop_front();
    }
}

void cCommandQueue::clearQueue()
{
    m_commandList.clear();
    m_channels.clear();
    m_targets.clear();
}

bool cCommandQueue::IsEmpty()
{
    if(m_commandList.empty() && m_channels.empty() && m_targets.empty())
        return true;
    return false;
}
