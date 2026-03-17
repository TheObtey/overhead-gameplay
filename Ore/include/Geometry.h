#ifndef ORE_GEOMETRY__H_
#define ORE_GEOMETRY__H_

#include "Define.h"
#include "IGeometry.h"
#include "Buffer.h"
#include "Shader.h"

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
    Geometry(std::vector<Vertex> const& points, std::vector<uint32> const& indices);
    Geometry(Geometry const& other);
    ~Geometry() override;

    void Draw(sptr<Shader> shader);

protected:
    void AddPoints(std::vector<Vertex> const& vertices);
    void AddIndices(std::vector<uint32> const& indices);

private:
    void Setup();

private:
    uptr<Buffer<Vertex>> m_pVertexBuffer;
    uptr<Buffer<uint32>> m_pIndexBuffer;
    uptr<VertexArrayObject> m_pVao;
    uint32 m_indiceSize;

};

#endif //!ORE_GEOMETRY__H_
