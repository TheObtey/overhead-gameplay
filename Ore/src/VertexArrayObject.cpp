#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(uint32 id)
{
    m_id = id;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject const& other)
{
    m_id = other.m_id;
}

VertexArrayObject::~VertexArrayObject()
{
}

void VertexArrayObject::Bind()
{
    glBindVertexArray(m_id);
}

