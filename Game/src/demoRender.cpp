#include "Window.h"
#include "Viewport.h"
#include "Geometry.h"
#include "Color.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"
#include "Program.h"
#include "Shader.h"
#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"

#include <glm/gtc/matrix_transform.hpp>

constexpr uint32 SCR_WIDTH = 900;
constexpr uint32 SCR_HEIGHT = 900;
enum class GeoType
{
    PLANE,
    CUBE
};

sptr<Ore::Geometry> GenerateCube()
{
    Ore::Vertex vertices[8] = 
    {
        {glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},

        {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
    };
    std::vector<uint32> indices = 
    {
        // front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
    };

    sptr<Ore::Geometry> geo = make_shared<Ore::Geometry>(vertices, indices);

    return geo;
}

sptr<Ore::Geometry> GeneratePlane()
{
    std::vector<Ore::Vertex> vertices = 
    {
        {glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        {glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
    };

    std::vector<uint32> indices = 
    {
        0,1,2,
        0,2,3
    };

    sptr<Ore::Geometry> geo = make_shared<Ore::Geometry>(vertices, indices);

    return geo;
}

sptr<Ore::Geometry> GenerateGeometry(GeoType type)
{

    switch(type)
    {
    case GeoType::PLANE:
        return GeneratePlane();
    case GeoType::CUBE:
        return GenerateCube();
        
    }

    return GenerateCube();
}

int main()
{
    Ore::Window window(SCR_WIDTH, SCR_HEIGHT, "Demo Ore", false, true);
    window.Open();
    Ore::Viewport viewport(0, 0, SCR_WIDTH, SCR_HEIGHT, Ore::Color::SKY_BLUE);
    window.AddViewport(viewport);

    sptr<Ore::Geometry> pPlane = GenerateGeometry(GeoType::PLANE);
    sptr<Ore::Geometry> pCube  = GenerateGeometry(GeoType::CUBE);

    sptr<Ore::Texture> pDiffuse   = std::make_shared<Ore::Texture>("res/textures/diffuse.jpg", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE);
    sptr<Ore::Texture> pGreen   = std::make_shared<Ore::Texture>("res/textures/green.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE);
    sptr<Ore::Texture> pWhite   = std::make_shared<Ore::Texture>("res/textures/white.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE);
    sptr<Ore::Texture> pRed   = std::make_shared<Ore::Texture>("res/textures/red.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE);
    sptr<Ore::Texture> pSpecular  = std::make_shared<Ore::Texture>("res/textures/defaultSpecular.jpg", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::SPECULAR);
    sptr<Ore::Texture> pNormal    = std::make_shared<Ore::Texture>("res/textures/defaultNormal.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::NORMAL);

    sptr<Ore::Texture> textures[3] = {pDiffuse, pSpecular, pNormal};
    sptr<Ore::Texture> green[3] = {pGreen, pSpecular, pNormal};
    sptr<Ore::Texture> white[3] = {pWhite, pSpecular, pNormal};
    sptr<Ore::Texture> red[3] = {pRed, pSpecular, pNormal};


    Ore::Mesh BackWall(pPlane, white, glm::mat4(1.0f));
    Ore::Mesh RightWall(pPlane, green, glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.5f)) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Ore::Mesh LeftWall(pPlane, red, glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.5f)) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    Ore::Mesh floor(pPlane, white, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.5f)) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    Ore::Mesh ceiling(pPlane, white, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.5f)) * 
            glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));


    glm::mat4 cubeMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 0.35f, 0.6f));
    cubeMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cubeMatrix = glm::scale(cubeMatrix, glm::vec3(0.30f));

    glm::mat4 longBoxMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.25f, 0.2f));
    longBoxMatrix *= glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    longBoxMatrix = glm::scale(longBoxMatrix, glm::vec3(0.4f, 0.75f, 0.4f));
    
    Ore::Mesh cube(pCube, white, cubeMatrix);
    Ore::Mesh longBox(pCube, white, longBoxMatrix);

    sptr<Ore::Camera> pCamera = std::make_shared<Ore::Camera>(); 
    pCamera->Perspective.aspectRatio = static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT);
    pCamera->Perspective.fov = 30.0f;
    pCamera->SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.5f)));
    pCamera->UpdateCamera();
    
    Ore::Light leftLight;
    leftLight.linear        = 0.2f;
    leftLight.quadratic     = 0.8f;
    leftLight.constant      = 0.0f;
    leftLight.position      = {0.0f, 0.0f, 1.f};
    
    Ore::Light rightLight;
    rightLight.linear       = 0.2f;
    rightLight.quadratic    = 0.8f;
    rightLight.constant     = 0.0f;
    rightLight.position     = {0.0f, 0.0f, 1.f};
    
    std::vector<Ore::Light> lights = {leftLight};
    
    Ore::Program geometryProgram;
    Ore::Program lightProgram;
    
    Ore::Shader geoFrag(Ore::ShaderType::TYPE_FRAGMENT);
    Ore::Shader geoVert(Ore::ShaderType::TYPE_VERTEX);
    geoFrag.Load("res/shaders/GBuffer.frag");
    geoVert.Load("res/shaders/GBuffer.vert");

    geometryProgram.AddShader(geoFrag);
    geometryProgram.AddShader(geoVert);
    geometryProgram.Load();

    geoFrag.Unload();
    geoVert.Unload();

    Ore::Shader lightFrag(Ore::ShaderType::TYPE_FRAGMENT);
    Ore::Shader lightVert(Ore::ShaderType::TYPE_VERTEX);
    lightFrag.Load("res/shaders/LightPass.frag");
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

    Ore::GeometryPass geoPass(geometryProgram, pCamera.get());
    Ore::LightPass lightPass(lightProgram, lights, pCamera.get());

    viewport.AddPass(&geoPass);
    viewport.AddPass(&lightPass);

    while(window.IsOpen())
    {
        window.Clear();
        viewport.Clear();
        
        //box.SetTransform(glm::rotate(box.GetTransform(), 0.016f, glm::vec3(0.0f,1.0f,0.0f)));
        geoPass.AddMesh(BackWall);
        geoPass.AddMesh(RightWall);
        geoPass.AddMesh(LeftWall);
        geoPass.AddMesh(floor);
        geoPass.AddMesh(ceiling);

        geoPass.AddMesh(cube);
        geoPass.AddMesh(longBox);

        viewport.Present();
        window.Present();
    }
    
    geometryProgram.Unload();
    lightProgram.Unload();

    window.Close();
}
