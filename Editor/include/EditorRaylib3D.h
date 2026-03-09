#ifndef __EDITOR_RAYLIB3D__H_
#define __EDITOR_RAYLIB3D__H_

#include <Serialization/json.hpp>

namespace rl
{
#include <raylib.h>
#include <raymath.h>
#include <raygizmo.h>
}

#include <Node.h>
#include <Define.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

using json = nlohmann::json;

struct DrawableElement
{
	uptr<rl::Mesh> mesh;
	glm::mat4 worldMatrix;
};

class EditorRaylib3D
{

public:
	EditorRaylib3D();
	~EditorRaylib3D();

	void InitWindow(float const& width, float const& height);
	void Render();
	void Update(float deltaTime);
	void Shutdown();

	void AddDrawableObject(std::string const& name,Node* jsonObject);
	void UpdateDrawableElement(std::string const& name, Node const* jsonObject);
	void RemoveDrawableElement(std::string const& elementName);

private:
	void DrawViewPort();
	
	void CheckIfIsDrawable(json element);

	void Instanciate3DMesh(std::string const& name, Node* nodeMesh3D);
	void InstanciateCollider3D();
	void InstanciateLight();


private:
	// List of Meshs will depends on NodeMesh for vertices later
	
	glm::vec3 m_currentPosition;
	glm::vec3 m_currentScale;
	glm::vec3 m_currentRotation;

	json m_jsonElementFromRoot;
	rl::Camera3D m_camera;
	rl::Material m_defaultMaterial;

	std::map<std::string, uptr<DrawableElement>> m_loadedMeshs;
	std::map<std::string, uptr<DrawableElement>> m_colliders;
	std::map<std::string, uptr<DrawableElement>> m_light;
};

#endif // __EDITOR_RAYLIB3D__H_