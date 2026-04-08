#include "Servers/GraphicServer.h"

#include "GeometryFactory.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeWindow.h"
#include "Nodes/NodeViewport.h"

void GraphicServer::OpenWindow(NodeWindow* pWindow)
{
    Instance().m_commands.push({CommandType::OPENWINDOW, pWindow});
}

void GraphicServer::Present(NodeViewport* pViewport)
{
    Instance().m_commands.push({CommandType::PRESENT, nullptr, pViewport});
}

void GraphicServer::Clear(NodeViewport* pViewport)
{
    Instance().m_commands.push({CommandType::CLEAR, nullptr, pViewport});
}

void GraphicServer::BeginFrame(NodeWindow* pWindow)
{
    Instance().m_commands.push({CommandType::BEGINFRAME, pWindow});
}

void GraphicServer::EndFrame(NodeWindow* pWindow)
{
    Instance().m_commands.push({CommandType::ENDFRAME, pWindow});
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

    m_geoProgram.AddShader(m_geoVert);
    m_geoProgram.AddShader(m_geoFrag);
    m_geoProgram.Load();

    m_lightProgram.AddShader(m_lightVert);
    m_lightProgram.AddShader(m_lightFrag);
    m_lightProgram.Load();

    GeoInfo cubeInfo = GeometryFactory::MakeCube(1.0f, 1.0f, 1.0f);
    m_defaultCubeGeo = std::make_shared<Ore::Geometry>(cubeInfo.m_vertices, cubeInfo.m_indices);
    m_defaultTexture = std::make_shared<Ore::Texture>("res/textures/Default.png", Ore::TextureType::TYPE_2D, Ore::TextureMaterialType::DIFFUSE);
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
                cmd.pNodeViewport->Clear();
                break;
            case CommandType::PRESENT:
                cmd.pNodeViewport->Present();
                break;
            case CommandType::BEGINFRAME:
                cmd.pNodeWindow->ClearWindow();
                break;
        case CommandType::ENDFRAME:
                m_pWindowsToSwap.push_back(cmd.pNodeWindow);
                break;
        }
        m_commands.pop();
    }

    std::ranges::for_each(m_pWindowsToSwap, [](NodeWindow const* pWindow){pWindow->PresentWindow(); });
    m_pWindowsToSwap.clear();
}
