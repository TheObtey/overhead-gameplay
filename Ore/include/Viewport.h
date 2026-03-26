#ifndef ORE_VIEWPORT__H_
#define ORE_VIEWPORT__H_

#include "IViewport.h"
#include "Passes/Pass.h"
#include "RenderGraph.h"

class Window;
class Pass;
class Viewport : public IViewport
{
public:
    Viewport() = default;
    Viewport(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor);
    ~Viewport() override = default;

    void Setup(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor);
    void Setup(glm::uvec2 const& pos, glm::uvec2 const& size, Color const& backgroundColor);

    void SetSize(uint16 width, uint16 height) override;
    void SetPos(uint16 x, uint16 y) override {m_x = x, m_y = y;}
    void SetBackgroundColor(Color const& color) override {m_backgroundColor = color;}

    uint16 GetWidth() override {return m_width;} 
    uint16 GetHeight() override  {return m_height;}

    void Clear() const override;
    void Present() const override;
    
    void AddPass(Pass* pPass);

private:
    uptr<RenderGraph> m_pRenderGraph;
};

#endif
