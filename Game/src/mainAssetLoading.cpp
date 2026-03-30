////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include <AssetLoading/AssetLoader.h>
#include <Window.h>
#include <Program.h>
#include <Shader.h>
#include <Passes/GeometryPass.h>
#include <Passes/LightPass.h>

namespace rl
{
	#include "raylib.h"
}

int main()
{
	Window window(1920, 1080, "ORE ORE OREORE ORE ORE OREORE OREORE");
	window.Open();
	Viewport viewport(0, 0, 1920, 1080, Color::SKY_BLUE);
	window.AddViewport(viewport);

    glm::vec3 position(0.0f, 0.0f, -5.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    float yaw = 90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float fov = 45.0f;

    sptr<Camera> camera = std::make_shared<Camera>(position, up, yaw, pitch, roll, fov);
    sptr<SceneData> Scene1 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_3.fbx", AssetLoader::FileType::FBX);
    sptr<SceneData> Scene2 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim.fbx", AssetLoader::FileType::FBX);
    std::vector<Mesh*> meshes = {};

    for (uint32 i = 0; i < Scene1->meshes.size(); ++i)
    {
        meshes.push_back(Scene1->meshes[i].mesh.get());
    }


    std::vector<Light> lights;
    for (int i = 0; i < 20; ++i)
    {
        float xPos = 0.0f;
        float yPos = 0.0f;
        float zPos = -5.0f;
        Light light;
        light.position = { xPos, yPos, zPos };
        lights.push_back(light);
    }

    Program geometryProgram;
    Program lightProgram;

    Shader geoFrag(ShaderType::TYPE_FRAGMENT);
    geoFrag.Load("res/shaders/GBuffer.frag");
    Shader geoVert(ShaderType::TYPE_VERTEX);
    geoVert.Load("res/shaders/GBuffer.vert");

    geometryProgram.AddShader(&geoFrag);
    geometryProgram.AddShader(&geoVert);
    geometryProgram.Load();

    geoFrag.Unload();
    geoVert.Unload();


    Shader lightFrag(ShaderType::TYPE_FRAGMENT);
    lightFrag.Load("res/shaders/LightPass.frag");
    Shader lightVert(ShaderType::TYPE_VERTEX);
    lightVert.Load("res/shaders/LightPass.vert");

    lightProgram.AddShader(&lightFrag);
    lightProgram.AddShader(&lightVert);
    lightProgram.Load();

    lightFrag.Unload();
    lightVert.Unload();

    lightProgram.Use();

    GeometryPass geoPass(geometryProgram, meshes, camera);
    LightPass lightPass(lightProgram, lights, camera);

    viewport.AddPass(&geoPass);
    viewport.AddPass(&lightPass);

	while(true)
    {
        window.Clear();

        window.Present();
    }

    geometryProgram.Unload();
    lightProgram.Unload();

    window.Close();
}