#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"
#include "Nodes/Node2D.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeMeshAnimated3D.h"
#include "Servers/GraphicServer.h"

#include <glm/fwd.hpp>
#include <memory>

NodeViewport::NodeViewport(std::string const& name) : Node2D(name)
{
	m_pViewPort = std::make_unique<Ore::Viewport>();
	GraphicServer::LoadShaderPrograms(this);

	OnHierarchyChanged += [&](){ TryAttachToWindow(); };
}

void NodeViewport::Setup()
{
	m_pViewPort->Setup({0.0f, 0.0f}, {10.0f, 10.0f}, m_clearColor);
	m_pGeometryPass = std::make_unique<Ore::GeometryPass>(GraphicServer::GetGeoProgram());
	m_pLightPass = std::make_unique<Ore::LightPass>(GraphicServer::GetLightProgram(), dummyLight);
	m_pAnimatedPass = std::make_unique<Ore::AnimatedPass>(GraphicServer::GetAnimatedProgram());

	Ore::Program& program = GraphicServer::GetLightProgram();
	program.Use();
	program.SetUniform("gPosition", 0);
	program.SetUniform("gNormal", 1);
	program.SetUniform("gAlbedoSpec", 2);


	m_pViewPort->AddPass(m_pGeometryPass.get());
	m_pViewPort->AddPass(m_pAnimatedPass.get());
	m_pViewPort->AddPass(m_pLightPass.get());
	UpdateViewport();
}

void NodeViewport::OnUpdate(double const delta)
{
	bool const dirty = m_transform.GetDirty();
	Node2D::OnUpdate(delta);
	if (dirty) UpdateViewport();

	GraphicServer::Clear(this);
	GraphicServer::Present(this);
}

void NodeViewport::SetBackgroundColor(Ore::Color const &color)
{
	m_clearColor = color;
	m_pViewPort->SetBackgroundColor(color);
}

void NodeViewport::SetCamera(NodeCamera* pCamera) const
{
	m_pGeometryPass->SetCamera(&pCamera->m_camera);
	m_pLightPass->SetCamera(&pCamera->m_camera);
	m_pAnimatedPass->SetCamera(&pCamera->m_camera);
}

void NodeViewport::AddMesh(NodeMesh const& mesh) const
{
	m_pGeometryPass->AddMesh(*mesh.m_pMesh);
}

void NodeViewport::AddSkeletalMesh(NodeMeshAnimated3D const& mesh) const
{
	m_pAnimatedPass->AddSkeletalMesh(*mesh.m_mesh);
}

void NodeViewport::UpdateViewport()
{
	glm::vec2 const& pos = m_transform.GetPosition();
	glm::vec2 const& scale = m_transform.GetScale();
	m_pViewPort->SetPos(pos.x, pos.y);
	m_pViewPort->SetSize(scale.x, scale.y);
	OnViewportResize(scale.x, scale.y);
}

void NodeViewport::TryAttachToWindow()
{
	if (auto const window = FindFirstParentOfType<NodeWindow>())
		GraphicServer::AttachToWindow(this, &window->get());
}

void NodeViewport::Clear() const
{
	m_pViewPort->Clear();
}

void NodeViewport::Present() const
{
	m_pViewPort->Present();
}

ISerializable* NodeViewport::CreateInstance() { return CreateNode<NodeViewport>("NodeViewport").release(); }
