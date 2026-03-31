
class NodeBoxCollider::Proxy : public NodeCollider::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : NodeCollider::Proxy(node) {}
	void SetShape(const glm::vec3& halfExtents) { m_pNode->SetShape(halfExtents); }
private:
	NodeBoxCollider* m_pNode;
};

BindProxy(NodeBoxCollider::Proxy,
	return binder.BindClass<NodeBoxCollider::Proxy>("nodeboxcollider",
		"SetShape", BIND(SetShape)
	);
)

REGISTER_PROXY(NodeBoxCollider::Proxy::ProxyBinding, NodeBoxColliderProxy);


class NodeSphereCollider::Proxy : public NodeCollider::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : NodeCollider::Proxy(node) {}
	void SetShape(float radius) { m_pNode->SetShape(radius); }
private:
	NodeSphereCollider* m_pNode;
};

BindProxy(NodeSphereCollider::Proxy,
	return binder.BindClass<NodeSphereCollider::Proxy>("nodespherecollider",
		"SetShape", BIND(SetShape)
	);
)

REGISTER_PROXY(NodeSphereCollider::Proxy::ProxyBinding, NodeSphereColliderProxy);


class NodeCapsuleCollider::Proxy : public NodeCollider::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : NodeCollider::Proxy(node) {}
	void SetShape(float radius, float height) { m_pNode->SetShape(radius, height); }
private:
	NodeCapsuleCollider* m_pNode;
};

BindProxy(NodeCapsuleCollider::Proxy,
	return binder.BindClass<NodeCapsuleCollider::Proxy>("nodecapsulecollider",
		"SetShape", BIND(SetShape)
	);
)

REGISTER_PROXY(NodeCapsuleCollider::Proxy::ProxyBinding, NodeCapsuleColliderProxy);