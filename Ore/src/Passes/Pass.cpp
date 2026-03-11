#include "Passes/Pass.h"

Pass::Pass(Shader& shader)
{
    m_pShader = std::make_shared<Shader>(shader);
}


