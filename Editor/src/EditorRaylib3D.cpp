#include "EditorRaylib3D.h"
#include "Editor.h"
#include "NewGizmo.hpp"

#include <AssetLoading/EditorAssetLoader.h>
#include <Debug.h>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <cmath>
#include <filesystem>

namespace
{
Mesh BuildRaylibMesh(GeoInfo const& geoInfo)
{
	Mesh mesh = {};
	mesh.vertexCount = static_cast<int>(geoInfo.m_vertices.size());
	mesh.triangleCount = static_cast<int>(geoInfo.m_indices.size() / 3);

	mesh.vertices = static_cast<float*>(MemAlloc(sizeof(float) * mesh.vertexCount * 3));
	mesh.normals = static_cast<float*>(MemAlloc(sizeof(float) * mesh.vertexCount * 3));
	mesh.texcoords = static_cast<float*>(MemAlloc(sizeof(float) * mesh.vertexCount * 2));

	for (int i = 0; i < mesh.vertexCount; ++i)
	{
		mesh.vertices[i * 3 + 0] = geoInfo.m_vertices[i].position.x;
		mesh.vertices[i * 3 + 1] = geoInfo.m_vertices[i].position.y;
		mesh.vertices[i * 3 + 2] = geoInfo.m_vertices[i].position.z;

		mesh.normals[i * 3 + 0] = geoInfo.m_vertices[i].normal.x;
		mesh.normals[i * 3 + 1] = geoInfo.m_vertices[i].normal.y;
		mesh.normals[i * 3 + 2] = geoInfo.m_vertices[i].normal.z;

		mesh.texcoords[i * 2 + 0] = geoInfo.m_vertices[i].texCoords.x;
		mesh.texcoords[i * 2 + 1] = geoInfo.m_vertices[i].texCoords.y;
	}

	mesh.indices = static_cast<unsigned short*>(MemAlloc(sizeof(unsigned short) * geoInfo.m_indices.size()));
	for (size_t i = 0; i < geoInfo.m_indices.size(); ++i)
	{
		mesh.indices[i] = static_cast<unsigned short>(geoInfo.m_indices[i]);
	}

	UploadMesh(&mesh, false);
	return mesh;
}

std::filesystem::path ResolveEditorFbxPath(std::filesystem::path const& fbxPath)
{
	if (fbxPath.empty())
		return {};

	if (fbxPath.is_absolute() && std::filesystem::exists(fbxPath))
		return fbxPath;

	if (std::filesystem::exists(fbxPath))
		return fbxPath;

	std::filesystem::path const inResFbx = std::filesystem::path("res/fbx") / fbxPath.filename();
	if (std::filesystem::exists(inResFbx))
		return inResFbx;

	std::filesystem::path const inGameResFbx = std::filesystem::path("../Game/res/fbx") / fbxPath.filename();
	if (std::filesystem::exists(inGameResFbx))
		return inGameResFbx;

	return {};
}
}

bool AreMatrixEqual(Matrix const& m1, Matrix const& m2)
{
	return memcmp(&m1, &m2, sizeof(Matrix)) == 0;

	float EPS = 0.0001f;
	return ((m1.m0 - m2.m0) < EPS && (m2.m0 - m1.m0) < EPS) && ((m1.m1 - m2.m1) < EPS && (m2.m1 - m1.m1) < EPS) && ((m1.m2 - m2.m2) < EPS && (m2.m2 - m1.m2) < EPS) && ((m1.m3 - m2.m3) < EPS && (m2.m3 - m1.m3) < EPS) &&
		   ((m1.m4 - m2.m4) < EPS && (m2.m4 - m1.m4) < EPS) && ((m1.m5 - m2.m5) < EPS && (m2.m5 - m1.m5) < EPS) && ((m1.m6 - m2.m6) < EPS && (m2.m6 - m1.m6) < EPS) && ((m1.m7 - m2.m7) < EPS && (m2.m7 - m1.m7) < EPS) &&
		   ((m1.m8 - m2.m8) < EPS && (m2.m8 - m1.m8) < EPS) && ((m1.m9 - m2.m9) < EPS && (m2.m9 - m1.m9) < EPS) && ((m1.m10 - m2.m10) < EPS && (m2.m10 - m1.m10) < EPS) && ((m1.m11 - m2.m11) < EPS && (m2.m11 - m1.m11) < EPS) &&
		   ((m1.m12 - m2.m12) < EPS && (m2.m12 - m1.m12) < EPS) && ((m1.m13 - m2.m13) < EPS && (m2.m13 - m1.m13) < EPS) && ((m1.m14 - m2.m14) < EPS && (m2.m14 - m1.m14) < EPS) && ((m1.m15 - m2.m15) < EPS && (m2.m15 - m1.m15) < EPS);
}

