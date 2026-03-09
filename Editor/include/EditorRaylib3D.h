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

class CustomDrawableElement
{
public:
	virtual void Draw() = 0;

private:
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rotation;
};

//class Light : CustomDrawableElement
//{
//	void Draw() override { }
//};


class EditorRaylib3D
{

public:
	EditorRaylib3D();
	~EditorRaylib3D();

	void InitWindow(float const& width, float const& height);
	void LoadDrawableObject(json const& jsonObject);
	void Render();
	void Update(float deltaTime);
	void UpdateJson(json const& newJson);
	void Shutdown();

	void AddDrawableElement(json const& elementToDraw);
	void RemoveDrawableElement(std::string const& elementName);

private:
	void DrawViewPort();
	
	void CheckIfIsDrawable(json element);

	void Draw3DMesh(json& nodeMesh3D);
	void DrawCollider3D();
	void DrawLight();


private:
	// List of Meshs will depends on NodeMesh for vertices later
	
	glm::vec3 m_currentPosition;
	glm::vec3 m_currentScale;
	glm::vec3 m_currentRotation;

	json m_jsonElementFromRoot;
	rl::Camera3D m_camera = {};

	std::map<std::string, uptr<rl::Mesh>> m_loadedMeshs;
};

#endif // __EDITOR_RAYLIB3D__H_