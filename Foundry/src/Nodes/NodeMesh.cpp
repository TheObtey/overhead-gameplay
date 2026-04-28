#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "GeometryFactory.h"
#include "Servers/GraphicServer.h"
#include "Serialization/SerializeObject.hpp"
#include "Serialization/ISerializableEncaps.h"
#include "AssetLoading/AssetLoader.h"
#include "AssetLoading/EditorAssetLoader.h"
#include "AssetLoading/AssetsStructs.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace
{
    sptr<Ore::Geometry> BuildPrimitiveGeometry(PrimitivesType const primitiveType)
    {
        GeoInfo const& geoInfo = GeometryFactory::GetGeometry(primitiveType);
        return std::make_shared<Ore::Geometry>(geoInfo.m_vertices, geoInfo.m_indices);
    }

    std::filesystem::path ResolveRuntimeFbxPath(std::filesystem::path const& fbxPath)
    {
        if (fbxPath.empty())
            return {};

        if (fbxPath.is_absolute())
            return fbxPath;

        if (std::filesystem::exists(fbxPath))
            return fbxPath;

        std::filesystem::path const inResFbx = std::filesystem::path("res/fbx") / fbxPath.filename();
        if (std::filesystem::exists(inResFbx))
            return inResFbx;

        std::filesystem::path const inGameResFbx = std::filesystem::path("../Game/res/fbx") / fbxPath.filename();
        if (std::filesystem::exists(inGameResFbx))
            return inGameResFbx;

        return fbxPath;
    }
}

NodeMesh::NodeMesh(std::string const &name) : NodeVisual(name)
{
    m_pMesh = std::make_unique<Ore::Mesh>();
    SetPrimitive(PrimitivesType::CUBE);
    // problem here
    if (!s_IsInEditor) SetDefaultTextures();
    m_pMesh->SetTransform(m_worldTransform);
}

void NodeMesh::OnUpdate(double delta)
{
    NodeVisual::OnUpdate(delta);
    m_pMesh->SetTransform( m_worldTransform * m_meshlocalTransform);

    if (IsVisible() && m_pViewport)
        m_pViewport->AddMesh(*this);
}

bool NodeMesh::IsVisible()
{
    // TODO CULLING
    return true;
}

void NodeMesh::SetGeometry(sptr<Ore::Geometry> const &geometry) const
{
    m_pMesh->SetGeometry(geometry);
}

void NodeMesh::SetActive(bool isActive) const
{
    m_pMesh->SetActive(isActive);
}

void NodeMesh::SetFromEditorSceneMesh(EditorSceneMeshData const& sceneMesh, std::filesystem::path const& fbxPath)
{
    m_geometrySourceType = MeshGeometrySourceType::FBX;
    //SetName(sceneMesh.name);
    m_worldTransform = sceneMesh.meshMatrix;
    m_meshIDInSceneFBX = sceneMesh.ID;
    m_fbxPath = fbxPath;
    m_texturesPaths.clear();
    bool hasDiffuse = false;
    bool hasNormal = false;
    bool hasOpacity = false;
    bool hasSpecular = false;
    for (uint8 i = 0; i < sceneMesh.textures.size(); ++i)
    {
        m_texturesPaths.push_back({});
        m_texturesPaths[i].path = sceneMesh.textures[i].path;
        m_texturesPaths[i].type = static_cast<Ore::TextureMaterialType>(sceneMesh.textures[i].textureType);
        switch (m_texturesPaths[i].type)
        {
        case Ore::TextureMaterialType::DIFFUSE:
            hasDiffuse = true;
            break;
        case Ore::TextureMaterialType::NORMAL:
            hasNormal = true;
            break;
        case Ore::TextureMaterialType::OPACITY:
            hasOpacity = true;
            break;
        case Ore::TextureMaterialType::SPECULAR:
            hasSpecular = true;
            break;
        default:
            break;
        }
    }
    if (hasDiffuse == false)
    {
        std::string path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::DIFFUSE);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::DIFFUSE;
        te.path = path;
        m_texturesPaths.push_back(te);
    }
    if (hasNormal == false)
    {
        std::string path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::NORMAL);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::NORMAL;
        te.path = path;
        m_texturesPaths.push_back(te);
    }
    if (hasOpacity == false)
    {
        std::string path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::OPACITY);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::OPACITY;
        te.path = path;
        m_texturesPaths.push_back(te);
    }
    if (hasSpecular == false)
    {
        std::string path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::SPECULAR);
        SerializedTexturesData te2 = {};
        te2.type = Ore::TextureMaterialType::SPECULAR;
        te2.path = path;
        m_texturesPaths.push_back(te2);
    }

}

