#ifndef ORE_MESH__H_
#define ORE_MESH__H_

#include "IMesh.h"
#include "Geometry.h"
#include "Texture.h"

#include <glm/glm.hpp>

class Mesh final : public IMesh 
{
public:
    Mesh(Geometry const& geometry, std::vector<Texture*> const& textures, glm::mat4 const& transform);
    ~Mesh() override;

    void Draw(IProgram const* program) override;
    void SetTransform(glm::mat4 const& transform) { m_transform = transform;}
    glm::mat4 const& GetTransform() const { return m_transform; }

private:
    sptr<Geometry> m_pGeometry;
    std::vector<Texture*> m_textures;
    glm::mat4 m_transform;
};

#endif
