#ifndef FOUNDRY_NODEWINDOW__H_
#define FOUNDRY_NODEWINDOW__H_

#include "Define.h"
#include "Window.h"
#include "Node2D.h"
#include "NodeViewport.h"

#include <string>

class NodeWindow : public Node2D
{
public:
    //class Proxy
    explicit NodeWindow(std::string const& name);
    ~NodeWindow() override;

    virtual void OnUpdate(double delta) override;

    bool IsOpen() const { return m_pWindow->IsOpen(); }

    void AddViewport(NodeViewport& viewport);
    void RemoveViewport(NodeViewport const& viewport);
    void SetDecoration(bool decoration) const;
    void SetIcon(std::string const& path) const;

    static ISerializable* CreateInstance();

private:
    void UpdateWindow() const;
    void OpenWindow() const;
    void ClearWindow() const;
    void PresentWindow() const;

private:
    uptr<Window> m_pWindow;
    std::vector<std::reference_wrapper<NodeViewport>> m_nViewports;
    std::string m_windowTitle;

    friend class GraphicServer;
};

REGISTER_ISERIALIZABLE(NodeWindow, NodeWindow::CreateInstance);

#endif