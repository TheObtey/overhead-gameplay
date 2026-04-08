#ifndef FOUNDRY_NODEMESH__H_
#define FOUNDRY_NODEMESH__H_

#include "Mesh.h"
#include "NodeVisual.h"
#include "GeometryFactory.h"

enum class MeshGeometrySourceType : uint8
{
	PRIMITIVE,
	FBX
};

class NodeMesh : public NodeVisual
{
public:
    explicit NodeMesh(std::string const& name);
    ~NodeMesh() override = default;

    virtual void OnUpdate(double delta) override;
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

    bool IsVisible() override;

	void SetGeometry(sptr<Geometry> const& geometry);
	void SetActive(bool isActive) const;
	void SetPrimitive(PrimitivesType primitiveType);
	void SetFbxPath(std::filesystem::path const& fbxPath);
	MeshGeometrySourceType GetGeometrySourceType() const { return m_geometrySourceType; }
	PrimitivesType GetPrimitiveType() const { return m_primitiveType; }
	std::filesystem::path const& GetFbxPath() const { return m_fbxPath; }

    template <typename ... Args>
    void AddTextures(Args ... textures);

    static ISerializable* CreateInstance();

private:
    uptr<Mesh> m_pMesh;
    std::vector<sptr<Texture>> m_textures;
    std::vector<TextureMaterialType> m_textureMaterialTypes;
	MeshGeometrySourceType m_geometrySourceType = MeshGeometrySourceType::PRIMITIVE;
	PrimitivesType m_primitiveType = PrimitivesType::CUBE;
	std::filesystem::path m_fbxPath {};

    friend class NodeViewport;
};


template <typename ... Args>
void NodeMesh::AddTextures(Args ... textures)
{
	((m_textures.push_back(textures), m_textureMaterialTypes.push_back(textures->GetTextureMaterialType())), ...);
    m_pMesh->SetTextures(m_textures);
}

REGISTER_ISERIALIZABLE(NodeMesh, NodeMesh::CreateInstance);

#endif