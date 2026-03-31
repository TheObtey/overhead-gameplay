#include "Servers/GraphicServer.h"

#include "GeometryFactory.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeWindow.h"
#include "Nodes/NodeViewport.h"

void GraphicServer::OpenWindow(NodeWindow* pWindow)
{
    Instance().m_commands.push({CommandType::OPENWINDOW, pWindow});
}

void GraphicServer::Present(NodeWindow* pWindow)
{
    Instance().m_commands.push({CommandType::PRESENT, pWindow});
}

void GraphicServer::Clear(NodeWindow* pWindow)
{
    Instance().m_commands.push({CommandType::CLEAR, pWindow});
}

void GraphicServer::AttachToWindow(NodeViewport *pViewport, NodeWindow *pWindow)
{
    Instance().m_commands.push({CommandType::ATTACHVIEWPORT, pWindow, pViewport});
}

void GraphicServer::AttachToViewport(NodeCamera *pCamera, NodeViewport *pViewport)
{
    Instance().m_commands.push({CommandType::ATTACHCAMERA, nullptr, pViewport, pCamera});
}

void GraphicServer::LoadShaderPrograms(NodeViewport* pViewport)
{
    Command<GraphicServer> cmd;
    cmd.Type = CommandType::SETUPVIEWPORT;
    cmd.pNodeViewport = pViewport;
    Instance().m_commands.push(cmd);
}

void GraphicServer::OnInitialize()
{
    Instance().m_commands.push({CommandType::LOADSHADERS, nullptr});
}

void GraphicServer::OnUnInitialize()
{
    m_geoVert.Unload();
    m_geoFrag.Unload();

    m_lightVert.Unload();
    m_lightFrag.Unload();
}

void GraphicServer::LoadShader()
{
    m_geoVert.Load("res/shaders/GBuffer.vert");
    m_geoFrag.Load("res/shaders/GBuffer.frag");

    m_lightVert.Load("res/shaders/LightPass.vert");
    m_lightFrag.Load("res/shaders/LightPass.frag");

    m_geoProgram.AddShader(&m_geoVert);
    m_geoProgram.AddShader(&m_geoFrag);
    m_geoProgram.Load();

    m_lightProgram.AddShader(&m_lightVert);
    m_lightProgram.AddShader(&m_lightFrag);
    m_lightProgram.Load();

    GeoInfo cubeInfo = GeometryFactory::MakeCube(.5f, .5f, .5f);
    m_defaultCubeGeo = std::make_shared<Geometry>(cubeInfo.m_vertices, cubeInfo.m_indices);
    m_defaultTexture = std::make_shared<Texture>("res/textures/Default.png", TextureType::TYPE_2D, TextureMaterialType::DIFFUSE);
}

void GraphicServer::BuildTasksImpl(TaskGraph& graph) {}

void GraphicServer::FlushCommandsImpl()
{
    while (m_commands.empty() == false)
    {
        auto& cmd = m_commands.front();
        switch (cmd.Type)
        {
            case CommandType::SETUPVIEWPORT:
                cmd.pNodeViewport->Setup();
                break;
            case CommandType::ATTACHCAMERA:
                cmd.pNodeCamera->UpdateCameraOwner(*cmd.pNodeViewport);
                break;
            case CommandType::ATTACHVIEWPORT:
                cmd.pNodeWindow->AddViewport(*cmd.pNodeViewport);
                break;
            case CommandType::LOADSHADERS:
                LoadShader();
                break;
            case CommandType::OPENWINDOW:
                cmd.pNodeWindow->OpenWindow();
                break;
            case CommandType::CLEAR:
                cmd.pNodeWindow->ClearWindow();
                break;
            case CommandType::PRESENT:
                cmd.pNodeWindow->PresentWindow();
                break;
        }
        m_commands.pop();
    }
}
