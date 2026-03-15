#ifndef ORE_VIEWPORT__H_
#define ORE_VIEWPORT__H_

#include "IViewport.h"
#include "RenderGraph.h"

class Window;
class Pass;
class Viewport : public IViewport
{
public:
    Viewport(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor, Window& Window);
    ~Viewport() override;
    
    void SetSize(uint16 width, uint16 height) override;
    void SetPos(uint16 x, uint16 y) override {m_x = x, m_y = y;}
    void SetBackgroundColor(Color const& color) override {m_backgroundColor = color;}

    uint16 GetWidth() override {return m_width;} 
    uint16 GetHeight() override  {return m_height;}

    void Clear() override;
    void Present() override;

    void AddPass(Pass& pass);

protected:
    void Display() override; 

private:
    sptr<Window> m_pWindow;
    uptr<RenderGraph> m_pRenderGraph;
};

#endif
