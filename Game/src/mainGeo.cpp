#include "EventManager.h"
#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"
#include "Mesh.h"
#include "Window.h"
#include "Logger.hpp"
#include "Program.h"
#include "Shader.h"

#include <GeometryFactory.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


int main()
{
    Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
    window.Open();
    Viewport viewport(0, 0, 800, 600, {1,1,1, 1.0f});
    //Viewport viewport(0, 0, 800, 600, Color::BLACK); 
    window.AddViewport(viewport);


    const GeoInfo& tmpcube = GeometryFactory::GetGeometry(PrimitivesType::CUBE);
    const GeoInfo& tmpsphere = GeometryFactory::GetGeometry(PrimitivesType::SPHERE);
    const GeoInfo& tmpcylinder = GeometryFactory::GetGeometry(PrimitivesType::CYLINDER);
    const GeoInfo& tmpcapsule = GeometryFactory::GetGeometry(PrimitivesType::CAPSULE);

    Geometry cube(tmpcube.m_vertices, tmpcube.m_indices);
    Geometry sphere(tmpsphere.m_vertices, tmpsphere.m_indices);
    Geometry cylinder(tmpcylinder.m_vertices, tmpcylinder.m_indices);
    Geometry capsule(tmpcapsule.m_vertices, tmpcapsule.m_indices);
    
    Texture diffuse("res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
    Texture specular("res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);
    Texture normal("res/textures/NormalMap.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);

    std::vector<Texture*> textures;
    textures.push_back(&diffuse);
    textures.push_back(&specular);
    textures.push_back(&normal);

    const glm::mat4 centerTransform = glm::mat4(1.0f);
    const glm::mat4 downTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    const glm::mat4 rightTransform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 upTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 leftTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

    Mesh mesh(cube, textures, glm::mat4(1.0f));
    Mesh mesh1(cube, textures, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    Mesh mesh2(sphere, textures, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    Mesh mesh3(cylinder, textures, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Mesh mesh4(capsule, textures, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 0.0f)));
    
    glm::vec3 position(0.0f, 0.0f, 5.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float fov = 45.0f;

    sptr<Camera> camera = std::make_shared<Camera>(position, up, yaw, pitch, roll, fov);
    std::vector<Mesh*> meshes;
    meshes.push_back(&mesh);
    meshes.push_back(&mesh2);
    meshes.push_back(&mesh3);
    meshes.push_back(&mesh4);
    meshes.push_back(&mesh1);
 
    std::vector<glm::mat4> baseTransforms;
    baseTransforms.reserve(meshes.size());
    baseTransforms.push_back(centerTransform);
    baseTransforms.push_back(rightTransform);
    baseTransforms.push_back(upTransform);
    baseTransforms.push_back(leftTransform);
    baseTransforms.push_back(downTransform);

    std::vector<float> rotationAngles(meshes.size(), 0.0f);
    std::vector<float> rotationSpeeds = {
        glm::radians(45.0f),
        glm::radians(60.0f),
        glm::radians(90.0f),
        glm::radians(30.0f),
        glm::radians(75.0f)
    };
    rotationSpeeds.resize(meshes.size(), glm::radians(45.0f));

    std::vector<glm::vec3> rotationAxes = {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::normalize(glm::vec3(0.35f, 1.0f, 0.1f)),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::normalize(glm::vec3(0.2f, 1.0f, 0.5f))
    };
    rotationAxes.resize(meshes.size(), glm::vec3(0.0f, 1.0f, 0.0f));


    std::vector<Light> lights;

    for (int i = 0; i < 10; ++i)
    {
        float xPos = 0.0f;
        float yPos = 0.0f;
        float zPos = 1.0f;
        Light light;
        //light.color = Color::BLUE;
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
    //lightProgram.SetUniform("gPosition", 0);
    //lightProgram.SetUniform("gNormal", 1);
    //lightProgram.SetUniform("gAlbedoSpec", 2);

    GeometryPass geoPass(geometryProgram, meshes, camera);
    LightPass lightPass(lightProgram, lights, camera);

    viewport.AddPass(&geoPass);
    viewport.AddPass(&lightPass);

    float lastTime = static_cast<float>(glfwGetTime());
 
     while (window.IsOpen())
     {
         window.Clear();
 
        const float currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        for (size_t i = 0; i < meshes.size(); ++i)
        {
            rotationAngles[i] += rotationSpeeds[i] * deltaTime;
            const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationAngles[i], rotationAxes[i]);
            meshes[i]->SetTransform(baseTransforms[i] * rotation);
        }
 
         window.Present();
     }
    geometryProgram.Unload();
    lightProgram.Unload();

    window.Close();
}

