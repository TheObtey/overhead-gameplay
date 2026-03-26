#include "Servers/GraphicServer.h"

#include "Window.h"
#include "Nodes/NodeWindow.h"

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

void GraphicServer::BuildTasksImpl(TaskGraph &graph)
{

}

void GraphicServer::FlushCommandsImpl()
{
    while (m_commands.empty() == false)
    {
        auto&[type, pWindow] = m_commands.front();
        switch (type)
        {
            case CommandType::OPENWINDOW:
                pWindow->OpenWindow();
                break;
            case CommandType::CLEAR:
                pWindow->ClearWindow();
                break;
            case CommandType::PRESENT:
                pWindow->PresentWindow();
                break;
        }
        m_commands.pop();
    }
}