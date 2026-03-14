#include "Passes/Pass.h"

Pass::Pass(Shader const& shader)
{
    m_pShader = std::make_shared<Shader>(shader);
}

Pass::~Pass()
{
}


