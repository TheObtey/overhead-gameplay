#include "Nodes/NodeWindow.h"

NodeWindow::NodeWindow(std::string const& name) : NodeViewport(name)
{
    m_pWindow = std::make_unique<Window>(1920, 1080, name);
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
}
