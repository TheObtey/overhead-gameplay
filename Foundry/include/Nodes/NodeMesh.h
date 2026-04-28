#ifndef FOUNDRY_NODEMESH__H_
#define FOUNDRY_NODEMESH__H_


#include "Mesh.h"
#include "NodeVisual.h"
#include "GeometryFactory.h"


struct SceneMesh;
struct EditorSceneMeshData;
enum class MeshGeometrySourceType : uint8
{
    PRIMITIVE,
    FBX
};

class SerializedTexturesData : public ISerializable
{
public:
    Ore::TextureMaterialType type = Ore::TextureMaterialType::DIFFUSE;
    std::filesystem::path path = "";

    virtual void Serialize(SerializedObject& datas) const override;
    virtual void Deserialize(SerializedObject const& datas) override;
    inline static ISerializable* CreateInstance() { return std::make_unique<SerializedTexturesData>().release(); }
};
REGISTER_ISERIALIZABLE(SerializedTexturesData, SerializedTexturesData::CreateInstance);


class NodeMesh : public NodeVisual
{
public:
    class Proxy;

    explicit NodeMesh(std::string const &name);
    ~NodeMesh() override = default;

    virtual void OnUpdate(double delta) override;
    virtual void Serialize(SerializedObject &datas) const override;
    virtual void Deserialize(SerializedObject const &datas) override;
    void DeserializeTextures(SerializedObject const &datas);
    void DeserializeTex(std::string& textPath, Ore::TextureMaterialType type, bool isSet,uint8 id);
    bool IsVisible() override;

    void SetGeometry(sptr<Ore::Geometry> const &geometry) const;
    void SetActive(bool isActive) const;
    void SetPrimitive(PrimitivesType primitiveType);
    void SetFbxPath(std::filesystem::path const &fbxPath);
    void SetFromEditorSceneMesh(EditorSceneMeshData const & sceneMesh, std::filesystem::path const& fbxPath);
    void SetFromSceneMesh(SceneMesh const & sceneMesh, std::filesystem::path const& fbxPath, bool usePos = true);
    void SetTexturesPaths(SceneMesh const& sceneMesh);
    MeshGeometrySourceType GetGeometrySourceType() const { return m_geometrySourceType; }
    PrimitivesType GetPrimitiveType() const { return m_primitiveType; }
    std::filesystem::path const &GetFbxPath() const { return m_fbxPath; }
    void SetDiffuseTexturePath(std::filesystem::path const& path) { m_diffusePath = path; }
    std::filesystem::path const& GetDiffuseTexturePath() const { return m_diffusePath; }

    uint32 GetMeshID() const {return m_meshIDInSceneFBX;}

    template <typename... Args>
    void AddTextures(Args... textures);

    static ISerializable *CreateInstance();
    uptr<Node> Clone() override;

    std::vector<SerializedTexturesData> const& GetTexturePaths() const {return m_texturesPaths;}

    glm::mat4x4 m_meshlocalTransform{ 1.0f };
protected:
    void AttachScriptDeserialize(uptr<LuaScriptInstance>& script) override;
    void SetDefaultTextures();

private:
    uptr<Ore::Mesh> m_pMesh;
    std::vector<sptr<Ore::Texture>> m_textures;
    std::vector<Ore::TextureMaterialType> m_textureMaterialTypes;
    MeshGeometrySourceType m_geometrySourceType = MeshGeometrySourceType::PRIMITIVE;
    PrimitivesType m_primitiveType = PrimitivesType::CUBE;
    std::filesystem::path m_fbxPath{};
    uint32 m_meshIDInSceneFBX = 0;

    std::vector<SerializedTexturesData> m_texturesPaths;
    std::filesystem::path m_diffusePath;

    friend class NodeViewport;
};

template <typename... Args>
void NodeMesh::AddTextures(Args... textures)
{
    ((m_textures.push_back(textures), m_textureMaterialTypes.push_back(textures->GetTextureMaterialType())), ...);
    m_pMesh->SetTextures(m_textures);
}

REGISTER_ISERIALIZABLE(NodeMesh, NodeMesh::CreateInstance);

#include "Scripting/Proxies/NodeMeshProxy.inl"

#endif
