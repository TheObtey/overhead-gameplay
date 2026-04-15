#include "Registries/AutomaticRegisterProxy.hpp"
using CState = Ore::Window::CursorState;

class NodeWindow::Proxy : public Node2D::Proxy
{
public:
    struct ProxyBinding;

    Proxy(Node& node) : Node2D::Proxy(node), m_pNodeWindow(static_cast<NodeWindow*>(&node)) {}
    void SetCursorState(CState const cursorState) const { m_pNodeWindow->SetCursorState(cursorState); }

private:
    NodeWindow* m_pNodeWindow;
};

BindProxy(NodeWindow::Proxy,
    binder.BindEnum<CState>("CursorState",
        "FREE",   CState::FREE,
        "HIDDEN", CState::HIDDEN,
        "LOCKED", CState::LOCKED
    );

    return binder.BindClass<NodeWindow::Proxy>("nodewindow",
        sol::base_classes, sol::bases<Node::Proxy, Node2D::Proxy>(),
        sol::meta_function::garbage_collect, BIND(GCNodeProxy),
        sol::meta_function::new_index, StoreUserData(),
        sol::meta_function::index, LoadUserData(),
        "SetCursorState", BIND(SetCursorState)
    );
)