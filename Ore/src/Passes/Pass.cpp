#include "Passes/Pass.h"

Pass::Pass(Program const& program, sptr<Camera> pCamera)
{
    m_pProgram = std::make_shared<Program>(program);
    m_pCamera = pCamera;
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


