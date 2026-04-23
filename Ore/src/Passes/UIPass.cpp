#include "Passes/UIPass.h"

#include <Logger.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Ore;
UIPass::UIPass(Program& program) : Pass(program)
{
    std::vector<uint8>          indices;
    std::vector<Vertex>         vertices;

    GLuint buffers[3];
    glGenBuffers(3, buffers);

    m_pIndicesBuffer     = std::make_unique<Buffer<uint8>>(indices, buffers[0], BufferType::BUFFER_ELEMENT, false);
    m_pVertexBuffer      = std::make_unique<Buffer<Vertex>>(vertices, buffers[1], BufferType::BUFFER, false);
}

UIPass::UIPass(Program& program, Camera* pcamera) : Pass(program, pcamera)
{

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint buffers[3];
    glGenBuffers(3, buffers);
    
    Vertex quadVertices[4] = 
    {
        {glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3( 1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 1.0f,  0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3( 0.0f,  0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
    };

    uint8 quadIndices[6] = 
    {
        0, 1, 2,
        0, 2, 3
    };

    m_pVertexArray      = std::make_unique<VertexArrayObject>(VAO);
    m_pIndicesBuffer    = std::make_unique<Buffer<uint8>>(quadIndices, buffers[0], BufferType::BUFFER_ELEMENT, true);
    m_pVertexBuffer     = std::make_unique<Buffer<Vertex>>(quadVertices, buffers[1], BufferType::BUFFER, true);

    m_pVertexArray->Bind();
    m_pVertexBuffer->Bind();
    m_pIndicesBuffer->Bind();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

void UIPass::Execute()
{
    if (m_pCamera == nullptr) return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    m_program.Use();
    m_program.SetUniform("resolution", glm::vec2(m_screenWidth, m_screenHeight));

    int32 i = 0;
    for(UIElement const& element : m_elements)
    {
        element.Draw(m_program, i);
    }

    m_pVertexArray->Bind();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0, i);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_elements.clear();
}

void UIPass::AddUIElement(UIElement const& element)
{
    m_elements.push_back(element);
}
