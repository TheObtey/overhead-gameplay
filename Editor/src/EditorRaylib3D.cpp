#include "EditorRaylib3D.h"


EditorRaylib3D::EditorRaylib3D(){}
EditorRaylib3D::~EditorRaylib3D(){}

void EditorRaylib3D::InitWindow(float const& width, float const& height)
{
	rl::SetConfigFlags(rl::FLAG_MSAA_4X_HINT | rl::FLAG_VSYNC_HINT | rl::FLAG_WINDOW_RESIZABLE);
	rl::InitWindow(width, height, "Foundry Editor");
	rl::SetTargetFPS(144);

	// Static Cam 
	m_camera.position = { 10.0f, 10.0f, 10.0f };
	m_camera.target = { 0.0f, 0.0f, 0.0f };
	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 45.0f;
	m_camera.projection = rl::CAMERA_PERSPECTIVE;

	m_defaultMaterial = rl::LoadMaterialDefault();
}

void EditorRaylib3D::Update(float deltaTime)
{
	if (rl::IsMouseButtonDown(rl::MOUSE_BUTTON_RIGHT)) {
		UpdateCamera(&m_camera, rl::CAMERA_FREE);
	}
	if (rl::IsMouseButtonDown(rl::MOUSE_BUTTON_MIDDLE)) {
		UpdateCamera(&m_camera, rl::CAMERA_ORBITAL);
	}
}

void EditorRaylib3D::AddDrawableObject(std::string const& name, Node* pNode)
{
	if (dynamic_cast<Node*>(pNode) != nullptr) // TestTemp
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

void EditorRaylib3D::UpdateDrawableElement(std::string const& name, Node const* jsonObject)
{
}

void EditorRaylib3D::Instanciate3DMesh(std::string const& name, Node* pNodeMesh3D) // NodeMesh3D
{
	if (m_loadedMeshs.find(name) != m_loadedMeshs.end())
	{
		// ERROR
	}
	else
	{
		rl::Mesh m_mesh = rl::GenMeshCube(1, 1, 1);
		// Custom Mesh with Mesh3D
		rl::UploadMesh(&m_mesh, false);
		m_loadedMeshs[name] = std::make_unique<DrawableElement>();
		m_loadedMeshs[name].get()->mesh = std::make_unique<rl::Mesh>(m_mesh); // GetMesh;..
		Node* pNode3DParent = nullptr; // Node3D
		glm::mat4 worldMatrix = { 1.0f };
		while (pNode3DParent == nullptr)
		{
			Node* pParent = pNodeMesh3D->GetParent();
			pNode3DParent = pParent;

			//pNode3DParent = dynamic_cast<Node3D*>(pParent);
			if (pParent == nullptr) break;
			//if (pNode3DParent != nullptr) worldMatrix = pNode3DParent.GetWorldMatrix;
		}
		m_loadedMeshs[name].get()->worldMatrix = worldMatrix;
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
	rl::BeginMode3D(m_camera);
	DrawViewPort();

	for (std::map<std::string, uptr<DrawableElement>>::iterator it  = m_loadedMeshs.begin(); it != m_loadedMeshs.end(); it++)
	{
		rl::Matrix matrix = {};
		glm::mat4 mat = it->second.get()->worldMatrix;
		memcpy_s(&matrix.m0, sizeof(rl::Matrix), &mat[0][0], sizeof(glm::mat4));
		rl::DrawMesh(*it->second.get()->mesh.get(), m_defaultMaterial, matrix);
	}

	rl::EndMode3D();
}

void EditorRaylib3D::DrawViewPort()
{
	rl::DrawGrid(20, 1.0f);
	rl::DrawLine3D({ 0, 0, 0 }, { 500, 0, 0 }, rl::RED);
	rl::DrawLine3D({ 0, 0, 0 }, { 0, 500, 0 }, rl::GREEN);
	rl::DrawLine3D({ 0, 0, 0 }, { 0, 0, 500 }, rl::BLUE);
	rl::DrawLine3D({ 0, 0, 0 }, { -500, 0, 0 }, rl::RED);
	rl::DrawLine3D({ 0, 0, 0 }, { 0, -500, 0 }, rl::GREEN);
	rl::DrawLine3D({ 0, 0, 0 }, { 0, 0,-500 }, rl::BLUE);
}


void EditorRaylib3D::Shutdown()
{
}
