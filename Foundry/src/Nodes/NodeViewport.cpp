#include "Nodes/NodeViewport.h" 
#include "Nodes/Node2D.h"

#include <glm/fwd.hpp>
#include <memory>

#include "Servers/GraphicServer.h"

NodeViewport::NodeViewport(std::string const& name) : Node2D(name)
{
	m_pViewPort = std::make_unique<Viewport>();
	GraphicServer::LoadShaderPrograms(this);

	OnParentChange += [&](Node& n)
	{
		if (auto parent = FindFirstParentOfType<NodeWindow>())
			parent->get().AddViewport(*this);
	};
}

void NodeViewport::Setup() const
{
	m_pViewPort->Setup({0.0f, 0.0f}, {10.0f, 10.0f}, m_clearColor);
	UpdateViewport();
}

void NodeViewport::OnUpdate(double const delta)
{
	bool const dirty = m_transform.GetDirty();
	Node2D::OnUpdate(delta);
	if (dirty) UpdateViewport();
}

void NodeViewport::SetBackgroundColor(Color const &color)
{
	m_clearColor = color;
	m_pViewPort->SetBackgroundColor(color);
}

void NodeViewport::UpdateViewport() const
{
	glm::vec2 const& pos = m_transform.GetPosition();
	glm::vec2 const& scale = m_transform.GetScale();
	m_pViewPort->SetPos(pos.x, pos.y);
	m_pViewPort->SetSize(scale.x, scale.y);
}

ISerializable* NodeViewport::CreateInstance() { return CreateNode<NodeViewport>("NodeViewport").release(); }
