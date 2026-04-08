#ifndef FOUNDRY_NODEVIEWPORT__H_
#define FOUNDRY_NODEVIEWPORT__H_

#include "Define.h"
#include "Node2D.h"
#include "Viewport.h"

#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"

class NodeMesh;
class NodeCamera;

class NodeViewport : public Node2D
{
public:
	//class Proxy

	explicit NodeViewport(std::string const& name);
	~NodeViewport() override = default;

	void Setup();

	virtual void OnUpdate(double delta) override;
	void SetBackgroundColor(Color const& color);

	//Set to nullptr if no camera is used
	void SetCamera(NodeCamera *pCamera) const;
	void AddMesh(NodeMesh const &mesh) const;

	static ISerializable* CreateInstance();

	Event<void(uint32, uint32)> OnViewportResize;
private:
	void UpdateViewport();
	void TryAttachToWindow();
	void Clear() const;
	void Present() const;

protected:
	uptr<Viewport> m_pViewPort;
	std::vector<Light*> m_visibleLights;

	uptr<GeometryPass> m_pGeometryPass;
	uptr<LightPass> m_pLightPass;

	Color m_clearColor {Color::SKY_BLUE};
	//TODO REMOVE
	std::array<Light, 5> dummyLight {};


	friend class GraphicServer;
	friend class NodeWindow;
};

REGISTER_ISERIALIZABLE(NodeViewport, NodeViewport::CreateInstance);

#endif

