#ifndef ORE_PASS__H_
#define ORE_PASS__H_

#include "IPass.h"
#include "Program.h"
#include "Camera.h"

namespace Ore
{
class TextureObject;
class Pass : public IPass
{
public:
    Pass(Program& program);
    Pass(Program& program, Camera* camera);
    void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

private:
    virtual void Execute() override {};
    virtual void SetGBuffer(uint32 gbuffer) override {m_gBuffer = gbuffer;}
    virtual void SetTextures(sptr<TextureObject> gPosition, sptr<TextureObject> gNormal, sptr<TextureObject> gAlbedoSpec);
    virtual void SetScreenSize(uint32 width, uint32 height) {m_screenWidth = width, m_screenHeight = height;}

protected:

    uint32 m_gBuffer;
    uint32 m_screenWidth;
    uint32 m_screenHeight;

    Program& m_program;
    Camera* m_pCamera = nullptr;

    sptr<TextureObject> m_pGPosition;
    sptr<TextureObject> m_pGNormal;
    sptr<TextureObject> m_pGAlbedoSpec;

friend class RenderGraph;
};
}
#endif //!RHI_IPASS__H_
