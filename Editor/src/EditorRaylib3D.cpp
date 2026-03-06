#include "EditorRaylib3D.h"


EditorRaylib3D::EditorRaylib3D(){}
EditorRaylib3D::~EditorRaylib3D(){}

void EditorRaylib3D::Init()
{
	// Static Cam 
	m_camera.position = { 10.0f, 10.0f, 10.0f };
	m_camera.target = { 0.0f, 0.0f, 0.0f };
	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 45.0f;
	m_camera.projection = CAMERA_PERSPECTIVE;
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

void EditorRaylib3D::Render()
{
	BeginMode3D(m_camera);
	DrawViewPort();

	json nFinder = m_jsonElementFromRoot["Root"];
	
	CheckIfIsDrawable(nFinder);

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

void EditorRaylib3D::CheckIfIsDrawable(json element)
{
	if (element["TYPE"] == "class Node3D")
	{
		for (uint32 i = 0; i < element["Children"].size(); i++)
		{
			// CreateWorldMatrix From Values
			CheckIfIsDrawable(element["Children"][i]);
		}
		break;
	}
	else if (element["TYPE"] == "class NodeLight")
	{
		DrawLight();
	}
	else if (element["TYPE"] == "class NodeMesh3D")
	{
		Draw3DMesh();
	}
	else if (element["TYPE"] == "class NodeCollider3D")
	{
		DrawCollider3D(m_loadedMeshs[element["DATAS"]["m_name"]]);
	}

	for (uint32 i = 0; i < element["Children"].size(); i++)
	{
		CheckIfIsDrawable(element["Children"][i]);
	}
}

void EditorRaylib3D::Draw3DMesh(Mesh& mesh)
{
}

void EditorRaylib3D::DrawCollider3D()
{
}

void EditorRaylib3D::DrawLight()
{
}


void EditorRaylib3D::Shutdown()
{
}