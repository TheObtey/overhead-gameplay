#ifndef RHI_IVIEWPORT__H_
#define RHI_IVIEWPORT__H_

#include "IRenderGraph.h"
#include "IWindow.h"
#include "Color.h"

class IViewport
{
public:
    virtual ~IViewport() = 0;    

    virtual void SetSize(uint16 width, uint16 height) = 0;
    virtual void SetPos(uint16 x, uint16 y) = 0;
    virtual void SetBackgroundColor(Color const& color) = 0;

    virtual uint16 GetWidth() = 0; 
    virtual uint16 GetHeight() = 0;

    virtual void Clear() = 0;
    virtual void Present() = 0;

protected:
    virtual void Display() = 0;

protected:
    uint16 m_width = 0;
    uint16 m_height = 0;
    uint16 m_x = 0;
    uint16 m_y = 0;
    Color m_backgroundColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
};

inline IViewport::~IViewport() {}

#endif //!RHI_IVIEWPORT__H_
