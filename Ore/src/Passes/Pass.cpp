#include "Passes/Pass.h"

Pass::Pass(Shader const& shader, Camera const& camera)
{
    m_pShader = std::make_shared<Shader>(shader);
    m_pCamera = std::make_shared<Camera>(camera);
}

Pass::~Pass()
{
}

void Pass::SetTextures(sptr<TextureObject> gPosition, sptr<TextureObject> gNormal, sptr<TextureObject> gAlbedoSpec)
{
    m_pGPosition = gPosition;
    m_pGNormal = gNormal;
    m_pGAlbedoSpec = gAlbedoSpec;
}


