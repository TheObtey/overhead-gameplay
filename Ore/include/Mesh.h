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

    void SetActive(bool isActive) {m_isActive = isActive;}
    bool GetIsActive() const {return m_isActive;}
    void Draw(IProgram const& program) const override;
    void SetTransform(glm::mat4 const& transform) { m_transform = transform; }
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
    std::vector<Texture*> m_textures;
    bool m_isActive;
    glm::mat4 m_transform;
    std::vector<glm::mat4> m_bonesTransform;
    std::vector<glm::mat4> m_bonesOffset;
};

#endif
