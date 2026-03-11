#include "EditorRaylib3D.h"
#include <Nodes/AllNodes.h>

namespace rl
{
#include <raymath.h>
}

#include <rlgl.h>

EditorRaylib3D::EditorRaylib3D(){}
EditorRaylib3D::~EditorRaylib3D(){}

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
}

void EditorRaylib3D::Update(float deltaTime)
{
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		UpdateCamera(&m_camera, CAMERA_FREE);
	}
	if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
		UpdateCamera(&m_camera, CAMERA_ORBITAL);
	}
}

void EditorRaylib3D::AddDrawableObject(std::string const& name, Node* pNode)
{
	if (dynamic_cast<Node3D*>(pNode) != nullptr) // TestTemp
	{
		Instanciate3DMesh(name, pNode);
	}

	//if (dynamic_cast<NodeLight*>(pNode) != nullptr)
	//{
	//	//DrawLight();
	//}
	//else if (dynamic_cast<NodeMesh3D*>(pNode) != nullptr)
	//{
	//	Instanciate3DMesh(name, jsonObject);
	//}
	//else if (dynamic_cast<NodeCollider3D*>(pNode) != nullptr)
	//{
	//	//DrawCollider3D(m_loadedMeshs[element["DATAS"]["m_name"]]);
	//}
}

Matrix EditorRaylib3D::FindParentWorldMatrix(Node* pNode)
{
	Node3D* pNode3DParent = nullptr; // Node3D
	Vector3 worldPos = { 0.0f };
	Vector3 worldScale = { 1.0f,1.0f,1.0f };
	Vector3 worldRot = { 0.0f };

	pNode3DParent = dynamic_cast<Node3D*>(pNode);
	while (pNode3DParent == nullptr)
	{
		Node* pParent = pNode->GetParent();

		if (pParent == nullptr) break;
		pNode3DParent = dynamic_cast<Node3D*>(pParent);
		if (pNode3DParent != nullptr)
		{
			worldPos = { pNode3DParent->GetWorldPosition().x,pNode3DParent->GetWorldPosition().y, pNode3DParent->GetWorldPosition().z};
			worldScale = { pNode3DParent->GetWorldScale().x,pNode3DParent->GetWorldScale().y, pNode3DParent->GetWorldScale().z };
			worldRot = { pNode3DParent->GetWorldRotation().x,pNode3DParent->GetWorldRotation().y, pNode3DParent->GetWorldRotation().z };
		}
	}
	return rl::MatrixMultiply(rl::MatrixRotateXYZ({ worldRot.x, worldRot.y, worldRot.z }), rl::MatrixMultiply(rl::MatrixScale(worldScale.x, worldScale.y, worldScale.z), rl::MatrixTranslate(worldPos.x, worldPos.y, worldPos.z)));
}

void EditorRaylib3D::UpdateDrawableElement(std::string const& name, Node* pNode)
{
	if (dynamic_cast<Node3D*>(pNode) != nullptr) // TestTemp
	{
		m_loadedMeshs[name].get()->worldMatrix = FindParentWorldMatrix(pNode);
	}
}

void EditorRaylib3D::Instanciate3DMesh(std::string const& name, Node* pNodeMesh3D) // NodeMesh3D
{
	if (m_loadedMeshs.find(name) != m_loadedMeshs.end())
	{
		// ERROR
	}
	else
	{
		Mesh m_mesh = GenMeshCube(1, 1, 1);
		// Custom Mesh with Mesh3D
		UploadMesh(&m_mesh, false);
		m_loadedMeshs[name] = std::make_unique<DrawableElement>();
		m_loadedMeshs[name].get()->mesh = std::make_unique<Mesh>(m_mesh); // GetMesh;..

		m_loadedMeshs[name].get()->worldMatrix = FindParentWorldMatrix(pNodeMesh3D);
	}
}


void EditorRaylib3D::InstanciateCollider3D()
{
}

void EditorRaylib3D::InstanciateLight()
{
}


void EditorRaylib3D::Render()
{
	BeginMode3D(m_camera);
	DrawViewPort();

	for (std::map<std::string, uptr<DrawableElement>>::iterator it  = m_loadedMeshs.begin(); it != m_loadedMeshs.end(); it++)
	{
		DrawMesh(*it->second.get()->mesh.get(), m_defaultMaterial, it->second.get()->worldMatrix);
		//Matrix m = it->second.get()->worldMatrix;
		//Transform gTransform = GizmoIdentity();
		//DrawGizmo3D(GIZMO_TRANSLATE, &gTransform);
	}

	EndMode3D();
}

void EditorRaylib3D::DrawViewPort()
{
	DrawGrid(20, 1.0f);
	DrawLine3D({ 0, 0, 0 }, { 500, 0, 0 }, RED);
	DrawLine3D({ 0, 0, 0 }, { 0, 500, 0 }, GREEN);
	DrawLine3D({ 0, 0, 0 }, { 0, 0, 500 }, BLUE);
	DrawLine3D({ 0, 0, 0 }, { -500, 0, 0 }, RED);
	DrawLine3D({ 0, 0, 0 }, { 0, -500, 0 }, GREEN);
	DrawLine3D({ 0, 0, 0 }, { 0, 0,-500 }, BLUE);
}


void EditorRaylib3D::Shutdown()
{
}
