#include "Passes/LightPass.h"
#include "TextureObject.h"
#include "Logger.hpp"

#include <glad/glad.h>

using namespace Ore;
LightPass::LightPass(Program& program,LightSpan lights) : Pass(program)
{
    m_quadVAOId = 0;
    m_quadVBOId = 0;

    SetLights(lights);
    GenerateQuad();
}

LightPass::LightPass(Program& program, LightSpan lights, Camera* pCamera) : Pass(program, pCamera)
{
    m_quadVAOId = 0;
    m_quadVBOId = 0;

    SetLights(lights);
    GenerateQuad();
}

void LightPass::GenerateQuad()
{
    std::vector<float> quadVertices =
    {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.1f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.1f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.1f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.1f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &m_quadVAOId);
    glGenBuffers(1, &m_quadVBOId);

    m_quadVAO = std::make_unique<VertexArrayObject>(m_quadVAOId);
    m_quadVAO->Bind();

    m_quadVBO = std::make_unique<Buffer<float>>(quadVertices, m_quadVBOId, BufferType::BUFFER, true);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void LightPass::SetLights(LightSpan const& lights)
{
    m_lights = lights;
}

void LightPass::Execute()
{
    if (m_pCamera == nullptr) return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    m_program.Use();

    glActiveTexture(GL_TEXTURE0);
    m_pGPosition->Bind();
    glActiveTexture(GL_TEXTURE1);
    m_pGNormal->Bind();
    glActiveTexture(GL_TEXTURE2);
    m_pGAlbedoSpec->Bind();

    for (uint32 i = 0; i < m_lights.size(); ++i)
    {
        std::string const indexStr = std::to_string(i);
        Light& light = m_lights[i];

        m_program.SetUniform("lights[" + indexStr + "].Position", light.position);
        m_program.SetUniform("lights[" + indexStr + "].Color", glm::vec3{ light.color.r, light.color.g, light.color.b });

        m_program.SetUniform("lights[" + indexStr + "].Linear", light.linear);
        m_program.SetUniform("lights[" + indexStr + "].Quadratic", light.quadratic);

        float const maxBrightness = std::fmaxf(std::fmaxf(light.color.r, light.color.g), light.color.b);
        float radius = (-light.linear + std::sqrt(light.linear * light.linear - 4 * light.quadratic * (light.constant - (256.0f / 5.0f) * maxBrightness)))
            / (2.0f * light.quadratic);
        m_program.SetUniform("lights[" + indexStr + "].Radius", radius);
    }

    m_program.SetUniform("viewPos", m_pCamera->GetPosition());
    RenderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightPass::RenderQuad()
{
    m_quadVAO->Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
