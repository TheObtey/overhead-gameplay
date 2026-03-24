#ifndef FOUNDRY_NODEVIEWPORT__H_
#define FOUNDRY_NODEVIEWPORT__H_

#include "Define.h"
#include "Node2D.h"
#include "Viewport.h"

class NodeViewport : public Node2D
{
public:
	//class Proxy

	explicit NodeViewport(std::string const& name);
	~NodeViewport() override = default;

	virtual void OnUpdate(double delta) override;

	void SetBackgroundColor(Color const& color) const;

	static ISerializable* CreateInstance();

private:
	void UpdateViewport() const;

protected:
	uptr<Viewport> m_pViewPort;
};

REGISTER_ISERIALIZABLE(NodeViewport, NodeViewport::CreateInstance);

#endif

