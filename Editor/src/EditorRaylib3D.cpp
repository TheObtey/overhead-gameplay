#include "EditorRaylib3D.h"
#include "Editor.h"
#include "NewGizmo.hpp"

#include <Nodes/AllNodes.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <cmath>

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
		Node3D* pNode3D = static_cast<Node3D*>(FindNode3DWorldMatrix(pNode, newWorldMatrix)); // Matrix given by Inspector
		if (!AreMatrixEqual(newWorldMatrix, m_loadedMeshs[name].get()->worldMatrix))
		{
			m_loadedMeshs[name]->gizmoTransform.translation = Vector3{ pNode3D->GetWorldPosition().x,pNode3D->GetWorldPosition().y,pNode3D->GetWorldPosition().z};
			m_loadedMeshs[name]->gizmoTransform.scale = Vector3{ pNode3D->GetWorldScale().x,pNode3D->GetWorldScale().y,pNode3D->GetWorldScale().z};
			m_loadedMeshs[name]->gizmoTransform.rotation = Quaternion{ pNode3D->GetWorldRotationQuaternion().x,pNode3D->GetWorldRotationQuaternion().y,pNode3D->GetWorldRotationQuaternion().z,pNode3D->GetWorldRotationQuaternion().w };
		}
		else if (m_loadedMeshs[name]->gizmoUpdated)
		{
			m_loadedMeshs[name]->gizmoUpdated = false;
			pNode3D->SetWorldPosition({ m_loadedMeshs[name]->gizmoTransform.translation.x,m_loadedMeshs[name]->gizmoTransform.translation.y,m_loadedMeshs[name]->gizmoTransform.translation.z });
			pNode3D->SetWorldScale({ m_loadedMeshs[name]->gizmoTransform.scale.x,m_loadedMeshs[name]->gizmoTransform.scale.y,m_loadedMeshs[name]->gizmoTransform.scale.z });
			pNode3D->SetWorldRotationQuaternion(glm::quat{ m_loadedMeshs[name]->gizmoTransform.rotation.w,m_loadedMeshs[name]->gizmoTransform.rotation.x,m_loadedMeshs[name]->gizmoTransform.rotation.y,m_loadedMeshs[name]->gizmoTransform.rotation.z });
			m_gizmoDirty = true;
		}
		m_loadedMeshs[name]->worldMatrix = GlmToMatrix(pNode3D->GetWorldMatrix());
	}
}

void EditorRaylib3D::UpdateElementName(std::string const& oldName, Node* pNode)
{
	if (!m_loadedMeshs.contains(oldName)) return;

	AddDrawableObject(pNode->GetName(), pNode);
	m_loadedMeshs.erase(oldName);
	m_selectedObject = pNode->GetName();
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
		m_loadedMeshs[name]->gizmoTransform = RayGizmo::GizmoIdentity();
		m_loadedMeshs[name]->mesh = std::make_unique<Mesh>(m_mesh); // GetMesh
		m_loadedMeshs[name]->worldMatrix = {}; // GetMesh

		Node3D* pNode3D = static_cast<Node3D*>(FindNode3DWorldMatrix(pNodeMesh3D, m_loadedMeshs[name].get()->worldMatrix));
		if (pNode3D != nullptr)
		{
			m_loadedMeshs[name]->gizmoTransform.translation = Vector3{ pNode3D->GetWorldPosition().x,pNode3D->GetWorldPosition().y,pNode3D->GetWorldPosition().z };
			m_loadedMeshs[name]->gizmoTransform.scale = Vector3{ pNode3D->GetWorldScale().x,pNode3D->GetWorldScale().y,pNode3D->GetWorldScale().z };
			m_loadedMeshs[name]->gizmoTransform.rotation = Quaternion{ pNode3D->GetWorldRotationQuaternion().x,pNode3D->GetWorldRotationQuaternion().y,pNode3D->GetWorldRotationQuaternion().z,pNode3D->GetWorldRotationQuaternion().w };
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
	//UpdateCamera(&m_camera, m_cameraMode);
	BeginMode3D(m_camera);
	DrawViewPort();

	for (std::map<std::string, uptr<DrawableElement>>::iterator it  = m_loadedMeshs.begin(); it != m_loadedMeshs.end(); it++)
	{	
		DrawMesh(*it->second->mesh.get(), m_defaultMaterial, it->second->worldMatrix);
		if (it->first == m_selectedObject)
		{
			RayGizmo::SetGizmoSize(m_gizmoSize);
			if(RayGizmo::DrawGizmo3D(static_cast<int>(m_gizmoFlags), &it->second->gizmoTransform))
			{
				m_loadedMeshs[it->first]->gizmoUpdated = true;
			}
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
