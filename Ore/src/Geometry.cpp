#include "Geometry.h"
#include "VertexArrayObject.h"
#include "Logger.hpp"

Geometry::Geometry(std::span<Vertex const> points, std::vector<uint32> const& indices)
{
    m_indiceSize = static_cast<uint32>(indices.size());
    GLuint vaoId;

    glGenVertexArrays(1, &vaoId);

    m_pVao = std::make_unique<VertexArrayObject>(vaoId);
    m_pVao->Bind();

    AddPoints(points);
    AddIndices(indices);
    Setup();    
}

Geometry::Geometry(Geometry const& other)
{
    m_pVao = std::make_unique<VertexArrayObject>(*other.m_pVao);
    m_indiceSize = other.m_indiceSize;
    m_pIndexBuffer = std::make_unique<Buffer<uint32>>(*other.m_pIndexBuffer);
    m_pVertexBuffer = std::make_unique<Buffer<Vertex>>(*other.m_pVertexBuffer);
}

void Geometry::Draw()
{
    m_pVao->Bind();
    glDrawElements(GL_TRIANGLES, m_indiceSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Geometry::AddPoints(std::span<Vertex const> vertices)
{
    GLuint id;
    glGenBuffers(1, &id); 

    m_pVertexBuffer = std::make_unique<Buffer<Vertex>>(vertices, id, BufferType::BUFFER, true);
}

void Geometry::AddIndices(std::span<uint32 const> indices)
{
    GLuint id;
    glGenBuffers(1, &id);

    m_pIndexBuffer = std::make_unique<Buffer<uint32>>(indices, id, BufferType::BUFFER_ELEMENT, true);
}

void Geometry::Setup()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
