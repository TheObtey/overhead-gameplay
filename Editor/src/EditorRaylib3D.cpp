#include "EditorRaylib3D.h"
#include "Editor.h"
#include "NewGizmo.hpp"

#include <Nodes/AllNodes.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

bool AreMatrixEqual(Matrix const& m1, Matrix const& m2)
{
	return (m1.m0 == m2.m0) && (m1.m1 == m2.m1) && (m1.m2 == m2.m2) && (m1.m3 == m2.m3) &&
		(m1.m4 == m2.m4) && (m1.m5 == m2.m5) && (m1.m6 == m2.m6) && (m1.m7 == m2.m7) &&
		(m1.m8 == m2.m8) && (m1.m9 == m2.m9) && (m1.m10 == m2.m10) && (m1.m11 == m2.m11) &&
		(m1.m12 == m2.m12) && (m1.m13 == m2.m13) && (m1.m14 == m2.m14) && (m1.m15 == m2.m15);
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

Node* EditorRaylib3D::FindNode3DWorldMatrix(Node* pNode, Matrix& outMatrix)
{
	Node3D* pNode3D = nullptr; // Node3D
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
		glm::mat4 m1 = pNode3D->GetWorldMatrix();
		outMatrix = {
			m1[0][0], m1[1][0], m1[2][0], m1[3][0],
			m1[0][1], m1[1][1], m1[2][1], m1[3][1],
			m1[0][2], m1[1][2], m1[2][2], m1[3][2],
			m1[0][3], m1[1][3], m1[2][3], m1[3][3]
		};
	}

	return pNode3D;
}

void EditorRaylib3D::UpdateDrawableElement(Node* pNode)
{
	if (dynamic_cast<Node3D*>(pNode) != nullptr) // TestTemp
	{
		std::string name = pNode->GetName();
		Matrix newWorldMatrix = {};
		Node3D* pNode3D = static_cast<Node3D*>(FindNode3DWorldMatrix(pNode, newWorldMatrix)); // Given by Node
		Matrix gMatrix = RayGizmo::GizmoToMatrix(m_loadedMeshs[name].get()->worldTransform); // Given by Gizmo
		if (!AreMatrixEqual(newWorldMatrix, m_loadedMeshs[name].get()->worldMatrix))
		{
			m_loadedMeshs[name].get()->worldMatrix = newWorldMatrix;
			glm::vec3 pos = pNode3D->GetWorldPosition();
			glm::vec3 scale = pNode3D->GetWorldScale();
			glm::quat rot = pNode3D->GetWorldRotationQuat();

			m_loadedMeshs[name]->worldTransform.translation = { pos.x,pos.y,pos.z };
			m_loadedMeshs[name]->worldTransform.scale = { scale.x,scale.y,scale.z };
			m_loadedMeshs[name]->worldTransform.rotation = { rot.x,rot.y,rot.z,rot.w};
		}
		else if (!AreMatrixEqual(gMatrix, m_loadedMeshs[name].get()->worldMatrix))
		{
			m_loadedMeshs[name].get()->worldMatrix = gMatrix;

			pNode3D->SetWorldPosition({ m_loadedMeshs[name]->worldTransform.translation.x,
										m_loadedMeshs[name]->worldTransform.translation.y,
										m_loadedMeshs[name]->worldTransform.translation.z });

			pNode3D->SetWorldScale({ m_loadedMeshs[name]->worldTransform.scale.x,
							m_loadedMeshs[name]->worldTransform.scale.y,
							m_loadedMeshs[name]->worldTransform.scale.z });

			pNode3D->SetWorldRotationQuat({ m_loadedMeshs[name]->worldTransform.rotation.x,
							m_loadedMeshs[name]->worldTransform.rotation.y,
							m_loadedMeshs[name]->worldTransform.rotation.z,
							m_loadedMeshs[name]->worldTransform.rotation.w });
			pNode3D->Update(1 / 16.0f);

			m_gizmoDirty = true;
		}
	}
}

void EditorRaylib3D::RemoveDrawableElement(std::string const& elementName)
{
	 if (m_loadedMeshs.contains(elementName))
	 {
		 UnloadMesh(*m_loadedMeshs[elementName]->mesh.release());
		 m_loadedMeshs.erase(elementName);
	 }
}

void EditorRaylib3D::ClearWindow()
{
	m_loadedMeshs.clear();
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
		m_loadedMeshs[name].get()->mesh = std::make_unique<Mesh>(m_mesh); // GetMesh
		Node3D* pNode3D = static_cast<Node3D*>(FindNode3DWorldMatrix(pNodeMesh3D, m_loadedMeshs[name].get()->worldMatrix));
		if (pNode3D != nullptr)
		{
			glm::vec3 pos = pNode3D->GetWorldPosition();
			glm::vec3 scale = pNode3D->GetWorldScale();
			glm::quat rot = pNode3D->GetWorldRotationQuat();
			m_loadedMeshs[name]->worldTransform.translation = { pos.x,pos.y,pos.z };
			m_loadedMeshs[name]->worldTransform.scale = { scale.x,scale.y,scale.z };
			m_loadedMeshs[name]->worldTransform.rotation = { rot.x,rot.y,rot.z,rot.w };
		}
		else
		{
			m_loadedMeshs[name]->worldTransform = RayGizmo::GizmoIdentity();
		}

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
		if (it->first == m_selectedObject)
		{
			RayGizmo::DrawGizmo3D(RayGizmo::GIZMO_TRANSLATE, &it->second.get()->worldTransform);
		}
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

//void EditorRaylib3D::SetTranslateGizmo(bool state)
//{
//	state ? (m_gizmoFlags &= GizmoFlags::TRANSLATE) : (m_gizmoFlags &= ~(GizmoFlags::TRANSLATE));
//}
//void EditorRaylib3D::SetScaleGizmo(bool state)
//{
//	state ? (m_gizmoFlags &= GizmoFlags::SCALE) : (m_gizmoFlags &= ~(GizmoFlags::SCALE));
//}
//void EditorRaylib3D::SetRotateGizmo(bool state)
//{
//	state ? (m_gizmoFlags &= GizmoFlags::ROTATE) : (m_gizmoFlags &= ~(GizmoFlags::ROTATE));
//}

void EditorRaylib3D::Shutdown()
{
}
