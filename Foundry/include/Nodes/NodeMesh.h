#ifndef FOUNDRY_NODEMESH__H_
#define FOUNDRY_NODEMESH__H_

#include "Mesh.h"
#include "NodeVisual.h"

class NodeMesh : public NodeVisual
{
public:
    explicit NodeMesh(std::string const& name);
    ~NodeMesh() override = default;

    virtual void OnUpdate(double delta) override;
    virtual void Serialize(SerializedObject& datas) const override {};
    virtual void Deserialize(SerializedObject const& datas) override {};

    bool IsVisible() override;

    void SetGeometry(sptr<Ore::Geometry> const& geometry) const;

    template <typename ... Args>
    void AddTextures(Args ... textures);

    static ISerializable* CreateInstance();

private:
    uptr<Ore::Mesh> m_pMesh;
    std::vector<sptr<Ore::Texture>> m_textures;

    friend class NodeViewport;
};


template <typename ... Args>
void NodeMesh::AddTextures(Args ... textures)
{
    (m_textures.push_back(textures), ...);
    m_pMesh->SetTextures(m_textures);
}

REGISTER_ISERIALIZABLE(NodeMesh, NodeVisual::CreateInstance);

#endif
