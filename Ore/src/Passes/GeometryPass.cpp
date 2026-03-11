#include "Passes/GeometryPass.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
GeometryPass::GeometryPass(Shader& shader, std::vector<sptr<Geometry>> const& geometries, uint32 gBuffer, Camera& camera) : Pass(shader)
{
    m_geometries = geometries;
    m_pCamera = std::make_shared<Camera>(camera);
}

void GeometryPass::Execute()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projMatrix = m_pCamera->GetProjectionMatrix(ProjectionType::PERSPECTIVE, 1920, 1080, 0.1f, 100.0f);
    glm::mat4 viewMatrix = m_pCamera->GetViewMatrix();

    m_pShader->Use();
    m_pShader->SetMat4("projection", projMatrix);
    m_pShader->SetMat4("view", viewMatrix);

    glm::mat4 model = glm::mat4(1.0f); // TODO just take transform matrix as parameter

    for(uint32 i = 0; i<m_geometries.size(); ++i)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, i, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f));
        m_pShader->SetMat4("model", model);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
