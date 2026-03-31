#ifndef ORE_MESH__H_
#define ORE_MESH__H_

#include "IMesh.h"
#include "Geometry.h"
#include "Texture.h"

#include <span>
#include <glm/glm.hpp>

using TextureSpan = std::span<std::reference_wrapper<Texture>>;

class Mesh final : public IMesh 
{
public:
    Mesh() = default;
    Mesh(sptr<Geometry> const& geometry, TextureSpan textures, glm::mat4 const& transform);
    ~Mesh() override = default;

    void SetActive(bool const isActive) {m_isActive = isActive;}
    bool GetIsActive() const {return m_isActive;}
    glm::mat4 const& GetTransform() const { return m_transform; }

    void Draw(IProgram const& program) const override;

    void SetTransform(glm::mat4 const& transform) { m_transform = transform;}
    void SetGeometry(sptr<Geometry> const& geometry) { m_pGeometry = geometry; }
    void SetTextures(std::span<std::reference_wrapper<Texture>> textures) { m_textures = textures; }

private:
    sptr<Geometry> m_pGeometry;
    TextureSpan m_textures;
    bool m_isActive = true;
    glm::mat4 m_transform;
};

#endif
