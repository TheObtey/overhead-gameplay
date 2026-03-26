#include "Nodes/NodeWindow.h"

#include "Servers/GraphicServer.h"

NodeWindow::NodeWindow(std::string const& name) : NodeViewport(name)
{
    m_pWindow = std::make_unique<Window>(1920, 1080, name);
    m_pWindow->AddViewport(*m_pViewPort);
    m_pWindow->onResizeEvent += [&](uint32 const width, uint32 const height) { m_pViewPort->SetSize(width, height); };
    GraphicServer::OpenWindow(this);
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
    GraphicServer::Clear(this);
    GraphicServer::Present(this);
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

void NodeWindow::OpenWindow() const
{
    m_pWindow->Open();
    m_pWindow->GetViewport(0)->Setup({0, 0}, m_pWindow->GetSize(), m_clearColor);
}

void NodeWindow::ClearWindow() const
{
    m_pWindow->Clear();
}

void NodeWindow::PresentWindow() const
{
    m_pWindow->Present();
}
