#ifndef EDITOR_EDITOR_RAYLIB3D_H__
#define EDITOR_EDITOR_RAYLIB3D_H__

#include <Node.h>
#include <Nodes/Node3D.h>
#include <Nodes/AllNodes.h>
#include <Serialization/json.hpp>
#include <Define.h>
#include <raylib.h>

using json = nlohmann::json;

struct DrawableElement
{
	uptr<Mesh> mesh;
	Matrix worldMatrix; // Raylib Draw
	Transform gizmoTransform; // Gizmo Transform
	PrimitivesType primitiveType = PrimitivesType::CUBE;
	bool gizmoUpdated = false;
};
enum class GizmoFlags
{
	NONE = 0,
	TRANSLATE = 1 << 0,
	ROTATE = 1 << 1,
	SCALE = 1 << 2,
};

ENUM_CLASS_FLAGS(GizmoFlags);

class EditorRaylib3D
{

public:
	EditorRaylib3D();
	~EditorRaylib3D();

	void Init(float const& width, float const& height);
	void Render();
	void Update(float deltaTime);
	void UpdateDisplay(Node* pNode);
	void Shutdown();

	void AddDrawableObject(std::string const& name, Node* pNode);
	void UpdateDrawableElement(Node* pNode);
	void UpdateElementName(std::string const& oldName,Node* pNode);
	void RemoveDrawableElement(std::string const& elementName);
	void ClearWindow();

	void SetTranslateGizmo(bool state);
	void SetScaleGizmo(bool state);
	void SetRotateGizmo(bool state);

	enum RaylibAxis
	{
		X,Y,Z
	};

	void SetCameraOnAxis(RaylibAxis axis);

	enum CameraState{ FREE, ORTHOGRAPHIC, ORBITAL, PERSPECTIVE };
	void ChangeCamera(CameraState state);

	bool IsGizmoTranslate() { return Any(m_gizmoFlags & GizmoFlags::TRANSLATE); }
	bool IsGizmoScale() { return Any(m_gizmoFlags & GizmoFlags::SCALE); }
	bool IsGizmoRotate() { return Any(m_gizmoFlags & GizmoFlags::ROTATE); }

	void SetSelectedNode(std::string const& name) { m_selectedObject = name; }

	void UpdateDirtyGizmo() { m_gizmoDirty = false; }
	bool IsGizmoDirty() {return m_gizmoDirty;}


private:
	void DrawViewPort();
	
	void Instanciate3DMesh(std::string const& name, Node* nodeMesh3D);
	void InstanciateCollider3D();
	void InstanciateLight();
	Node* FindNode3DWorldMatrix(Node* pNode, Matrix& outMatrix);
private:
	// List of Meshs will depends on NodeMesh for vertices later
	Camera3D m_camera;
	Camera2D m_cam;
	CameraMode m_cameraMode = CAMERA_CUSTOM;

	RaylibAxis m_currentAxis = X;
	float m_gizmoSize = 1.0f;

	Material m_defaultMaterial;

	std::map<std::string, uptr<DrawableElement>> m_loadedMeshs;

	std::string m_selectedObject;

	GizmoFlags m_gizmoFlags;
	bool m_gizmoDirty = false;
};

#endif // __EDITOR_RAYLIB3D__H_