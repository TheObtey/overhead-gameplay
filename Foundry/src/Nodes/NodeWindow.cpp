#include "Nodes/NodeWindow.h"

#include "Servers/GraphicServer.h"

NodeWindow::NodeWindow(std::string const& name) : NodeViewport(name)
{
    m_pWindow = std::make_unique<Window>(1920, 1080, name);
    m_pWindow->AddViewport(*m_pViewPort);
    m_pWindow->onResizeEvent += [&](uint32 const width, uint32 const height) { m_pViewPort->SetSize(width, height); };
    GraphicServer::OpenWindow(m_pWindow.get());
}

NodeWindow::~NodeWindow()
{
    m_pWindow->Close();
}

void NodeWindow::OnUpdate(double const delta)
{
    if (m_transform.GetDirty())
        UpdateWindow();
    NodeViewport::OnUpdate(delta);
    GraphicServer::Clear(m_pWindow.get());
    GraphicServer::Present(m_pWindow.get());
}

void NodeWindow::AddViewport(Viewport& viewport) const
{
    m_pWindow->AddViewport(viewport);
}

void NodeWindow::SetDecoration(bool const decoration) const
{
    m_pWindow->SetDecoration(decoration);
}

void NodeWindow::SetIcon(std::string const &path) const
{
    m_pWindow->SetIcon(path);
}

ISerializable* NodeWindow::CreateInstance()
{
    return CreateNode<NodeWindow>("NodeWindow").release();
}

void NodeWindow::UpdateWindow() const
{
    glm::vec2 const& scale = m_transform.GetScale();
    m_pWindow->SetSize(scale.x, scale.y);
    m_pViewPort->SetSize(scale.x, scale.y);
}
