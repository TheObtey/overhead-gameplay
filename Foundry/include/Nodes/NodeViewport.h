#ifndef FOUNDRY_NODEVIEWPORT__H_
#define FOUNDRY_NODEVIEWPORT__H_

#include "Define.h"
#include "Node2D.h"
#include "Viewport.h"

#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"
#include "Passes/AnimatedPass.h"

class NodeMesh;
class NodeMeshAnimated3D;
class NodeCamera;

class NodeViewport : public Node2D
{
public:
	//class Proxy

	explicit NodeViewport(std::string const& name);
	~NodeViewport() override = default;

	void Setup();

	virtual void OnUpdate(double delta) override;
	void SetBackgroundColor(Ore::Color const& color);

	//Set to nullptr if no camera is used
	void SetCamera(NodeCamera *pCamera);
	void AddMesh(NodeMesh const &mesh) const;
	void AddSkeletalMesh(NodeMeshAnimated3D const &mesh) const;

	static ISerializable* CreateInstance();

	Event<void(uint32, uint32)> OnViewportResize;
	uptr<Node> Clone() override;

private:
	void UpdateViewport();
	void TryAttachToWindow();
	void Clear() const;
	void Present() const;

protected:
	uptr<Ore::Viewport> m_pViewPort;
	std::vector<Ore::Light*> m_visibleLights;

	uptr<Ore::GeometryPass> m_pGeometryPass;
	uptr<Ore::LightPass> m_pLightPass;
	uptr<Ore::AnimatedPass> m_pAnimatedPass;

	Ore::Color m_clearColor { Ore::Color::SKY_BLUE};
	//TODO REMOVE
	std::array<Ore::Light, 5> dummyLight {};

	NodeCamera* m_pCamera = nullptr;

	friend class GraphicServer;
	friend class NodeWindow;
};

REGISTER_ISERIALIZABLE(NodeViewport, NodeViewport::CreateInstance);

#endif

