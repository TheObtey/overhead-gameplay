#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"
#include "Mesh.h"
#include "EventManager.h"
#include "Viewport.h"

#include <iostream>

int main()
{
    Window window(800, 600, "ORE ORE OREORE ORE ORE OREORE OREORE");
    window.Open();
    Viewport viewport(0, 0, 800, 600, Color::SKY_BLUE); 
    //Viewport viewport(0, 0, 800, 600, Color::BLACK); 
    window.AddViewport(viewport);

    std::vector<Vertex> vertices;
    vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});

    // Back face
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f)});

    // Left face
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});

    // Right face
    vertices.push_back({glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)});

    // Top face
    vertices.push_back({glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)});

    // Bottom face
    vertices.push_back({glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)});
    vertices.push_back({glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)});
    vertices.push_back({glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)});

    std::vector<uint32> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9, 10, 10, 11, 8,
        // Right face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    Geometry cube(vertices, indices);
    Texture diffuse("../res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
    Texture specular("../res/textures/specular.jpg", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);

    std::vector<Texture> tex;
    tex.push_back(diffuse); 
    tex.push_back(specular); 

    Mesh mesh(cube, tex, glm::mat4(1.0f));

    glm::vec3 position(0.0f, 0.0f, 5.0f); 
    glm::vec3 up(0.0f, 1.0f, 0.0f);       
    glm::vec3 forward(0.0f, 0.0f, 1.0f); 

    float yaw = 0.0f;   
    float pitch = 0.0f; 
    float roll = 0.0f;  

    float fov = 45.0f; 

    Camera camera(position, up, forward, yaw, pitch, roll, fov);
    std::vector<Mesh> meshes;
    meshes.push_back(mesh);

    Light light;
    std::vector<Light> lights;

    Shader shader("../res/shaders/bin/GBuffer.vert.spv", "../res/shaders/bin/GBuffer.frag.spv");
    Shader shaderLight("../res/shaders/bin/LightPass.vert.spv", "../res/shaders/bin/LightPass.frag.spv");

    shaderLight.Use();
    shaderLight.SetInt("gPosition", 0);
    shaderLight.SetInt("gNormal", 1);
    shaderLight.SetInt("gAlbedoSpec", 2);

    GeometryPass geoPass(shader, meshes, camera);
    LightPass lightPass(shaderLight, lights, camera);

    viewport.AddPass(geoPass);
    viewport.AddPass(lightPass);

	while (window.IsOpen())
	{
        window.Clear();
        
        geoPass.Execute();
        lightPass.Execute();

        window.Present();
	}

	window.Close();
}
