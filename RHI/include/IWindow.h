#ifndef RHI_IWINDOW__H_
#define RHI_IWINDOW__H_

#include "Event.hpp"

#include <string>

class IWindow 
{
public:
    IWindow(int width, int height, std::string name);
    ~IWindow();
    
    virtual void Close() = 0;
    Event<void()> onOpenEvent;
    Event<void()> onCloseEvent;

protected:
    virtual void Open() = 0;

protected:
    int m_width;
    int m_height;
};
#endif //!RHI_IWINDOW__H_
