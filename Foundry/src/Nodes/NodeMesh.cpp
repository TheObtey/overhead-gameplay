#include "Nodes/NodeMesh.h"
#include "Nodes/NodeViewport.h"
#include "GeometryFactory.h"
#include "Servers/GraphicServer.h"
#include "Serialization/SerializeObject.hpp"

namespace
{
    sptr<Ore::Geometry> BuildPrimitiveGeometry(PrimitivesType const primitiveType)
    {
        GeoInfo const &geoInfo = GeometryFactory::GetGeometry(primitiveType);
        return std::make_shared<Ore::Geometry>(geoInfo.m_vertices, geoInfo.m_indices);
    }
}

NodeMesh::NodeMesh(std::string const &name) : NodeVisual(name)
{
    m_pMesh = std::make_unique<Ore::Mesh>();
    SetPrimitive(PrimitivesType::CUBE);
    // problem here
    if (!s_IsInEditor)
        AddTextures(GraphicServer::GetDefaultTexture());
    m_pMesh->SetTransform(m_transform.GetMatrix());
}

void NodeMesh::OnUpdate(double delta)
{
    NodeVisual::OnUpdate(delta);
    m_pMesh->SetTransform(m_transform.GetMatrix());

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
    /*TODO Re;ove Default and add fbx parsing + setting geometry logic here*/
    m_pMesh->SetGeometry(GraphicServer::GetDefaultGeo());
}

void NodeMesh::Serialize(SerializedObject &datas) const
{
    Node3D::Serialize(datas);
    datas.SetType("NodeMesh");

    bool const isActive = m_pMesh->GetIsActive();
    datas.AddPublicElement("IsActive", &isActive);

    int const geometrySourceType = static_cast<int>(m_geometrySourceType);
    int const primitiveType = static_cast<int>(m_primitiveType);
    std::string const fbxPath = m_fbxPath.string();

    datas.AddPublicElement("GeometrySourceType", &geometrySourceType);
    datas.AddPublicElement("PrimitiveType", &primitiveType);
    datas.AddPublicElement("FbxPath", &fbxPath);

    uint32 textureCount = static_cast<uint32>(m_textureMaterialTypes.size());
    if (textureCount == 0)
        textureCount = static_cast<uint32>(m_textures.size());
    datas.AddPublicElement("TextureCount", &textureCount);
}

void NodeMesh::Deserialize(SerializedObject const &datas)
{
    Node3D::Deserialize(datas);

    if (!m_pMesh)
        m_pMesh = std::make_unique<Ore::Mesh>();

    int geometrySourceType = static_cast<int>(MeshGeometrySourceType::PRIMITIVE);
    int primitiveType = static_cast<int>(PrimitivesType::CUBE);
    std::string fbxPath;

    datas.GetPublicElement("GeometrySourceType", &geometrySourceType);
    datas.GetPublicElement("PrimitiveType", &primitiveType);
    datas.GetPublicElement("FbxPath", &fbxPath);

    m_geometrySourceType = static_cast<MeshGeometrySourceType>(geometrySourceType);
    m_primitiveType = static_cast<PrimitivesType>(primitiveType);
    m_fbxPath = fbxPath;

    if (m_geometrySourceType == MeshGeometrySourceType::PRIMITIVE)
    {
        SetPrimitive(m_primitiveType);
    }
    else
    {
        SetFbxPath(m_fbxPath);
    }

    bool isActive = true;
    datas.GetPublicElement("IsActive", &isActive);
    m_pMesh->SetActive(isActive);

    m_textures.clear();
    m_textureMaterialTypes.clear();

    uint32 textureCount = 1;
    datas.GetPublicElement("TextureCount", &textureCount);

    if (textureCount == 0)
    {
        if (!s_IsInEditor)
            m_textureMaterialTypes.push_back(Ore::TextureMaterialType::DIFFUSE);
    }
    else
    {
        if (!s_IsInEditor)
            m_textureMaterialTypes.assign(textureCount, Ore::TextureMaterialType::DIFFUSE);
    }

    for (Ore::TextureMaterialType const type : m_textureMaterialTypes)
    {
        if (!s_IsInEditor)
            m_textures.push_back(std::make_shared<Ore::Texture>("res/textures/Default.png", Ore::TextureType::TYPE_2D, type));
    }

    if (!s_IsInEditor)
        m_pMesh->SetTextures(m_textures);
    m_pMesh->SetTransform(m_transform.GetMatrix());
}

ISerializable *NodeMesh::CreateInstance()
{
    return Node::CreateNode<NodeMesh>("NodeMesh").release();
}
