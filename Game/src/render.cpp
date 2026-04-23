#include "EventManager.h"
#include "Passes/GeometryPass.h"
#include "Passes/AnimatedPass.h"
#include "Passes/LightPass.h"
#include "Mesh.h"
#include "Window.h"
#include "Logger.hpp"
#include "Program.h"
#include "Shader.h"
#include "Passes/UIPass.h"
#include "UIElements/Image.h"
#include "UIElements/FTFontFace.h"
#include "UIElements/Text.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace Ore;
int main()
{
    Window window(1920, 1080, "HELLO", false, true);
    window.Open();
    Viewport viewport(0, 0, 1920, 1080, Color::SKY_BLUE);
    window.AddViewport(viewport);

    std::vector<Vertex> vertices;
    vertices.push_back({ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) });
    vertices.push_back({ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) });
    vertices.push_back({ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) });
    vertices.push_back({
        glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) });

    std::vector<uint32> indices = {
        0,1,2,
        0,2,3
    };

    sptr<Geometry> cube     = std::make_shared<Geometry>(vertices, indices);

    sptr<Texture> diffuse   = std::make_shared<Texture>("res/textures/diffuse.jpg", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
    sptr<Texture> specular  = std::make_shared<Texture>("res/textures/defaultSpecular.png", TextureType::TYPE_2D, TextureMaterialType::SPECULAR);
    sptr<Texture> normal    = std::make_shared<Texture>("res/textures/defaultNormal.png", TextureType::TYPE_2D, TextureMaterialType::NORMAL);
    FTFontFace fontFace("res/fonts/Roboto-Medium.ttf", 50);

    sptr<Texture> ui        = std::make_shared<Texture>("res/textures/bib.png", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);

    sptr<Texture> font = std::make_shared<Texture>(fontFace.GetTextureObject(), TextureMaterialType::DIFFUSE);
    std::vector<sptr<Texture>> textures;
    textures.push_back(font);
    textures.push_back(specular);
    textures.push_back(normal);

    Mesh mesh(cube, textures, glm::scale(glm::mat4(1.0f), glm::vec3(1.f)));
    Mesh mesh1(cube, textures, glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    sptr<Camera> camera = std::make_shared<Camera>();
    camera->Perspective.aspectRatio = 1920.0f/1080.0f;
    camera->SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    camera->UpdateCamera();

    std::vector<Light> lights;

    for (int i = 0; i < 1; ++i)
    {
        float xPos = 0.0f;
        float yPos = 0.0f;
        float zPos = -1.0f;
        Light light;
        light.quadratic = 0.5f;
        light.linear = 0.4f;
        light.constant = 0.0f;
        //light.color = Color::BLUE;
        light.position = { xPos, yPos, zPos };
        lights.push_back(light);
    }

    Program geometryProgram;
    Program lightProgram;
    Program UIProgram;

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

    Shader uiFrag(ShaderType::TYPE_FRAGMENT);
    uiFrag.Load("res/shaders/UIPass.frag");
    Shader uiVert(ShaderType::TYPE_VERTEX);
    uiVert.Load("res/shaders/UIPass.vert");

    UIProgram.AddShader(uiFrag);
    UIProgram.AddShader(uiVert);
    UIProgram.Load();

    uiFrag.Unload();
    uiVert.Unload();  


    GeometryPass geoPass(geometryProgram, camera.get());
    LightPass lightPass(lightProgram, lights, camera.get());
    UIPass uiPass(UIProgram, camera.get());


    viewport.AddPass(&geoPass);
    viewport.AddPass(&lightPass);
    viewport.AddPass(&uiPass);

    //Image image(10, 100, 1000, 1000, diffuse);
    Image image1(0, 0, 100, 100, diffuse);
    Image image2(10, 1, 100, 100, ui);
    Image image3(1, 10, 100, 100, ui);

    sptr<FontFace> roboto = std::make_shared<FTFontFace>("res/fonts/FuzzyBubbles-Bold.ttf", 50);
    Text text("The Quick Brown fox jumps over the lazy dog", roboto); 
    text.x = 20;
    text.y = 399;
    text.width = 50;
    text.height = 50;
    text.scale = 1.0f;

    sptr<FontFace> roboto2 = std::make_shared<FTFontFace>("res/fonts/Roboto-Medium.ttf", 18);
    Text text2("Petit text", roboto2);
    text2.x = 200;
    text2.y = 500;
    text2.width = 50;
    text2.height = 50;
    text2.scale = 1.0f;
    text2.color = Color{0.0f, 1.0f, 0.0f, 1.0f};

    while (window.IsOpen())
    {
        window.Clear();
        viewport.Clear();
        //glm::vec3 camPos = camera->GetPosition() + glm::vec3(0.016f,0.0f,0.0f);
        //inf += 0.0016f * fact;
        //glm::mat4 meshTransform = glm::translate(mesh.GetTransform(), glm::vec3(0.0016f * fact, 0.0f, 0.0f));
        //mesh.SetTransform(meshTransform);
        //camera->SetRoll(roll ++);
        //Logger::LogWithLevel(LogLevel::ERROR, yaw);
        //camera->SetPosition(camPos);
        //lights[0].position += glm::vec3(1.0f, 0.0f, 0.0f);

        //geoPass.AddMesh(mesh);
        geoPass.AddMesh(mesh);
        //uiPass.AddUIElement(image);
        //uiPass.AddUIElement(image1);
        //uiPass.AddUIElement(image2);
        //uiPass.AddUIElement(image3);
        uiPass.AddUIElement(text2);
        uiPass.AddUIElement(text);
        viewport.Present();
        window.Present();
    }

    geometryProgram.Unload();
    lightProgram.Unload();

	window.Close();
}

