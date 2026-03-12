#ifndef EDITOR_EDITOR_RAYLIB3D_H__
#define EDITOR_EDITOR_RAYLIB3D_H__

#include <Serialization/json.hpp>
#include <Define.h>
#include <raylib.h>

class Node;
class Node3D;

using json = nlohmann::json;

struct DrawableElement
{
	uptr<Mesh> mesh;
	Matrix worldMatrix;
	Transform worldTransform;
};

enum class GizmoFlags
{
	TRANSLATE,
	SCALE,
	ROTATE

#define ENUM_CLASS_FLAGS(GizmoFlags)
};     


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

	void AddDrawableObject(std::string const& name,Node* jsonObject);
	void UpdateDrawableElement(Node* pNode);
	void RemoveDrawableElement(std::string const& elementName);
	void ClearWindow();

	void SetTranslateGizmo(bool state);
	void SetScaleGizmo(bool state);
	void SetRotateGizmo(bool state);

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
	Material m_defaultMaterial;

	std::map<std::string, uptr<DrawableElement>> m_loadedMeshs;

	std::string m_selectedObject;

	GizmoFlags m_gizmoFlags;
	bool m_gizmoDirty = false;
};

#endif // __EDITOR_RAYLIB3D__H_