void NodeMesh::SetTexturesPaths(SceneMesh const& sceneMesh)
{
    bool hasDiffuse = false;
    bool hasNormal  = false;
    bool hasOpacity = false;
    bool hasSpecular = false;

    for (uint8 i = 0; i < m_textures.size(); ++i)
    {
        switch (m_textures[i]->GetTextureMaterialType())
        {
        case Ore::TextureMaterialType::DIFFUSE:
            hasDiffuse = true;
            break;
        case Ore::TextureMaterialType::NORMAL:
            hasNormal = true;
            break;
        case Ore::TextureMaterialType::OPACITY:
            hasOpacity = true;
            break;
        case Ore::TextureMaterialType::SPECULAR:
            hasSpecular = true;
            break;
        default:
            break;
        }
        SerializedTexturesData te = {};
        te.type = m_textures[i]->GetTextureMaterialType();
        te.path = sceneMesh.paths[i];
        m_texturesPaths.push_back(te);
    }
    if (hasDiffuse == false)
    {
        
        sptr<Ore::Texture> text = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::DIFFUSE),Ore::TextureMaterialType::DIFFUSE);
        m_textures.push_back(text);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::DIFFUSE;
        te.path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::DIFFUSE);
        m_texturesPaths.push_back(te);
    }
    if (hasNormal == false)
    {
        sptr<Ore::Texture> text = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::NORMAL), Ore::TextureMaterialType::NORMAL);
        m_textures.push_back(text);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::NORMAL;
        te.path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::NORMAL);
        m_texturesPaths.push_back(te);
    }
    if (hasOpacity == false)
    {
        sptr<Ore::Texture> text = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::OPACITY), Ore::TextureMaterialType::OPACITY);
        m_textures.push_back(text);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::OPACITY;
        te.path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::OPACITY);
        m_texturesPaths.push_back(te);
    }
    if (hasSpecular == false)
    {
        sptr<Ore::Texture> text = AssetLoader::GetSharedTexture(AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::SPECULAR), Ore::TextureMaterialType::SPECULAR);
        m_textures.push_back(text);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::SPECULAR;
        te.path = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::SPECULAR);
        m_texturesPaths.push_back(te);
    }
}

void NodeMesh::SetFromSceneMesh(SceneMesh const& sceneMesh, std::filesystem::path const& fbxPath)
{
    m_geometrySourceType = MeshGeometrySourceType::FBX;
    sptr<Ore::Geometry> geo = std::make_shared<Ore::Geometry>(sceneMesh.vertices, sceneMesh.indices);
    m_pMesh->SetGeometry(geo);
    SetName(sceneMesh.name);
    m_textures = sceneMesh.meshTextures;
    glm::vec3 scale(1.0f);
    glm::quat rotation;
    glm::vec3 translation;
    glm::vec3 skew;
    glm::vec4 perspective;
    if (glm::decompose(sceneMesh.meshMatrix, scale, rotation, translation, skew, perspective))
    {
        SetWorldPosition(translation);
        SetWorldScale(scale);
        SetWorldRotationQuaternion(rotation);
    }
    bool hasNormal = false;
    SetTexturesPaths(sceneMesh);
    m_pMesh->SetTextures(m_textures);
    m_fbxPath = fbxPath;
    m_meshIDInSceneFBX = sceneMesh.ID;
}

void NodeMesh::SetPrimitive(PrimitivesType primitiveType)
{
    m_geometrySourceType = MeshGeometrySourceType::PRIMITIVE;
    m_primitiveType = primitiveType;
    m_fbxPath.clear();
    if (!s_IsInEditor)
        m_pMesh->SetGeometry(BuildPrimitiveGeometry(primitiveType));
}

