#include "Passes/GeometryPass.h"
#include "Logger.hpp"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace Ore;
GeometryPass::GeometryPass(Program& program) : Pass(program) {}
GeometryPass::GeometryPass(Program& program, Camera* pCamera) : Pass(program, pCamera) {}

void GeometryPass::AddMesh(Mesh const& mesh)
{
    m_meshes.push_back(mesh);
}

void GeometryPass::Execute()
{
    if (m_pCamera == nullptr) return;

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program.Use();
    m_program.SetUniform("viewProj", m_pCamera->GetViewProjMatrix());
    //m_program.SetUniform("view", viewMatrix);

    for(uint32 i = 0; i<m_meshes.size(); ++i)
    {
        if(m_meshes[i].get().GetIsActive() == false)
            continue;

        m_program.SetUniform("model", glm::inverse(m_meshes[i].get().GetTransform()));
        m_meshes[i].get().Draw(m_program);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_meshes.clear();
}
