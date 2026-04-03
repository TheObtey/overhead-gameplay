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
#include <Passes/AnimatedPass.h>
#include <Logger.hpp>
#include <Nodes/NodeMeshAnimated3D.h>
#include <Nodes/Node3D.h>
#include <Servers/AnimationServer.h>

namespace rl
{
	#include "raylib.h"
}

int main()
{
	Window window(1920, 1080, "ORE ORE OREORE ORE ORE OREORE OREORE", false, true);
	window.Open();
	Viewport viewport(0, 0, 1920, 1080, Color::SKY_BLUE);
	window.AddViewport(viewport);

    //sptr<SceneData> Scene1 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_3.fbx", AssetLoader::FileType::FBX);
    sptr<SceneData> Scene4 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim_2.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene2 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Anim.fbx", AssetLoader::FileType::FBX);
    //sptr<SceneData> Scene3 = AssetLoader::LoadSceneFromFile("res/fbx/Test_Bones.fbx", AssetLoader::FileType::FBX);
    
    std::vector<Vertex> vertices;
    vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) });
    vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back({ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) });

    std::vector<uint32> indices = {
        0,1,2,
        0,2,3
    };

    Geometry cube(vertices, indices);
    Texture diffuse("res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
    Texture specular("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);
    Texture normal("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);

    std::vector<Texture*> textures;
    textures.push_back(&diffuse);
    textures.push_back(&specular);
    textures.push_back(&normal);
    Mesh mesh(cube, textures, glm::scale(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 10.0f)));

    glm::vec3 position(-10.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float fov = 45.0f;

    sptr<Camera> camera = std::make_shared<Camera>(position, up, yaw, pitch, roll, fov);

    std::vector<Light> lights;
    for (int i = 0; i < 20; ++i)
    {
        float xPos = -10.0f + i;
        float yPos = 0.0f;
        float zPos = 5.0f;
        Light light;
        light.position = { xPos, yPos, zPos };
        //light.quadratic = 5.0f;
        //light.constant = 5.0f;
        //light.linear = 5.0f;
        lights.push_back(light);
    }

    Program geometryProgram;
    Program lightProgram;
    Program animateProgram;

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

    Shader animFrag(ShaderType::TYPE_FRAGMENT);
    animFrag.Load("res/shaders/GBuffer.frag");
    Shader animVert(ShaderType::TYPE_VERTEX);
    animVert.Load("res/shaders/Animated.vert");

    animateProgram.AddShader(&animFrag);
    animateProgram.AddShader(&animVert);
    animateProgram.Load();

    animFrag.Unload();
    animVert.Unload();

    //mesh = *Scene4->allMesh[0];

    //GeometryPass geoPass(geometryProgram, camera);
    AnimatedPass animPass(animateProgram, camera);
    LightPass lightPass(lightProgram, lights, camera);


    AnimationServer::InitAnimationPass(&animPass);

    uptr<Node> pNode = Node::CreateNode<NodeMeshAnimated3D>("AA");
    NodeMeshAnimated3D* pMesh = dynamic_cast<NodeMeshAnimated3D*>(pNode.get());
    Node3D* p3D = dynamic_cast<Node3D*>(pNode.get());
    pMesh->SetMesh(*Scene4->allMesh[0]);
    pMesh->SetAnimation(*Scene4->animations[0]);
    //pNode3D->AddChild(pNode);
    //p3D->SetWorldPosition({ 0.0f,0.0f,0.0f });


    //viewport.AddPass(&geoPass);
    viewport.AddPass(&animPass);
    viewport.AddPass(&lightPass);
    pMesh->PlayAnimation("Test Anim 2",true);
	
    while(window.IsOpen())
    {
        window.Clear();
        pNode->Update(0.16f);
        //p3D->AddLocalRotation({ 0.0f,0.016f,0.0f });
        AnimationServer::FlushCommands();
        window.Present();
    }

    geometryProgram.Unload();
    lightProgram.Unload();

    window.Close();
}