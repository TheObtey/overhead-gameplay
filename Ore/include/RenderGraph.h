#ifndef ORE_RENDER_GRAPH__H_
#define ORE_RENDER_GRAPH__H_

#include "IRenderGraph.h"

class Pass;
class RenderGraph : public IRenderGraph
{
public:
    RenderGraph(uint32 screenWidth, uint32 screenHeight);
    ~RenderGraph() override; 

    void AddPass(Pass& pass);
    void Execute() override;

protected:
    void CreateGBuffer(uint32 screenWidth, uint32 screenHeight) override;

private:
    std::vector<uptr<IPass>> m_passes;
    uint32 m_gBuffer;

};
#endif
