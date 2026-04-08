#ifndef ORE_WINDOW__H_
#define ORE_WINDOW__H_

#include "IWindow.h"
#include "Viewport.h"

#include <vector>
#include <GLFW/glfw3.h>
namespace Ore
{
class Window : public IWindow
{
public:
    Window(int width, int height, std::string name, bool enableTransparency = false, bool visible = true);
    ~Window();

    void Clear() override;
    void Close() override;
    bool IsOpen() override;
    void Present() override;

    void AddViewport(Viewport& viewport) {m_viewports.push_back(&viewport);} 
    void RemoveViewport(Viewport const& viewport);
    Viewport* GetViewport(uint32 const index) const {return m_viewports[index];}
    int32 GetViewportsCount() const { return m_viewports.size(); }
    uint32 GetWidth() const { return m_width; }
    uint32 GetHeight() const { return m_height; }
    glm::uvec2 GetSize() const { return { m_width, m_height} ; }

    void SetDecoration(bool hasDecoration) override;
    void SetIcon(std::string const& path) override; 
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
}
#endif //!ORE_WINDOW__H_
