#ifndef ORE_PASS__H_
#define ORE_PASS__H_

#include "IPass.h"
#include "Shader.h"
#include "TextureObject.h"
#include "Camera.h"

class Pass : public IPass
{
public:
    Pass(Shader const& shader, Camera const& camera);
    Pass(Pass& other) {m_pShader = other.m_pShader;}
    ~Pass() override;

protected:
    virtual void Execute() override {};
    virtual void SetGBuffer(uint32 gbuffer) override {m_gBuffer = gbuffer;}
    virtual void SetTextures(sptr<TextureObject> gPosition, sptr<TextureObject> gNormal, sptr<TextureObject> gAlbedoSpec);

protected:
    sptr<Shader> m_pShader;
    sptr<Camera> m_pCamera;
    uint32 m_gBuffer;
    sptr<TextureObject> m_pGPosition;
    sptr<TextureObject> m_pGNormal;
    sptr<TextureObject> m_pGAlbedoSpec;

friend class RenderGraph;
};

#endif //!RHI_IPASS__H_
