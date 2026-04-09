#ifndef ORE_MESH__H_
#define ORE_MESH__H_

#include "IMesh.h"
#include "Geometry.h"
#include "Texture.h"

#include <span>
#include <glm/glm.hpp>
namespace Ore
{
using TextureSpan = std::span<sptr<Texture>>;

class Mesh final : public IMesh 
{
public:
    Mesh() = default;
    Mesh(sptr<Geometry> const& geometry, TextureSpan textures, glm::mat4 const& transform);
    ~Mesh() = default;

    void SetActive(bool const isActive) {m_isActive = isActive;}
    bool GetIsActive() const {return m_isActive;}
    void SetTransform(glm::mat4 const& transform) { m_transform = transform; }
    void SetGeometry(sptr<Geometry> const& geometry) { m_pGeometry = geometry; }
    void SetTextures(TextureSpan const textures) { m_textures = textures; }
    void Draw(IProgram const& program) const override;
    glm::mat4 const& GetTransform() const { return m_transform; }


    std::vector<glm::mat4> const& GetBonesTransform() const { return m_bonesTransform; }
    glm::mat4 const& GetBoneValue(uint8 index) const { return m_bonesTransform[index]; }
    void SetBones(std::vector<glm::mat4> const& bones) { m_bonesTransform = bones; }
    void SetBoneValue(uint32 indx, glm::mat4 const& bone) { m_bonesTransform[indx] = bone; };

    void SetBonesOffsets(std::vector<glm::mat4> const& bones) { m_bonesOffset = bones;}
    glm::mat4 const& GetBoneOffset(uint32 index) const { return m_bonesOffset[index]; }
    std::vector<glm::mat4> const& GetAllBonesOffset() const { return m_bonesOffset; }

private:
    sptr<Geometry> m_pGeometry;
    TextureSpan m_textures;
    glm::mat4 m_transform;
    std::vector<glm::mat4> m_bonesTransform;
    std::vector<glm::mat4> m_bonesOffset;
    bool m_isActive = true;
};
}
#endif
