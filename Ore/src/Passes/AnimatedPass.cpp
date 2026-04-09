#include "Passes/AnimatedPass.h"
#include "Logger.hpp"
#include "Mesh.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace Ore;

AnimatedPass::AnimatedPass(Program& program) : Pass(program)
{}

AnimatedPass::AnimatedPass(Program& program, Camera* pCamera) : Pass(program, pCamera)
{}

void AnimatedPass::AddSkeletalMesh(Mesh const& mesh)
{
    m_skeletalMeshes.push_back(mesh);
}

void AnimatedPass::Execute()
{
    //Logger::Log("Start Animation Pass Execution");

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program.Use();
    m_program.SetUniform("viewProj", m_pCamera->GetViewProjMatrix());

    //Logger::Log("Start SkeletalMesh");
    for(uint32 i = 0; i<m_skeletalMeshes.size(); ++i)
    {
        std::vector<glm::mat4> boneTransforms = m_skeletalMeshes[i].get().GetBonesTransform();

        for(int i = 0; i<boneTransforms.size(); ++i)
        {
            m_program.SetUniform("gBones[" + std::to_string(i) + "]", boneTransforms[i]);
        }

        m_program.SetUniform("model", glm::inverse(m_skeletalMeshes[i].get().GetTransform()));
        m_skeletalMeshes[i].get().Draw(m_program);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_skeletalMeshes.clear();
}
