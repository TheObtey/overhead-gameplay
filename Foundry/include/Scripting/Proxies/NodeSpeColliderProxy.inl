
class NodeBoxCollider::Proxy : public NodeCollider::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : NodeCollider::Proxy(node), m_pNode(static_cast<NodeBoxCollider*>(&node)) {}

	static Proxy* CreateNodeBoxColliderProxy(std::string const& name);

	void SetShape(const glm::vec3& halfExtents) { m_pNode->SetShape(halfExtents); }
private:
	NodeBoxCollider* m_pNode;
};

struct NodeBoxCollider::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNodeBoxCollider", &NodeBoxCollider::Proxy::CreateNodeBoxColliderProxy);
		binder.BindClass<NodeBoxCollider::Proxy>("nodeboxcollider",
			sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy, NodeCollider::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"SetShape", BIND(SetShape));
	};
};

REGISTER_PROXY(NodeBoxCollider::Proxy::ProxyBinding, NodeBoxColliderProxy);


class NodeSphereCollider::Proxy : public NodeCollider::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : NodeCollider::Proxy(node), m_pNode(static_cast<NodeSphereCollider*>(&node)) {}

	static Proxy* CreateNodeSphereColliderProxy(std::string const& name);

	void SetShape(float radius) { m_pNode->SetShape(radius); }
private:
	NodeSphereCollider* m_pNode;
};

struct NodeSphereCollider::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNodeSphereCollider", &NodeSphereCollider::Proxy::CreateNodeSphereColliderProxy);
		binder.BindClass<NodeSphereCollider::Proxy>("nodespherecollider",
			sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy, NodeCollider::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"SetShape", BIND(SetShape));
	};
};

REGISTER_PROXY(NodeSphereCollider::Proxy::ProxyBinding, NodeSphereColliderProxy);


class NodeCapsuleCollider::Proxy : public NodeCollider::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : NodeCollider::Proxy(node), m_pNode(static_cast<NodeCapsuleCollider*>(&node)) {}

	static Proxy* CreateNodeCapsuleColliderProxy(std::string const& name);

	void SetShape(float radius, float height) { m_pNode->SetShape(radius, height); }
private:
	NodeCapsuleCollider* m_pNode;
};


struct NodeCapsuleCollider::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNodeCapsuleCollider", &NodeCapsuleCollider::Proxy::CreateNodeCapsuleColliderProxy);
		binder.BindClass<NodeCapsuleCollider::Proxy>("nodecapsulecollider",
			sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy, NodeCollider::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"SetShape", BIND(SetShape));
	};
};

REGISTER_PROXY(NodeCapsuleCollider::Proxy::ProxyBinding, NodeCapsuleColliderProxy);