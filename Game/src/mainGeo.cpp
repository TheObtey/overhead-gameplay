#include "EventManager.h"
#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"
#include "Mesh.h"
#include "Window.h"
#include "Program.h"
#include "Shader.h"

#include <GeometryFactory.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

using namespace Ore;

int main()
{
	Window window(1280, 720, "Main Geo", false, true);
	window.Open();

	Viewport viewport(0, 0, 1280, 720, Color::SKY_BLUE);
	window.AddViewport(viewport);

	// GeometryFactory -> Ore::Geometry (API actuelle en shared_ptr)
	const GeoInfo& cubeInfo = GeometryFactory::GetGeometry(PrimitivesType::CUBE);
	const GeoInfo& sphereInfo = GeometryFactory::GetGeometry(PrimitivesType::SPHERE);
	const GeoInfo& cylinderInfo = GeometryFactory::GetGeometry(PrimitivesType::CYLINDER);
	const GeoInfo& capsuleInfo = GeometryFactory::GetGeometry(PrimitivesType::CAPSULE);

	sptr<Geometry> cubeGeo = std::make_shared<Geometry>(cubeInfo.m_vertices, cubeInfo.m_indices);
	sptr<Geometry> sphereGeo = std::make_shared<Geometry>(sphereInfo.m_vertices, sphereInfo.m_indices);
	sptr<Geometry> cylinderGeo = std::make_shared<Geometry>(cylinderInfo.m_vertices, cylinderInfo.m_indices);
	sptr<Geometry> capsuleGeo = std::make_shared<Geometry>(capsuleInfo.m_vertices, capsuleInfo.m_indices);

	// Textures (API actuelle en shared_ptr)
	sptr<Texture> diffuse = std::make_shared<Texture>("res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
	sptr<Texture> specular = std::make_shared<Texture>("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);
	sptr<Texture> normal = std::make_shared<Texture>("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);

	std::vector<sptr<Texture>> textures = { diffuse, specular, normal };

	// Placement
	const glm::mat4 centerTransform = glm::mat4(1.0f);
	const glm::mat4 downTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
	const glm::mat4 rightTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1.8f, 0.0f, 0.0f));
	const glm::mat4 upTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
	const glm::mat4 leftTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.8f, 0.0f, 0.0f));

	Mesh cubeMesh(cubeGeo, textures, centerTransform);
	Mesh sphereMesh(sphereGeo, textures, rightTransform);
	Mesh cylinderMesh(cylinderGeo, textures, upTransform);
	Mesh capsuleMesh(capsuleGeo, textures, leftTransform);
	Mesh cubeMeshDown(cubeGeo, textures, downTransform);

	std::vector<Mesh*> meshes = { &cubeMesh, &sphereMesh, &cylinderMesh, &capsuleMesh, &cubeMeshDown };
	std::vector<glm::mat4> baseTransforms = { centerTransform, rightTransform, upTransform, leftTransform, downTransform };

	std::vector<float> rotationAngles(meshes.size(), 0.0f);
	std::vector<float> rotationSpeeds = {
		glm::radians(45.0f),
		glm::radians(60.0f),
		glm::radians(90.0f),
		glm::radians(30.0f),
		glm::radians(75.0f)
	};

	std::vector<glm::vec3> rotationAxes = {
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::normalize(glm::vec3(0.35f, 1.0f, 0.1f)),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::normalize(glm::vec3(0.2f, 1.0f, 0.5f))
	};

	// Camera API actuelle
	sptr<Camera> camera = std::make_shared<Camera>();
	camera->Perspective.aspectRatio = 1280.0f / 720.0f;
	camera->Perspective.fov = 45.0f;
	camera->SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 6.0f)));
	camera->UpdateCamera();

	// Lumières
	std::vector<Light> lights;
	Light light;
	light.position = { 0.0f, 2.0f, 2.0f };
	light.linear = 0.4f;
	light.quadratic = 0.5f;
	lights.push_back(light);

	// Programmes
	Program geometryProgram;
	Program lightProgram;

	Shader geoFrag(ShaderType::TYPE_FRAGMENT);
	geoFrag.Load("res/shaders/GBuffer.frag");
	Shader geoVert(ShaderType::TYPE_VERTEX);
	geoVert.Load("res/shaders/GBuffer.vert");

	geometryProgram.AddShader(geoFrag);
	geometryProgram.AddShader(geoVert);
	geometryProgram.Load();

	geoFrag.Unload();
	geoVert.Unload();

	Shader lightFrag(ShaderType::TYPE_FRAGMENT);
	lightFrag.Load("res/shaders/LightPass.frag");
	Shader lightVert(ShaderType::TYPE_VERTEX);
	lightVert.Load("res/shaders/LightPass.vert");

	lightProgram.AddShader(lightFrag);
	lightProgram.AddShader(lightVert);
	lightProgram.Load();

	lightFrag.Unload();
	lightVert.Unload();

	lightProgram.Use();
	lightProgram.SetUniform("gPosition", 0);
	lightProgram.SetUniform("gNormal", 1);
	lightProgram.SetUniform("gAlbedoSpec", 2);

	GeometryPass geoPass(geometryProgram, camera.get());
	LightPass lightPass(lightProgram, lights, camera.get());

	viewport.AddPass(&geoPass);
	viewport.AddPass(&lightPass);

	float lastTime = static_cast<float>(glfwGetTime());

	while (window.IsOpen())
	{
		window.Clear();
		viewport.Clear();

		const float currentTime = static_cast<float>(glfwGetTime());
		const float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		for (size_t i = 0; i < meshes.size(); ++i)
		{
			rotationAngles[i] += rotationSpeeds[i] * deltaTime;
			const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationAngles[i], rotationAxes[i]);
			meshes[i]->SetTransform(baseTransforms[i] * rotation);
			geoPass.AddMesh(*meshes[i]);
		}

		viewport.Present();
		window.Present();
	}

	geometryProgram.Unload();
	lightProgram.Unload();
	window.Close();

	return 0;
}

