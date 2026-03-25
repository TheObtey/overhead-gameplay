#ifndef ORE_PASS__H_
#define ORE_PASS__H_

#include "IPass.h"
#include "Program.h"
#include "Camera.h"

class TextureObject;
class Pass : public IPass
{
public:
    Pass(Program const& shader, sptr<Camera> camera);
    Pass(Pass& other) { m_pProgram = other.m_pProgram; }
    ~Pass() override;

private:
    virtual void Execute() override {};
    virtual void SetGBuffer(uint32 gbuffer) override {m_gBuffer = gbuffer;}
    virtual void SetTextures(sptr<TextureObject> gPosition, sptr<TextureObject> gNormal, sptr<TextureObject> gAlbedoSpec);
    virtual void SetScreenSize(uint32 width, uint32 height) {m_screenWidth = width, m_screenHeight = height;}

protected:

    uint32 m_gBuffer;
    uint32 m_screenWidth;
    uint32 m_screenHeight;

    sptr<Program> m_pProgram;
    sptr<Camera> m_pCamera;

    sptr<TextureObject> m_pGPosition;
    sptr<TextureObject> m_pGNormal;
    sptr<TextureObject> m_pGAlbedoSpec;

friend class RenderGraph;
};

#endif //!RHI_IPASS__H_
