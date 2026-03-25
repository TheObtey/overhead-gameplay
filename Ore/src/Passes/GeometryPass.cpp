#include "Passes/GeometryPass.h"
#include "Logger.hpp"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

GeometryPass::GeometryPass(Program const& program, std::vector<Mesh*> const& meshes, sptr<Camera> pCamera) : Pass(program, pCamera)
{
    m_meshes = meshes;
}

GeometryPass::~GeometryPass()
{
}

void GeometryPass::Execute()
{
    Logger::Log("Start Geometry Pass Execution");

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projMatrix = m_pCamera->GetProjectionMatrix(ProjectionType::PERSPECTIVE, m_screenWidth, m_screenHeight, 0.1f, 100.0f);
    glm::mat4 viewMatrix = m_pCamera->GetViewMatrix();

    m_pProgram->Use();
    m_pProgram->SetUniform("projection", projMatrix);
    m_pProgram->SetUniform("view", viewMatrix);

    Logger::Log("Start geometries");
    for(uint32 i = 0; i<m_meshes.size(); ++i)
    {
        Logger::Log("Mesh");

        m_pProgram->SetUniform("model", m_meshes[i]->GetTransform());
        m_meshes[i]->Draw(m_pProgram.get());
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
