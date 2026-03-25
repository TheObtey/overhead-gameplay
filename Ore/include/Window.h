#ifndef ORE_WINDOW__H_
#define ORE_WINDOW__H_

#include "IWindow.h"
#include "Viewport.h"

#include <map>
#include <vector>
#include <GLFW/glfw3.h>

class Window : public IWindow
{
public:
    Window(int width, int height, std::string name, bool enableTransparency = false);
    ~Window() override;

    void Clear() override;
    void Close() override;
    bool IsOpen() override;
    void Present() override;

    void AddViewport(Viewport& viewport) {m_viewports.push_back(&viewport);} 
    void RemoveViewport(Viewport const& viewport);
    Viewport* GetWiewport(uint32 index) const {return m_viewports[index];}

    void SetDecoration(bool hasDecoration) override;
    void SetIcon(std::string const& path) override {};
    void SetSize(uint16 width, uint16 height) override;
    static void FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height);
    void Open() override;

private:
    static std::unordered_map<GLFWwindow*, Window*> s_windows;
    GLFWwindow* m_pWindow;
    std::vector<Viewport*> m_viewports;

    friend class Viewport;
    friend class EventManager;
};

#endif //!ORE_WINDOW__H_
