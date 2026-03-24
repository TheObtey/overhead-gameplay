#include "EventManager.h"
#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"
#include "Mesh.h"
#include "Window.h"
#include "Logger.hpp"

#include <glm/gtc/matrix_transform.hpp>


int main()
{
    Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
    window.Open();
    Viewport viewport(0, 0, 800, 600, Color::SKY_BLUE);
    //Viewport viewport(0, 0, 800, 600, Color::BLACK); 
    window.AddViewport(viewport);

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

    Mesh mesh(cube, textures, glm::mat4(1.0f));
    Mesh mesh1(cube, textures, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    glm::vec3 position(0.0f, 0.0f, 5.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float fov = 45.0f;

    sptr<Camera> camera = std::make_shared<Camera>(position, up, yaw, pitch, roll, fov);
    std::vector<Mesh*> meshes;
    meshes.push_back(&mesh);
    meshes.push_back(&mesh1);

    std::vector<Light> lights;

    for (int i = 0; i < 1; ++i)
    {
        float xPos = 0.0f;
        float yPos = 0.0f;
        float zPos = 1.0f;
        Light light;
        //light.color = Color::BLUE;
        light.position = { xPos, yPos, zPos };
        lights.push_back(light);
    }

    Shader shader("res/shaders/GBuffer.vert", "res/shaders/GBuffer.frag");
    Shader shaderLight("res/shaders/LightPass.vert", "res/shaders/LightPass.frag");

    shaderLight.Use();
    shaderLight.SetInt("gPosition", 0);
    shaderLight.SetInt("gNormal", 1);
    shaderLight.SetInt("gAlbedoSpec", 2);

    GeometryPass geoPass(shader, meshes, camera);
    LightPass lightPass(shaderLight, lights, camera);

    viewport.AddPass(&geoPass);
    viewport.AddPass(&lightPass);

    while (window.IsOpen())
    {
        window.Clear();

        //glm::vec3 camPos = camera->GetPosition() + glm::vec3(0.016f,0.0f,0.0f);
        //glm::mat4 meshTransform = glm::translate(mesh.GetTransform(), glm::vec3(0.0016f, 0.0f, 0.0f));
        //mesh.SetTransform(meshTransform);
        //camera->SetRoll(roll ++);
        //Logger::LogWithLevel(LogLevel::ERROR, yaw);
        //camera->SetPosition(camPos);

        window.Present();
    }

    window.Close();
}