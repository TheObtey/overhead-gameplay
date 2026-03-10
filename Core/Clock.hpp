#ifndef CLOCK__H_
#define CLOCK__H_

#include <chrono>

#include "Clock.hpp"

namespace ch = std::chrono;

template <typename T = double>
class Clock
{
public:
    Clock();
    ~Clock() = default;

    void Start();
    T Reset();
    T GetElapsed();

private:
    T m_elapsedTime = 0;
    ch::time_point<ch::system_clock> m_previousTime {};
};

template <typename T>
Clock<T>::Clock()
{
    Start();
}

template <typename T>
void Clock<T>::Start()
{
    m_previousTime = ch::system_clock::now();
}

template <typename T>
T Clock<T>::Reset()
{
    auto currentTime = ch::system_clock::now();
    auto elapsed = static_cast<ch::duration<T>>(currentTime - m_previousTime);
    m_elapsedTime = elapsed.count();

    m_previousTime = currentTime;
    return m_elapsedTime;
}

template <typename T>
T Clock<T>::GetElapsed()
{
    return m_elapsedTime;
}

#endif
