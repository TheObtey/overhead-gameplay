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

    void AddPass(Pass* pPass);
    void Execute() override;
    void SetSize(uint32 width, uint32 height);

protected:
    void CreateGBuffer(uint32 screenWidth, uint32 screenHeight) override;

private:
    uint32 m_screenWidth;
    uint32 m_screenHeight;
    uint32 m_gBuffer;

    std::vector<Pass*> m_passes;

    sptr<TextureObject> m_pGPosition;
    sptr<TextureObject> m_pGNormal;
    sptr<TextureObject> m_pGAlbedoSpec;
};
#endif
