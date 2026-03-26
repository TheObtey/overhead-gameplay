
class NodeCamera::Proxy : public Node3D::Proxy
{
public:
    struct ProxyBinding;

    Proxy(Node& node) : Node3D::Proxy(node), m_pNodeCamera(static_cast<NodeCamera*>(&node)) {}

private:
    NodeCamera* m_pNodeCamera;
};