Matrix GlmToMatrix(glm::mat4x4 const& m1)
{
	glm::mat4x4 mat = glm::transpose(m1);
	Matrix res = {};
	memcpy(&res, &mat, sizeof(glm::mat4x4));
	return res;
}

bool AreTransform3DMAtrixEqual(Transform3D const* m1, Node3D const* m2)
{
	bool step = glm::vec3{ m1->GetPosition() } == m2->GetPosition();
	step = step && glm::vec3{m1->GetScale()} == m2->GetScale();
	step = step && m1->GetRotationQuat() == m2->GetWorldRotationQuaternion();
	return step;	
}

EditorRaylib3D::EditorRaylib3D() {}
EditorRaylib3D::~EditorRaylib3D()
{}

void EditorRaylib3D::Init(float const& width, float const& height)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(width, height, "Foundry Editor");
	
	SetTargetFPS(144);

	// Static Cam 
	m_camera.position = { 10.0f, 10.0f, 10.0f };
	m_camera.target = { 0.0f, 0.0f, 0.0f };
	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 45.0f;
	m_camera.projection = CAMERA_PERSPECTIVE;

	m_defaultMaterial = LoadMaterialDefault();
	m_gizmoFlags = GizmoFlags::TRANSLATE | GizmoFlags::SCALE | GizmoFlags::ROTATE;
}

void EditorRaylib3D::Update(float deltaTime)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		UpdateCamera(&m_camera, CAMERA_FREE);
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
		UpdateCamera(&m_camera, CAMERA_ORBITAL);
	}

	if (IsKeyDown(KEY_O))
	{
		//ChangeCameraState();
	}
}

void EditorRaylib3D::ChangeCamera(CameraState state)
{
	switch (state)
	{
	case EditorRaylib3D::PERSPECTIVE:
		m_camera.position = { 10.0f, 10.0f, 10.0f };
		m_camera.target = { 0.0f, 0.0f, 0.0f };
		m_camera.up = { 0.0f, 1.0f, 0.0f };
		m_camera.fovy = 45.0f;
		m_camera.projection = CAMERA_PERSPECTIVE;
		m_gizmoSize = 1.0f;
		UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
		break;
	case EditorRaylib3D::ORTHOGRAPHIC:
		SetCameraOnAxis(m_currentAxis);
		break;
	case EditorRaylib3D::FREE:
		m_cameraMode = CAMERA_FREE;
		break;
	case EditorRaylib3D::ORBITAL:
		m_cameraMode = CAMERA_ORBITAL;
		break;

	default:
		break;
	}
}

void EditorRaylib3D::UpdateDisplay(Node* pNode)
{
	UpdateDrawableElement(pNode);
	for (uint32 i = 0; i < pNode->GetChildCount(); ++i)
	{
		UpdateDisplay(&pNode->GetChild(i));
	}
}

