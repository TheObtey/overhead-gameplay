#include "Passes/GeometryPass.h"

#include <glad/glad.h>
GeometryPass::GeometryPass(Shader& shader, std::vector<sptr<Geometry>> const& geometries, uint32 gBuffer) : Pass(shader)
{
    m_geometries = geometries;
}

void GeometryPass::Execute()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_pShader->Use();
}