void NodeMesh::SetFbxPath(std::filesystem::path const &fbxPath)
{
    m_geometrySourceType = MeshGeometrySourceType::FBX;
    m_fbxPath = fbxPath;

    if (s_IsInEditor)
        return;

    std::filesystem::path const resolvedPath = ResolveRuntimeFbxPath(m_fbxPath);
    sptr<SceneData> scene = AssetLoader::LoadSceneFromFile(resolvedPath.string(), AssetLoader::FileType::FBX);

    if (!scene || scene->allMesh.empty() || !scene->allMesh[0])
    {
        m_pMesh->SetGeometry(GraphicServer::GetDefaultGeo());
        return;
    }

    SceneMesh const& importedMesh = *scene->allMesh[0];
    if (importedMesh.vertices.empty() || importedMesh.indices.empty())
    {
        m_pMesh->SetGeometry(GraphicServer::GetDefaultGeo());
        return;
    }

    sptr<Ore::Geometry> geometry = std::make_shared<Ore::Geometry>(importedMesh.vertices, importedMesh.indices);
    m_pMesh->SetGeometry(geometry);

    m_textures.clear();

    if (!importedMesh.meshTextures.empty())
    {
        m_textures = importedMesh.meshTextures;
    }
    m_pMesh->SetTextures(m_textures);
}

void NodeMesh::Serialize(SerializedObject &datas) const
{

    Node3D::Serialize(datas);
    datas.SetType("NodeMesh");

    bool const isActive = m_pMesh->GetIsActive();
    datas.AddPublicElement("IsActive", &isActive);

    int const geometrySourceType = static_cast<int>(m_geometrySourceType);

    ClampedInt primitiveTypeClamped = ClampedInt(0, 3, static_cast<uint32>(m_primitiveType));
    std::string const fbxPath = m_fbxPath.string();
    uint32 tMesh = m_meshIDInSceneFBX;
    if (m_meshIDInSceneFBX > 3000)
        tMesh = 0;
    
    datas.AddPrivateElement("IdInFBX", &tMesh);
    datas.AddPublicElement("GeometrySourceType", &geometrySourceType);
    datas.AddPublicElement("PrimitiveType", static_cast<ISerializable*>(&primitiveTypeClamped));
    datas.AddPublicElement("FbxPath", &fbxPath);
    for (uint8  i = 0; i < m_texturesPaths.size();++i)
    {
        switch (m_texturesPaths[i].type)
        {
        case Ore::TextureMaterialType::DIFFUSE:
            datas.AddPublicElement("DiffuseTexturePath", &m_texturesPaths[i].path);
            break;
        case Ore::TextureMaterialType::NORMAL:
            datas.AddPublicElement("NormalTexturePath", &m_texturesPaths[i].path);
            break;
        case Ore::TextureMaterialType::OPACITY:
            datas.AddPublicElement("OpacityTexturePath", &m_texturesPaths[i].path);
            break;
        }
    }
    datas.AddPrivateArray("TexturePaths");
    for (uint8 i = 0; i < m_texturesPaths.size();++i)
    {
        datas.AddPrivateElementInArray("TexturePaths",static_cast<ISerializable const*>(&m_texturesPaths[i]));
    }
}

void NodeMesh::DeserializeTex(std::string& textPath, Ore::TextureMaterialType type, bool isSet, uint8 id)
{
    if (!isSet)
    {
        textPath = AssetLoader::GetDefaultTexturePath(type);
    }
    if (m_texturesPaths[id].path == "")
        m_texturesPaths[id].path = textPath;
    else if (isSet && m_texturesPaths[id].path.string() != textPath && textPath != "")
        m_texturesPaths[id].path = textPath;
    else
        textPath = m_texturesPaths[id].path.string();
}

