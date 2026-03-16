#ifndef ORE_RENDER_GRAPH__H_
#define ORE_RENDER_GRAPH__H_

#include "IRenderGraph.h"
#include "TextureObject.h"

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
    sptr<TextureObject> m_pGPosition;
    sptr<TextureObject> m_pGNormal;
    sptr<TextureObject> m_pGAlbedoSpec;

};
#endif
