class NodeMesh::Proxy : public Node3D::Proxy
{
public:
    struct ProxyBinding;

    Proxy(Node& node) : Node3D::Proxy(node), m_pNodeMesh(static_cast<NodeMesh*>(&node)) {}

private:
    NodeMesh* m_pNodeMesh;
};

BindProxy(NodeMesh::Proxy,
    return binder.BindClass<NodeMesh::Proxy>("nodemesh",
        sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy>(),
        sol::meta_function::garbage_collect, BIND(GCNodeProxy),
        sol::meta_function::new_index, StoreUserData(),
        sol::meta_function::index, LoadUserData()
    );
)
