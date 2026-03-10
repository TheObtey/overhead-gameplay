#ifndef RHI_IWINDOW__H_
#define RHI_IWINDOW__H_

#include "Event.hpp"
#include "Define.h"

#include <string>

class IWindow 
{
public:

    virtual ~IWindow() = 0;
    
    virtual void Close() = 0;
    virtual bool IsOpen() = 0; 
    virtual void Present() = 0; 

    virtual void SetDecoration(bool hasDecoration) = 0;
    virtual void SetSize(uint16 width, uint16 height) = 0;
   
    Event<void()> onOpenEvent;
    Event<void()> onCloseEvent;
    Event<void()> onResizeEvent;

protected:
    virtual void Open() = 0;

protected:
    int m_width = 0;
    int m_height = 0;
    std::string m_name = "";
};

inline IWindow::~IWindow() {}

#endif //!RHI_IWINDOW__H_ 
