#include "Nodes/NodeWindow.h"

#include "Servers/GraphicServer.h"

NodeWindow::NodeWindow(std::string const& name) : Node2D(name)
{
    m_pWindow = std::make_unique<Window>(1920, 1080, name, false, true);
    m_transform.SetScale(1920, 1080);
    m_pWindow->onResizeEvent += [&](int32 const width, int32 const height) { m_transform.SetScale(width, height); };
    GraphicServer::OpenWindow(this);
}

NodeWindow::~NodeWindow()
{
    m_pWindow->Close();
}
void NodeWindow::OnUpdate(double const delta)
{
    bool const dirty = m_transform.GetDirty();

    Node2D::OnUpdate(delta);
    if (dirty) UpdateWindow();

    GraphicServer::BeginFrame(this);
    GraphicServer::EndFrame(this);
}

void NodeWindow::AddViewport(NodeViewport& viewport)
{
    m_nViewports.push_back(viewport);
    m_pWindow->AddViewport(*viewport.m_pViewPort);
    UpdateWindow();
}

void NodeWindow::RemoveViewport(NodeViewport const& viewport)
{
    const auto it = std::ranges::find_if(m_nViewports, [&](const std::reference_wrapper<NodeViewport const> v)
    {
        return v.get().m_name == viewport.m_name;
    });

    if (it != m_nViewports.end())
        m_nViewports.erase(it);
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

    int32 viewportsC = m_pWindow->GetViewportsCount();

    //Automatic viewport splitting
    for (int8 i = 0; i < viewportsC; i ++)
    {
        NodeViewport& viewport =  m_nViewports[i].get();
        int const s = ((viewportsC + 1) * 0.5f);
        viewport.m_transform.SetScale(std::ceil(scale.x / viewportsC), scale.y / s);
        viewport.m_transform.SetPosition(std::ceil((scale.x / (viewportsC) - 1) * i),  (s - 1) * i);
    }
}

void NodeWindow::OpenWindow() const
{
    m_pWindow->Open();
}

void NodeWindow::ClearWindow() const
{
    m_pWindow->Clear();
}

void NodeWindow::PresentWindow() const
{
    m_pWindow->Present();
}
