#include "Passes/Pass.h"
#include <glad/glad.h>

using namespace Ore;
Pass::Pass(Program& program) : m_program(program) {}
Pass::Pass(Program& program, Camera *camera) : m_program(program), m_pCamera(camera) {}

void Pass::SetTextures(sptr<TextureObject> gPosition, sptr<TextureObject> gNormal, sptr<TextureObject> gAlbedoSpec)
{
    m_pGPosition = gPosition;
    m_pGNormal = gNormal;
    m_pGAlbedoSpec = gAlbedoSpec;
}


