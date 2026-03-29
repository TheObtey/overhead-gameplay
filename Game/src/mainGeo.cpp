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


int main()
{
    Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
    window.Open();
    Viewport viewport(0, 0, 800, 600, {1,1,1, 1.0f});
    //Viewport viewport(0, 0, 800, 600, Color::BLACK); 
    window.AddViewport(viewport);


    const GeoInfo& tmpcube = GeometryFactory::GetGeometry(PrimitivesType::SPHERE);
    const GeoInfo& tmpcube1 = GeometryFactory::GetGeometry(PrimitivesType::SPHERE);
    const GeoInfo& tmpcube2 = GeometryFactory::GetGeometry(PrimitivesType::SPHERE);
    const GeoInfo& tmpcube3 = GeometryFactory::GetGeometry(PrimitivesType::SPHERE);

    Geometry cube(tmpcube.m_vertices, tmpcube.m_indices);
    Geometry cube1(tmpcube1.m_vertices, tmpcube1.m_indices);
    Geometry cube2(tmpcube2.m_vertices, tmpcube2.m_indices);
    Geometry cube3(tmpcube3.m_vertices, tmpcube3.m_indices);
    
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
    Mesh mesh2(cube1, textures, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    Mesh mesh3(cube2, textures, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Mesh mesh4(cube3, textures, glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
    
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

    float rotationAngle = 0.0f;
    const glm::vec3 rotationAxis = glm::normalize(glm::vec3(0.45f, 1.0f, 0.2f));
    constexpr float rotationSpeed = 0.01f;

    while (window.IsOpen())
    {
        window.Clear();

        rotationAngle += rotationSpeed;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), rotationAngle, rotationAxis);
        for (size_t i = 0; i < meshes.size(); ++i)
        {
            meshes[i]->SetTransform(rotation * baseTransforms[i]);
        }

        window.Present();
    }
    geometryProgram.Unload();
    lightProgram.Unload();

    window.Close();
}

