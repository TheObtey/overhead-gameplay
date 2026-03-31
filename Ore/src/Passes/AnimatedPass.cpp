#include "Passes/AnimatedPass.h"
#include "Logger.hpp"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

AnimatedPass::AnimatedPass(Program const& program, sptr<Camera> pCamera) : Pass(program, pCamera)
{
}

AnimatedPass::~AnimatedPass()
{
}

void AnimatedPass::AddSkeletalMesh(Mesh const& mesh)
{
    m_skeletalMeshes.push_back(mesh);
}

void AnimatedPass::Execute()
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
    for(uint32 i = 0; i<m_skeletalMeshes.size(); ++i)
    {
        std::vector<glm::mat4> boneTransforms = m_skeletalMeshes[i].get().GetBonesTransform();

        for(int i = 0; i<boneTransforms.size(); ++i)
        {
            m_pProgram->SetUniform("gBones[" + std::to_string(i) + "]", boneTransforms[i]);
        }

        m_pProgram->SetUniform("model", m_skeletalMeshes[i].get().GetTransform());
        m_skeletalMeshes[i].get().Draw(*m_pProgram);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_skeletalMeshes.clear();
}
