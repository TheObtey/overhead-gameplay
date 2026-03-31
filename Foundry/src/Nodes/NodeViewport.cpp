#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"
#include "Nodes/Node2D.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Servers/GraphicServer.h"

#include <glm/fwd.hpp>
#include <memory>

NodeViewport::NodeViewport(std::string const& name) : Node2D(name)
{
	m_pViewPort = std::make_unique<Viewport>();
	GraphicServer::LoadShaderPrograms(this);

	OnHierarchyChanged += [&](){ TryAttachToWindow(); };
}

void NodeViewport::Setup()
{
	m_pViewPort->Setup({0.0f, 0.0f}, {10.0f, 10.0f}, m_clearColor);
	m_pGeometryPass = std::make_unique<GeometryPass>(GraphicServer::GetGeoProgram());
	m_pLightPass = std::make_unique<LightPass>(GraphicServer::GetLightProgram(), dummyLight);
	m_pViewPort->AddPass(m_pGeometryPass.get());
	m_pViewPort->AddPass(m_pLightPass.get());
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

void NodeViewport::SetCamera(NodeCamera* pCamera) const
{
	m_pGeometryPass->SetCamera(&pCamera->m_camera);
	m_pLightPass->SetCamera(&pCamera->m_camera);
}

void NodeViewport::AddMesh(NodeMesh const& mesh) const
{
	m_pGeometryPass->AddMesh(*mesh.m_pMesh);
}

void NodeViewport::UpdateViewport() const
{
	glm::vec2 const& pos = m_transform.GetPosition();
	glm::vec2 const& scale = m_transform.GetScale();
	m_pViewPort->SetPos(pos.x, pos.y);
	m_pViewPort->SetSize(scale.x, scale.y);
}

void NodeViewport::TryAttachToWindow()
{
	if (auto const window = FindFirstParentOfType<NodeWindow>())
		GraphicServer::AttachToWindow(this, &window->get());
}

ISerializable* NodeViewport::CreateInstance() { return CreateNode<NodeViewport>("NodeViewport").release(); }
