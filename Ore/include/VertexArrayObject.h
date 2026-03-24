#ifndef ORE_VERTEX_ARRAY_OBJECT__H_
#define ORE_VERTEX_ARRAY_OBJECT__H_

#include "Define.h"
#include "IVertexArrayObject.h"

#include <glad/glad.h>

class VertexArrayObject final : public IVertexArrayObject 
{
public:
    VertexArrayObject(uint32 id);
    ~VertexArrayObject() override;

    void Bind() override;

private:
    GLuint m_id;

};
#endif //!ORE_VERTEX_ARRAY_OBJECT__H_
