class NodeCamera::Proxy : public Node3D::Proxy
{
public:
    struct ProxyBinding;

    Proxy(Node& node) : Node3D::Proxy(node), m_pNodeCamera(static_cast<NodeCamera*>(&node)) {}
    void SetFOV(float fov) { m_pNodeCamera->SetFOV(fov); }

private:
    NodeCamera* m_pNodeCamera;
};

BindProxy(NodeCamera::Proxy,
    return binder.BindClass<NodeCamera::Proxy>("nodecamera",
    sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy>(),
    sol::meta_function::garbage_collect, BIND(GCNodeProxy),
    sol::meta_function::new_index, StoreUserData(),
    sol::meta_function::index, LoadUserData(),
    "SetFOV", BIND(SetFOV));
)

