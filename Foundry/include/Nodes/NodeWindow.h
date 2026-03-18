#ifndef FOUNDRY_NODEWINDOW__H_
#define FOUNDRY_NODEWINDOW__H_

#include "NodeViewport.h"
#include "Define.h"
#include "Window.h"

#include <string>

class Viewport;

class NodeWindow : public NodeViewport
{
public:
    //class Proxy
    explicit NodeWindow(std::string const& name);
    ~NodeWindow() override;

    virtual void OnUpdate(double delta) override;

    bool IsOpen() const { return m_pWindow->IsOpen(); }

    void AddViewport(Viewport& viewport) const;
    void SetDecoration(bool decoration) const;
    void SetIcon(std::string const& path) const;

    static ISerializable* CreateInstance();

private:
    void UpdateWindow() const;

private:
    uptr<Window> m_pWindow;
    std::string m_windowTitle;
};

REGISTER_ISERIALIZABLE(NodeWindow, NodeWindow::CreateInstance);

#endif