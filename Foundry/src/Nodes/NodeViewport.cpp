#include "Nodes/NodeViewport.h" 
#include "Nodes/Node2D.h"

#include <glm/fwd.hpp>
#include <memory>

NodeViewport::NodeViewport(std::string const& name) : Node2D(name)
{
	m_pViewPort = std::make_unique<Viewport>(0, 0, 1920, 1080, Color::SKY_BLUE );
}

void NodeViewport::OnUpdate(double const delta)
{
	if (m_transform.GetDirty())
		UpdateViewport();
	Node2D::OnUpdate(delta);
}

void NodeViewport::SetBackgroundColor(Color const &color) const
{
	if (m_pViewPort) m_pViewPort->SetBackgroundColor(color);
}

void NodeViewport::UpdateViewport() const
{
	glm::vec2 const& pos = m_transform.GetPosition();
	glm::vec2 const& scale = m_transform.GetScale();
	m_pViewPort->SetPos(pos.x, pos.y);
	m_pViewPort->SetSize(scale.x, scale.y);
}

ISerializable* NodeViewport::CreateInstance() { return CreateNode<NodeViewport>("NodeViewport").release(); }
