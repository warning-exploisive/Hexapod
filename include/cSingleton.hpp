//cSingleton.hpp
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

class cSingleton
{
private:
    cSingleton(const cSingleton &) = delete;
    cSingleton & operator=(const cSingleton &) = delete;
protected:
    cSingleton() {};
    virtual ~cSingleton() {};
public:
    static cSingleton & getInstance()
    {
        static cSingleton instance;
        return instance;
    }
};

#endif // SINGLETON_HPP
