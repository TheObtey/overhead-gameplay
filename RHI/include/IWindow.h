#ifndef RHI_IWINDOW__H_
#define RHI_IWINDOW__H_

#include "Event.hpp"
#include "Define.h"

#include <string>

namespace Ore
{
class IWindow 
{
public:
    virtual void Close() = 0;
    virtual bool IsOpen() = 0; 
    virtual void Clear() = 0;
    virtual void Present() = 0; 

    virtual void SetDecoration(bool hasDecoration) = 0;
    virtual void SetSize(uint16 width, uint16 height) = 0;
    virtual void SetIcon(std::string const& path) = 0;
   
    Event<void()> onOpenEvent;
    Event<void()> onCloseEvent;
    Event<void(uint32, uint32)> onResizeEvent;
    virtual void Open() = 0;

protected:
    uint32 m_width = 0;
    uint32 m_height = 0;
    std::string m_name = "";
};
}
#endif //!RHI_IWINDOW__H_ 
