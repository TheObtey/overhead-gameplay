#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(uint32 const id)
{
    m_id = id;
}

VertexArrayObject::VertexArrayObject(VertexArrayObject const& other)
{
    m_id = other.m_id;
}

void VertexArrayObject::Bind()
{
    glBindVertexArray(m_id);
}