void NodeMesh::DeserializeTextures(SerializedObject const& datas)
{
    std::string diffusePath = "";
    std::string normalPath = "";
    std::string opacityPath = "";
    bool textureSet = false;
    for (uint8 i = 0; i < m_texturesPaths.size();++i)
    {
        switch (m_texturesPaths[i].type)
        {
        case Ore::TextureMaterialType::DIFFUSE:
            if (diffusePath != "")
            {
                m_texturesPaths.erase(m_texturesPaths.begin()+i);
                break;
            }
            textureSet = datas.TryGetPublicElement("DiffuseTexturePath", &diffusePath);
            DeserializeTex(diffusePath, Ore::TextureMaterialType::DIFFUSE, textureSet, i);
            if (!s_IsInEditor)
                m_textures.push_back(AssetLoader::GetSharedTexture(diffusePath,Ore::TextureMaterialType::DIFFUSE));
            break;
        case Ore::TextureMaterialType::NORMAL:
            if (normalPath != "")
            {
                m_texturesPaths.erase(m_texturesPaths.begin() + i);
                break;
            }
            textureSet = datas.TryGetPublicElement("NormalTexturePath", &normalPath);
            DeserializeTex(normalPath, Ore::TextureMaterialType::NORMAL, textureSet, i);
            if (!s_IsInEditor)
                m_textures.push_back(AssetLoader::GetSharedTexture(normalPath, Ore::TextureMaterialType::NORMAL));
            break;
        case Ore::TextureMaterialType::SPECULAR:
            if (opacityPath != "")
            {
                m_texturesPaths.erase(m_texturesPaths.begin() + i);
                break;
            }
            textureSet = datas.TryGetPublicElement("OpacityTexturePath", &opacityPath);
            DeserializeTex(opacityPath, Ore::TextureMaterialType::SPECULAR, textureSet, i);
            if (!s_IsInEditor)
                m_textures.push_back(AssetLoader::GetSharedTexture(opacityPath, Ore::TextureMaterialType::SPECULAR));
            break;
        case Ore::TextureMaterialType::OPACITY:
            if (opacityPath != "")
            {
                m_texturesPaths.erase(m_texturesPaths.begin() + i);
                break;
            }
            textureSet = datas.TryGetPublicElement("OpacityTexturePath", &opacityPath);
            DeserializeTex(opacityPath, Ore::TextureMaterialType::OPACITY, textureSet, i);
            if (!s_IsInEditor)
                m_textures.push_back(AssetLoader::GetSharedTexture(opacityPath, Ore::TextureMaterialType::OPACITY));
            break;

        default:
            break;
        }
    }
    if (diffusePath == "")
    {
        diffusePath = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::DIFFUSE);
        m_texturesPaths.push_back({});
        m_texturesPaths[m_texturesPaths.size() - 1].path = diffusePath;
        m_texturesPaths[m_texturesPaths.size() - 1].type = Ore::TextureMaterialType::DIFFUSE;
        if (!s_IsInEditor)
            m_textures.push_back(AssetLoader::GetSharedTexture(diffusePath, Ore::TextureMaterialType::DIFFUSE));
    }
    if (normalPath == "")
    {
        normalPath = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::NORMAL);
        m_texturesPaths.push_back({});
        m_texturesPaths[m_texturesPaths.size() - 1].path = normalPath;
        m_texturesPaths[m_texturesPaths.size() - 1].type = Ore::TextureMaterialType::NORMAL;
        if (!s_IsInEditor)
            m_textures.push_back(AssetLoader::GetSharedTexture(normalPath, Ore::TextureMaterialType::NORMAL));
    }
    if (opacityPath == "")
    {
        opacityPath = AssetLoader::GetDefaultTexturePath(Ore::TextureMaterialType::OPACITY);
        m_texturesPaths.push_back({});
        m_texturesPaths[m_texturesPaths.size() - 1].path = opacityPath;
        m_texturesPaths[m_texturesPaths.size() - 1].type = Ore::TextureMaterialType::OPACITY;
        m_texturesPaths.push_back({});
        m_texturesPaths[m_texturesPaths.size() - 1].path = opacityPath;
        m_texturesPaths[m_texturesPaths.size() - 1].type = Ore::TextureMaterialType::SPECULAR;
        if (!s_IsInEditor)
        {
            m_textures.push_back(AssetLoader::GetSharedTexture(opacityPath, Ore::TextureMaterialType::OPACITY));
            m_textures.push_back(AssetLoader::GetSharedTexture(opacityPath, Ore::TextureMaterialType::SPECULAR));
        }
    }
    m_diffusePath = diffusePath;
    uint32 textureCount = 1;
    if (!datas.TryGetPrivateElement("TextureCount", &textureCount))
    {
        textureCount = 1;
    }
    if (!s_IsInEditor)
    {
        m_pMesh->SetTextures(m_textures);
    }
}

