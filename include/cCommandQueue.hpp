//cCommandQueue.hpp

#ifndef COMMANDQUEUE_HPP
#define COMMANDQUEUE_HPP

#include <list>
#include "cSingleton.hpp"
#include "cPololuSerial.hpp"

class cCommandQueue : public cSingleton
{
private:
    cPololuSerial & m_serial;
    std::list<void(cPololuSerial::*)(unsigned char channel, int target)> m_commandList;
    std::list<unsigned char> m_channels;
    std::list<int> m_targets;
protected:
    explicit cCommandQueue(cPololuSerial & serial);
public:
    static cCommandQueue & getInstance(cPololuSerial & serial)
    {
        static cCommandQueue instance(serial);
        return instance;
    }
    ~cCommandQueue() {};

    void addCommand(void(cPololuSerial::*command)(unsigned char channel, int target),
                    unsigned char channel, int target);
    void runCommand();
    void clearQueue();
    bool IsEmpty();
};

#endif // COMMANDQUEUE_HPP
