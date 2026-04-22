#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "GeometryFactory.h"
#include "Servers/GraphicServer.h"
#include "Serialization/SerializeObject.hpp"
#include "Serialization/ISerializableEncaps.h"
#include "AssetLoading/AssetLoader.h"
#include "AssetLoading/AssetsStructs.h"

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
    if (!s_IsInEditor)
        AddTextures(GraphicServer::GetDefaultTexture());
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

void NodeMesh::SetFromSceneMesh(SceneMesh const& sceneMesh, std::filesystem::path const& fbxPath)
{
    m_geometrySourceType = MeshGeometrySourceType::FBX;
    sptr<Ore::Geometry> geo = std::make_shared<Ore::Geometry>(sceneMesh.vertices, sceneMesh.indices);
    m_pMesh->SetGeometry(geo);
    SetName(sceneMesh.name);
    m_textures = sceneMesh.meshTextures;
    m_meshlocalTransform = sceneMesh.meshMatrix;
    //SetLocalPosition({sceneMesh.meshMatrix[3][0],sceneMesh.meshMatrix[3][1], sceneMesh.meshMatrix[3][2]});
    bool hasNormal = false;
    for (uint8 i = 0; i < m_textures.size(); ++i)
    {
        if (m_textures[i]->GetTextureMaterialType() == Ore::TextureMaterialType::NORMAL)
            hasNormal = true;
        SerializedTexturesData te = {};
        te.type = m_textures[i]->GetTextureMaterialType();
        te.path = sceneMesh.paths[i];
        m_texturesPaths.push_back(te);
    }
    if (hasNormal == false)
    {
        sptr<Ore::Texture> text = std::make_shared<Ore::Texture>("res/textures/NormalMap.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::NORMAL);
        m_textures.push_back(text);
        SerializedTexturesData te = {};
        te.type = Ore::TextureMaterialType::NORMAL;
        te.path = "res/textures/NormalMap.png";
        m_texturesPaths.push_back(te);
    }
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
    else if (!m_diffuseTexturePath.empty())
    {
        m_textures.push_back(std::make_shared<Ore::Texture>(m_diffuseTexturePath, Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE));
    }
    else
    {
        m_textures.push_back(std::make_shared<Ore::Texture>("res/textures/Default.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE));
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
    std::string const diffusePath = m_diffuseTexturePath.string();
    uint32 tMesh = m_meshIDInSceneFBX;
    if (m_meshIDInSceneFBX > 3000)
        tMesh = 0;
    datas.AddPrivateElement("IdInFBX", &tMesh);
    datas.AddPublicElement("GeometrySourceType", &geometrySourceType);
    datas.AddPublicElement("PrimitiveType", static_cast<ISerializable*>(&primitiveTypeClamped));
    datas.AddPublicElement("FbxPath", &fbxPath);
    datas.AddPublicElement("DiffuseTexturePath", &diffusePath);
    datas.AddPublicArray("TexturePaths");
    for (uint8 i = 0; i < m_texturesPaths.size();++i)
    {
        datas.AddPublicElementInArray("TexturePaths",static_cast<ISerializable const*>(&m_texturesPaths[0]));
    }
    uint32 textureCount = 1;
    //if (textureCount == 0)
    //    textureCount = static_cast<uint32>(m_textures.size());

    datas.AddPrivateElement("TextureCount", &textureCount);
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

    std::string diffusePath;
    if (!datas.TryGetPublicElement("DiffuseTexturePath", &diffusePath))
    {
        diffusePath = "res/textures/Default.png";
    }
    m_diffuseTexturePath = diffusePath;

    uint32 textureCount = 1;
    if (!datas.TryGetPrivateElement("TextureCount", &textureCount))
    {
        textureCount = 1;
    }
    datas.GetPrivateElement("IdInFBX", &m_meshIDInSceneFBX);
    datas.GetPublicElement("FbxPath", &fbxPath);
    m_geometrySourceType = static_cast<MeshGeometrySourceType>(geometrySourceType);
    m_primitiveType = static_cast<PrimitivesType>(primitiveTypeClamped.value);
    m_fbxPath = fbxPath;
    m_diffuseTexturePath = diffusePath;
    std::vector<ISerializable*> tempList = datas.GetPublicArray<ISerializable*>("TexturePaths");
    m_texturesPaths.clear();
    for (uint8 i = 0; i < tempList.size(); i++)
        m_texturesPaths.push_back(*static_cast<SerializedTexturesData*>(tempList[i]));

    if (m_geometrySourceType == MeshGeometrySourceType::PRIMITIVE)
    {
        SetPrimitive(m_primitiveType);
    }
    else
    {

        if (!s_IsInEditor)
        {
            Logger::Log("SceneMeshLoading");
            SetFromSceneMesh(*AssetLoader::LoadSceneFromFile(m_fbxPath.string(), AssetLoader::FileType::FBX)->allMesh[m_meshIDInSceneFBX], m_fbxPath);
        }
        //else
        //    ...
    }

    bool isActive = true;
    datas.GetPublicElement("IsActive", &isActive);
    m_pMesh->SetActive(isActive);
    if (!s_IsInEditor)
    {
        if (m_textures.size() == 0)
        {
            bool hasDiffuse = false;
            for (uint8 i = 0; i < m_texturesPaths.size(); i++)
            {
                if (m_texturesPaths[i].type == Ore::TextureMaterialType::DIFFUSE)
                    hasDiffuse = true;
                m_textures.push_back(std::make_shared<Ore::Texture>(m_texturesPaths[i].path, Ore::TextureType::TYPE_2D, m_texturesPaths[i].type));
            }

            if (hasDiffuse == false)
            {
                if (!m_diffuseTexturePath.empty())
                    m_textures.push_back(std::make_shared<Ore::Texture>(m_diffuseTexturePath, Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE));
                else
                    m_textures.push_back(std::make_shared<Ore::Texture>("res/textures/Default.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE));
            }
        }

        m_pMesh->SetTextures(m_textures);
    }
    Update(1);
    m_pMesh->SetTransform(m_meshlocalTransform * m_transform.GetMatrix());
}

void NodeMesh::AttachScriptDeserialize(uptr<LuaScriptInstance>& script)
{
    AttachScript<NodeMesh>(script, *this);
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