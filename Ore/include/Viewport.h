#ifndef ORE_VIEWPORT__H_
#define ORE_VIEWPORT__H_

#include "IViewport.h"
#include "Passes/Pass.h"

class Window;
class Viewport : public IViewport
{
public:
    Viewport(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor);
    ~Viewport() override;
    
    void SetSize(uint16 width, uint16 height) override;
    void SetPos(uint16 x, uint16 y) override {m_x = x, m_y = y;}
    void SetBackgroundColor(Color const& color) override {m_backgroundColor = color;}

    uint16 GetWidth() override {return m_width;} 
    uint16 GetHeight() override  {return m_height;}

    void Clear() const override;
    void Present() const override;
    
    void AddPass(sptr<Pass> pPass);
    void Setup() override; 

};

#endif
