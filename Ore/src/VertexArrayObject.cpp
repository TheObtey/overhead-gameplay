#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(uint32 id)
{
    m_id = id;
}

void VertexArrayObject::Bind()
{
    glBindVertexArray(m_id);
}