void EditorRaylib3D::AddDrawableObject(std::string const& name, Node* pNode)
{
	Node3D* pNode3D = dynamic_cast<Node3D*>(pNode);
	if (pNode3D != nullptr && !m_loadedNode3D.contains(name))
	{
		m_loadedNode3D[name] = std::make_unique<Node3DElement>();

		m_loadedNode3D[name]->gizmoTransform = RayGizmo::GizmoIdentity();
		m_loadedNode3D[name]->gizmoTransform.translation = {
			pNode3D->GetWorldPosition().x,
			pNode3D->GetWorldPosition().y,
			pNode3D->GetWorldPosition().z
		};
		m_loadedNode3D[name]->gizmoTransform.scale = {
			pNode3D->GetWorldScale().x,
			pNode3D->GetWorldScale().y,
			pNode3D->GetWorldScale().z
		};
		m_loadedNode3D[name]->gizmoTransform.rotation = {
			pNode3D->GetWorldRotationQuaternion().x,
			pNode3D->GetWorldRotationQuaternion().y,
			pNode3D->GetWorldRotationQuaternion().z,
			pNode3D->GetWorldRotationQuaternion().w
		};
		m_loadedNode3D[name]->worldMatrix = RayGizmo::GizmoToMatrix(m_loadedNode3D[name]->gizmoTransform);
	}

	NodeMesh* pNodeMesh = dynamic_cast<NodeMesh*>(pNode);
	if (pNodeMesh == nullptr) return;

	Instanciate3DMesh(name, pNode);
}

Node* EditorRaylib3D::FindNode3DWorldMatrix(Node* pNode, Matrix& outMatrix)
{
	Node3D* pNode3D = nullptr;
	outMatrix = {};
	Node* pParent = pNode;

	pNode3D = dynamic_cast<Node3D*>(pNode);

	while (pNode3D == nullptr)
	{
		pParent = pParent->GetParent();
		if (pParent == nullptr) break;
		pNode3D = dynamic_cast<Node3D*>(pParent);
	}

	if (pNode3D != nullptr)
	{
		outMatrix = GlmToMatrix(pNode3D->GetWorldMatrix());
	}

	return pNode3D;
}

void EditorRaylib3D::UpdateDrawableElement(Node* pNode)
{
	std::string const name = pNode->GetName();

	Node3D* pNode3D = dynamic_cast<Node3D*>(pNode);
	if (pNode3D != nullptr)
	{
		if (!m_loadedNode3D.contains(name))
		{
			m_loadedNode3D[name] = std::make_unique<Node3DElement>();
			m_loadedNode3D[name]->gizmoTransform = RayGizmo::GizmoIdentity();
			m_loadedNode3D[name]->worldMatrix = RayGizmo::GizmoToMatrix(m_loadedNode3D[name]->gizmoTransform);
		}

		Node3DElement& node3D = *m_loadedNode3D[name].get();

		if (node3D.gizmoUpdated)
		{
			node3D.gizmoUpdated = false;

			pNode3D->SetWorldPosition({
				node3D.gizmoTransform.translation.x,
				node3D.gizmoTransform.translation.y,
				node3D.gizmoTransform.translation.z
				});
			pNode3D->SetWorldScale({
				node3D.gizmoTransform.scale.x,
				node3D.gizmoTransform.scale.y,
				node3D.gizmoTransform.scale.z
				});
			pNode3D->SetWorldRotationQuaternion(glm::quat{
				node3D.gizmoTransform.rotation.w,
				node3D.gizmoTransform.rotation.x,
				node3D.gizmoTransform.rotation.y,
				node3D.gizmoTransform.rotation.z
				});

			m_gizmoDirty = true;
		}

		node3D.gizmoTransform.translation = {
			pNode3D->GetWorldPosition().x,
			pNode3D->GetWorldPosition().y,
			pNode3D->GetWorldPosition().z
		};
		node3D.gizmoTransform.scale = {
			pNode3D->GetWorldScale().x,
			pNode3D->GetWorldScale().y,
			pNode3D->GetWorldScale().z
		};
		node3D.gizmoTransform.rotation = {
			pNode3D->GetWorldRotationQuaternion().x,
			pNode3D->GetWorldRotationQuaternion().y,
			pNode3D->GetWorldRotationQuaternion().z,
			pNode3D->GetWorldRotationQuaternion().w
		};
		node3D.worldMatrix = RayGizmo::GizmoToMatrix(node3D.gizmoTransform);
	}

	NodeMesh* pNodeMesh = dynamic_cast<NodeMesh*>(pNode);
	if (pNodeMesh == nullptr) return;

	if (!m_loadedMeshes.contains(name))
	{
		Instanciate3DMesh(name, pNode);
		if (!m_loadedMeshes.contains(name)) return;
	}

	bool needRecreate = false;
	DrawableElement& drawable = *m_loadedMeshes[name].get();

	if (pNodeMesh->GetGeometrySourceType() == MeshGeometrySourceType::PRIMITIVE)
	{
		needRecreate =
			(drawable.geometrySourceType != MeshGeometrySourceType::PRIMITIVE) ||
			(drawable.primitiveType != pNodeMesh->GetPrimitiveType());
	}
	else
	{
		std::filesystem::path const resolvedPath = ResolveEditorFbxPath(pNodeMesh->GetFbxPath());
		if (resolvedPath.empty())
		{
			RemoveDrawableElement(name);
			return;
		}

		needRecreate =
			(drawable.geometrySourceType != MeshGeometrySourceType::FBX) ||
			(drawable.loadedFbxPath != resolvedPath.string());
	}

	if (needRecreate)
	{
		RemoveDrawableElement(name);
		Instanciate3DMesh(name, pNode);
		if (!m_loadedMeshes.contains(name)) return;
	}

	DrawableElement& updatedDrawable = *m_loadedMeshes[name].get();
	UpdateDrawableTexture(*pNodeMesh, updatedDrawable);

	if (m_loadedNode3D.contains(name))
	{
		updatedDrawable.worldMatrix = m_loadedNode3D[name]->worldMatrix;
	}
	else if (pNode3D != nullptr)
	{
		updatedDrawable.worldMatrix = GlmToMatrix(pNode3D->GetWorldMatrix());
	}
}


