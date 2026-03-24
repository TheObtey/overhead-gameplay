#include "Servers/GraphicServer.h"

#include "Window.h"

void GraphicServer::OpenWindow(Window *pWindow)
{
    Instance().m_commands.push({CommandType::OPENWINDOW, pWindow});
}

void GraphicServer::Present(Window *pWindow)
{
    Instance().m_commands.push({CommandType::PRESENT, pWindow});
}

void GraphicServer::Clear(Window *pWindow)
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
                pWindow->Open();
                pWindow->GetWiewport(0)->Setup();
                break;
            case CommandType::CLEAR:
                pWindow->Clear();
                break;
            case CommandType::PRESENT:
                pWindow->Present();
                break;
        }
        m_commands.pop();
    }
}