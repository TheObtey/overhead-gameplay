#ifndef FOUNDRY_NODEVIEWPORT__H_
#define FOUNDRY_NODEVIEWPORT__H_

#include "Define.h"
#include "Node2D.h"
#include "Viewport.h"

#include "Passes/GeometryPass.h"
#include "Passes/LightPass.h"

class NodeViewport : public Node2D
{
public:
	//class Proxy

	explicit NodeViewport(std::string const& name);
	~NodeViewport() override = default;

	void Setup() const;

	virtual void OnUpdate(double delta) override;
	void SetBackgroundColor(Color const& color);

	static ISerializable* CreateInstance();
private:
	void UpdateViewport() const;

protected:
	uptr<Viewport> m_pViewPort;
	std::vector<Light*> m_visibleLights;

	uptr<GeometryPass> m_pGeometryPass;
	uptr<LightPass> m_pLightPass;

	Color m_clearColor {Color::SKY_BLUE};

	friend class GraphicServer;
	friend class NodeWindow;
};

REGISTER_ISERIALIZABLE(NodeViewport, NodeViewport::CreateInstance);

#endif