void EditorRaylib3D::UpdateElementName(std::string const& oldName, Node* pNode)
{
	std::string const& newName = pNode->GetName();

	if (m_loadedMeshes.contains(oldName))
	{
		m_loadedMeshes[newName] = std::move(m_loadedMeshes[oldName]);
		m_loadedMeshes.erase(oldName);
	}

	if (m_loadedNode3D.contains(oldName))
	{
		m_loadedNode3D[newName] = std::move(m_loadedNode3D[oldName]);
		m_loadedNode3D.erase(oldName);
	}

	m_selectedObject = newName;
}

void EditorRaylib3D::RemoveDrawableElement(std::string const& elementName)
{
	if (m_loadedMeshes.contains(elementName))
	{
		auto& drawable = m_loadedMeshes[elementName];
		if (drawable->hasTexture)
		{
			UnloadTexture(drawable->diffuseTexture);
		}
		UnloadMaterial(drawable->material);

		for (DrawableSubMesh& subMesh : drawable->meshes)
		{
			if (subMesh.mesh)
			{
				UnloadMesh(*subMesh.mesh.get());
			}
		}

		m_loadedMeshes.erase(elementName);
	}

	if (m_loadedNode3D.contains(elementName))
	{
		m_loadedNode3D.erase(elementName);
	}
}

void EditorRaylib3D::ClearWindow()
{
	for (auto& [name, drawable] : m_loadedMeshes)
	{
		if (drawable)
		{
			for (DrawableSubMesh& subMesh : drawable->meshes)
			{
				if (subMesh.mesh)
				{
					UnloadMesh(*subMesh.mesh.get());
				}
			}	
		}
	}
	m_loadedMeshes.clear();
}

