#ifndef ORE_RENDER_GRAPH__H_
#define ORE_RENDER_GRAPH__H_

#include "IRenderGraph.h"
#include "TextureObject.h"

namespace Ore
{
class Viewport;
class Pass;
class RenderGraph : public IRenderGraph
{
public:
    RenderGraph(Viewport& viewport);
    ~RenderGraph() = default;

    void AddPass(Pass* pPass);
    void Execute() override;
    void UpdateGBuffer();

protected:
    void CreateGBuffer(uint32 screenWidth, uint32 screenHeight) override;
    void End();

private:
    uint32 m_gBuffer;
    uint32 m_rboDepth;

    std::vector<Pass*> m_passes;

    //Circular depency TODO Find a better way
    Viewport& m_viewport;

    sptr<TextureObject> m_pGPosition;
    sptr<TextureObject> m_pGNormal;
    sptr<TextureObject> m_pGAlbedoSpec;
};
}
#endif
