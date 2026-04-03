#ifndef ORE_VERTEX_ARRAY_OBJECT__H_
#define ORE_VERTEX_ARRAY_OBJECT__H_

#include "Define.h"
#include "IVertexArrayObject.h"

class VertexArrayObject final : public IVertexArrayObject 
{
public:
    VertexArrayObject(uint32 id);
    VertexArrayObject(VertexArrayObject const& other);

    void Bind() override;

private:
    uint32 m_id;
};
#endif //!ORE_VERTEX_ARRAY_OBJECT__H_