void EditorRaylib3D::Instanciate3DMesh(std::string const& name, Node* pNodeMesh3D)
{
	NodeMesh* pNodeMesh = dynamic_cast<NodeMesh*>(pNodeMesh3D);
	if (pNodeMesh == nullptr) return;

	if (m_loadedMeshes.find(name) != m_loadedMeshes.end())
	{
		return;
	}

	m_loadedMeshes[name] = std::make_unique<DrawableElement>();
	DrawableElement& drawable = *m_loadedMeshes[name].get();
	drawable.worldMatrix = MatrixIdentity();
	drawable.material = LoadMaterialDefault();

	if (pNodeMesh->GetGeometrySourceType() == MeshGeometrySourceType::PRIMITIVE)
	{
		GeoInfo const& geoInfo = GeometryFactory::GetGeometry(pNodeMesh->GetPrimitiveType());
		Mesh m_mesh = BuildRaylibMesh(geoInfo);

		DrawableSubMesh subMesh;
		subMesh.mesh = std::make_unique<Mesh>(m_mesh);
		subMesh.localMatrix = MatrixIdentity();

		drawable.meshes.push_back(std::move(subMesh));
		drawable.geometrySourceType = MeshGeometrySourceType::PRIMITIVE;
		drawable.primitiveType = pNodeMesh->GetPrimitiveType();
		drawable.loadedFbxPath.clear();
		drawable.loadedFbxDiffusePath.clear();
	}
	else
	{
		std::filesystem::path const resolvedPath = ResolveEditorFbxPath(pNodeMesh->GetFbxPath());
		if (resolvedPath.empty())
		{
			m_loadedMeshes.erase(name);
			return;
		}

		sptr<EditorSceneData> scene = EditorAssetLoader::LoadSceneFromFile(resolvedPath.string(), EditorAssetLoader::FBX);
		if (!scene || scene->meshes.empty())
		{
			m_loadedMeshes.erase(name);
			return;
		}

		for (EditorSceneMeshData const& importedMesh : scene->meshes)
		{
			if (importedMesh.geometry.m_vertices.empty() || importedMesh.geometry.m_indices.empty())
				continue;

			Mesh m_mesh = BuildRaylibMesh(importedMesh.geometry);

			DrawableSubMesh subMesh;
			subMesh.mesh = std::make_unique<Mesh>(m_mesh);

			subMesh.localMatrix = GlmToMatrix(importedMesh.meshMatrix);
	

			if (drawable.loadedFbxDiffusePath.empty() && !importedMesh.textures.empty())
			{
				drawable.loadedFbxDiffusePath = importedMesh.textures[0].path;
			}

			drawable.meshes.push_back(std::move(subMesh));
		}

		if (drawable.meshes.empty())
		{
			m_loadedMeshes.erase(name);
			return;
		}

		drawable.geometrySourceType = MeshGeometrySourceType::FBX;
		drawable.loadedFbxPath = resolvedPath.string();
	}

	UpdateDrawableTexture(*pNodeMesh, drawable);

	if (m_loadedNode3D.contains(name))
	{
		drawable.worldMatrix = m_loadedNode3D[name]->worldMatrix;
	}
	else
	{
		Matrix world = {};
		Node3D* pNode3D = static_cast<Node3D*>(FindNode3DWorldMatrix(pNodeMesh3D, world));
		if (pNode3D != nullptr)
		{
			drawable.worldMatrix = GlmToMatrix(pNode3D->GetWorldMatrix());
		}
	}
}



void EditorRaylib3D::InstanciateCollider3D()
{
}

void EditorRaylib3D::InstanciateLight()
{
}

std::string EditorRaylib3D::ResolveEditorTexturePath(std::filesystem::path const& logicalPath)
{
	if (logicalPath.empty())
		return "../Game/res/textures/Default.png";

	if (logicalPath.is_absolute())
		return logicalPath.string();

	std::filesystem::path p = logicalPath;
	std::string const s = p.generic_string();

	if (s.rfind("res/textures/", 0) == 0)
		return ("../Game/" + s);

	return ("../Game/res/textures/" + p.filename().generic_string());
}


void EditorRaylib3D::UpdateDrawableTexture(NodeMesh const& nodeMesh, DrawableElement& drawable)
{
	std::filesystem::path wanted = nodeMesh.GetDiffuseTexturePath();
	if (wanted.empty())
	{
		if (drawable.geometrySourceType == MeshGeometrySourceType::FBX && !drawable.loadedFbxDiffusePath.empty())
		{
			wanted = drawable.loadedFbxDiffusePath;
		}
		else
		{
			wanted = "res/textures/Default.png";
		}
	}

	std::string const resolved = ResolveEditorTexturePath(wanted);
	if (resolved == drawable.loadedDiffusePath)
		return;

	if (drawable.hasTexture)
	{
		UnloadTexture(drawable.diffuseTexture);
		drawable.hasTexture = false;
	}

	Texture2D tex = LoadTexture(resolved.c_str());
	if (tex.id == 0)
	{
		tex = LoadTexture("../Game/res/textures/Default.png");
	}

	if (tex.id != 0)
	{
		drawable.diffuseTexture = tex;
		drawable.hasTexture = true;
		drawable.loadedDiffusePath = resolved;
		SetMaterialTexture(&drawable.material, MATERIAL_MAP_DIFFUSE, drawable.diffuseTexture);
	}
}

