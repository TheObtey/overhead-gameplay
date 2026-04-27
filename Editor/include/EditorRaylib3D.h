#ifndef EDITOR_EDITOR_RAYLIB3D_H__
#define EDITOR_EDITOR_RAYLIB3D_H__

#include <Node.h>
#include <Nodes/Node3D.h>
#include <Nodes/AllNodes.h>
#include <Serialization/json.hpp>
#include <Define.h>
#include <raylib.h>
#include <raymath.h>
#include <unordered_set>
using json = nlohmann::json;

struct CachedTexture
{
	Texture2D texture = {};
	uint32 refCount = 0;
};

struct DrawableSubMesh
{
	uptr<Mesh> mesh;
	Matrix localMatrix = MatrixIdentity();
};

struct DrawableElement
{
	std::vector<DrawableSubMesh> meshes;
	MeshGeometrySourceType geometrySourceType = MeshGeometrySourceType::PRIMITIVE;
	PrimitivesType primitiveType = PrimitivesType::CUBE;
	std::string loadedFbxPath;
	std::string loadedFbxDiffusePath;

	Matrix worldMatrix;
	Transform gizmoTransform;
	bool gizmoUpdated = false;

	Material material = {};
	Texture2D diffuseTexture = {};
	bool hasTexture = false;
	std::string loadedDiffusePath;
};

struct Node3DElement
{
	Matrix worldMatrix; // Raylib Draw
	Transform gizmoTransform; // Gizmo Transform
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
	void AddDrawableObject(Node* pNode);
	void UpdateDrawableElement(Node* pNode);
	void UpdateDrawableTexture(NodeMesh const& nodeMesh, DrawableElement& drawable);
	std::string ResolveEditorTexturePath(std::filesystem::path const& logicalPath);
	void UpdateElementName(std::string const& oldName, Node* pNode);
	void RemoveDrawableElement(Node* pNode);
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

	void SetSelectedNode(Node* pNode) { m_pSelectedObject = pNode; }

	void UpdateDirtyGizmo() { m_gizmoDirty = false; }
	bool IsGizmoDirty() {return m_gizmoDirty;}

private:
	void DrawViewPort();
	void DrawDebugOverlays();
	void DrawCameraFrustumWire(NodeCamera const& cameraNode);
	void DrawBoxColliderWire(NodeBoxCollider const& colliderNode);
	void DrawSphereColliderWire(NodeSphereCollider const& colliderNode);
	void DrawCapsuleColliderWire(NodeCapsuleCollider const& colliderNode);

	void Instanciate3DMesh(Node* nodeMesh3D);
	void InstanciateCollider3D();
	void InstanciateLight();
	Node* FindNode3DWorldMatrix(Node* pNode, Matrix& outMatrix);

	bool AcquireSharedTexture(std::string const& path, Texture2D& outTexture);
	void ReleaseSharedTexture(std::string const& path);

private:
	Camera3D m_camera;
	Camera2D m_cam;
	CameraMode m_cameraMode = CAMERA_CUSTOM;

	RaylibAxis m_currentAxis = X;
	float m_gizmoSize = 1.0f;

	Material m_defaultMaterial;


	std::unordered_map<std::string, CachedTexture> m_sharedTextures;
	std::unordered_map<Node*, uptr<DrawableElement>> m_loadedMeshes;
	std::unordered_map<Node*, uptr<Node3DElement>> m_loadedNode3D;
	Node* m_pSelectedObject = nullptr;

	std::unordered_set<NodeCamera*> m_debugCameras;
	std::unordered_set<NodeBoxCollider*> m_debugBoxColliders;
	std::unordered_set<NodeSphereCollider*> m_debugSphereColliders;
	std::unordered_set<NodeCapsuleCollider*> m_debugCapsuleColliders;

	GizmoFlags m_gizmoFlags;
	bool m_gizmoDirty = false;

	void EnsureDebugPrimitiveModels();
	void ReleaseDebugPrimitiveModels();
	void DrawWireModelWithMatrix(Model const& model, Matrix const& matrix, Color color) const;

	Model m_debugBoxModel = {};
	Model m_debugSphereModel = {};
	Model m_debugCapsuleModel = {};
	bool m_debugPrimitiveModelsReady = false;
};

#endif // __EDITOR_RAYLIB3D__H_