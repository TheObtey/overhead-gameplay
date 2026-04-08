#ifndef ORE_GEOMETRY__H_
#define ORE_GEOMETRY__H_

#include "IGeometry.h"
#include "Buffer.h"

#include <Define.h>
#include <glm/glm.hpp>

struct Vertex 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class VertexArrayObject;
class Geometry final : public IGeometry
{
public:
    Geometry(std::span<Vertex const> points, std::vector<uint32> const& indices);
    Geometry(Geometry const& other);
    ~Geometry() = default;

    void Draw() override;

protected:
    void AddPoints(std::span<Vertex const> vertices);
    void AddIndices(std::span<uint32 const> indices);

private:
    void Setup();

private:
    uptr<Buffer<Vertex>> m_pVertexBuffer;
    uptr<Buffer<uint32>> m_pIndexBuffer;
    uptr<VertexArrayObject> m_pVao;
    uint32 m_indiceSize;

};

#endif //!ORE_GEOMETRY__H_