void NodeMesh::Deserialize(SerializedObject const& datas)
{
    Node3D::Deserialize(datas);

    if (!m_pMesh)
        m_pMesh = std::make_unique<Ore::Mesh>();

    int geometrySourceType = static_cast<int>(MeshGeometrySourceType::PRIMITIVE);

    ClampedInt primitiveTypeClamped = ClampedInt(0, 3, static_cast<uint32>(PrimitivesType::CUBE));

    std::string fbxPath;
    datas.GetPublicElement("GeometrySourceType", &geometrySourceType);
    bool primitiveLoaded = false;
    try
    {
        datas.GetPublicElement("PrimitiveType", static_cast<ISerializable*>(&primitiveTypeClamped));
        primitiveLoaded = true;
    }
    catch (...)
    {
    }
    if (!primitiveLoaded)
    {
        try
        {
            int legacyPrimitiveType = static_cast<int>(PrimitivesType::CUBE);
            datas.GetPublicElement("PrimitiveType", &legacyPrimitiveType);
            primitiveTypeClamped = ClampedInt(0, 3, legacyPrimitiveType);
            primitiveLoaded = true;
        }
        catch (...)
        {
            primitiveTypeClamped = ClampedInt(0, 3, static_cast<uint32>(PrimitivesType::CUBE));
        }
    }
    if (!datas.TryGetPrivateElement("IdInFBX", &m_meshIDInSceneFBX)) {
        m_meshIDInSceneFBX = 0;
    }
    datas.GetPublicElement("FbxPath", &fbxPath);
    m_geometrySourceType = static_cast<MeshGeometrySourceType>(geometrySourceType);
    m_primitiveType = static_cast<PrimitivesType>(primitiveTypeClamped.value);
    m_fbxPath = fbxPath;
    m_textures.clear();
    if (m_geometrySourceType == MeshGeometrySourceType::PRIMITIVE)
    {
        SetPrimitive(m_primitiveType);
        std::vector<ISerializable*> tempList = datas.GetPrivateArray<ISerializable*>("TexturePaths");
        m_texturesPaths.clear();
        for (uint8 i = 0; i < tempList.size(); i++)
            m_texturesPaths.push_back(*static_cast<SerializedTexturesData*>(tempList[i]));;
    }
    else
    {
        if (!s_IsInEditor)
            SetFromSceneMesh(*AssetLoader::LoadSceneFromFile(m_fbxPath.string(), AssetLoader::FileType::FBX)->allMesh[m_meshIDInSceneFBX], m_fbxPath);
        else
            SetFromEditorSceneMesh(EditorAssetLoader::LoadSceneFromFile(m_fbxPath.string(), EditorAssetLoader::FileType::FBX)->meshes[m_meshIDInSceneFBX], m_fbxPath);
    }
    DeserializeTextures(datas);
    bool isActive = true;
    datas.GetPublicElement("IsActive", &isActive);
    m_pMesh->SetActive(isActive);

    Update(1);
    m_pMesh->SetTransform(m_meshlocalTransform * m_transform.GetMatrix());
}

void NodeMesh::AttachScriptDeserialize(uptr<LuaScriptInstance>& script)
{
    AttachScript<NodeMesh>(script, *this);
}

void NodeMesh::SetDefaultTextures()
{
    AddTextures(
           GraphicServer::GetDefaultTexture(),
           GraphicServer::GetDefaultOpacity(),
           GraphicServer::GetDefaultSpecular());
}

ISerializable *NodeMesh::CreateInstance()
{
    return Node::CreateNode<NodeMesh>("NodeMesh").release();
}

uptr<Node> NodeMesh::Clone()
{
	uptr<NodeMesh> clone = Node::CreateNode<NodeMesh>(GetName());

	SerializedObject datas;
	Serialize(datas);
	clone->Deserialize(datas);

	return clone;
}

void SerializedTexturesData::Serialize(SerializedObject& datas) const
{
    datas.SetType("SerializedTexturesData");
    datas.AddPublicElement("TextureType", &type);
    std::string val = path.string();
    datas.AddPublicElement("TexturePath", &val);
}

void SerializedTexturesData::Deserialize(SerializedObject const& datas)
{
    datas.GetPublicElement("TextureType", &type);
    std::string spath = "";
    datas.GetPublicElement("TexturePath", &spath);
    path = spath;
}