void EditorRaylib3D::Render()
{
	BeginMode3D(m_camera);

	for (auto it = m_loadedMeshes.begin(); it != m_loadedMeshes.end(); ++it)
	{
		DrawableElement& drawable = *it->second.get();

		for (DrawableSubMesh const& subMesh : drawable.meshes)
		{
			if (!subMesh.mesh) continue;
			// gauche a droite local * world
			Matrix finalMatrix = MatrixMultiply(subMesh.localMatrix, drawable.worldMatrix);
			DrawMesh(*subMesh.mesh.get(), drawable.material, finalMatrix);
		}
	}

	DrawViewPort();

	if (m_loadedNode3D.contains(m_selectedObject))
	{
		RayGizmo::SetGizmoSize(m_gizmoSize);

		rlDisableDepthTest();

		if (RayGizmo::DrawGizmo3D(static_cast<int>(m_gizmoFlags), &m_loadedNode3D[m_selectedObject]->gizmoTransform))
		{
			m_loadedNode3D[m_selectedObject]->gizmoUpdated = true;
		}

		rlEnableDepthTest();
	}

	EndMode3D();
}

void EditorRaylib3D::DrawViewPort()
{
	// Grille plus grande + léger offset en Y pour éviter le z-fighting
	rlPushMatrix();
	rlTranslatef(0.0f, -0.01f, 0.0f);
	DrawGrid(200, 1.0f);
	rlPopMatrix();

	DrawLine3D({ 0, 0, 0 }, { 500, 0, 0 }, RED);
	DrawLine3D({ 0, 0, 0 }, { 0, 500, 0 }, GREEN);
	DrawLine3D({ 0, 0, 0 }, { 0, 0, 500 }, BLUE);
	DrawLine3D({ 0, 0, 0 }, { -500, 0, 0 }, RED);
	DrawLine3D({ 0, 0, 0 }, { 0, -500, 0 }, GREEN);
	DrawLine3D({ 0, 0, 0 }, { 0, 0,-500 }, BLUE);
}

void EditorRaylib3D::SetTranslateGizmo(bool state)
{
	state ? (m_gizmoFlags |= GizmoFlags::TRANSLATE) : (m_gizmoFlags &= ~(GizmoFlags::TRANSLATE));
}
void EditorRaylib3D::SetScaleGizmo(bool state)
{
	state ? (m_gizmoFlags |= GizmoFlags::SCALE) : (m_gizmoFlags &= ~(GizmoFlags::SCALE));
}
void EditorRaylib3D::SetRotateGizmo(bool state)
{
	state ? (m_gizmoFlags |= GizmoFlags::ROTATE) : (m_gizmoFlags &= ~(GizmoFlags::ROTATE));
}

void EditorRaylib3D::SetCameraOnAxis(RaylibAxis axis)
{
	m_gizmoSize = 10.0f;
	m_camera.projection = CAMERA_ORTHOGRAPHIC;
	m_camera.fovy = 5.0f;
	switch (axis)
	{
	case EditorRaylib3D::X:
		m_camera.position = { 1.0f,0.0f,0.0f };
		m_camera.target = Vector3(0.0f, 0.0f, 0.0f);
		m_camera.up = Vector3(0.0f, 1.0f, 0.0f);
		break;
	case EditorRaylib3D::Y:
		m_camera.position = { 0.0f,1.0f,0.0f };
		m_camera.target = Vector3(0.0f, 0.0f, 0.0f);
		m_camera.up = Vector3(0.0f, 0.0f, 1.0f);
		break;
	case EditorRaylib3D::Z:
		m_camera.position = { 0.0f,0.0f,1.0f };
		m_camera.target = Vector3(0.0f, 0.0f, 0.0f);
		m_camera.up = Vector3(0.0f, 1.0f, 0.0f);
		break;
	default:
		break;
	}
}

void EditorRaylib3D::Shutdown()
{
}
