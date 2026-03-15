#ifndef ORE_WINDOW__H_
#define ORE_WINDOW__H_

#include "IWindow.h"
#include "Viewport.h"

#include <map>

class GLFWwindow;
class Window : public IWindow
{
public:
    Window(int width, int height, std::string name, bool enableTransparency = false);
    ~Window() override;

    void Close() override;
    bool IsOpen() override;
    void Present() override;
    void AddViewport(Viewport& viewport) {}; 

    void SetDecoration(bool hasDecoration) override;
    void SetIcon(std::string const& path) override {};
    void SetSize(uint16 width, uint16 height) override;
    static void FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height);

protected:
    void Open() override;

private:
    GLFWwindow* Get() const { return m_pWindow;}
    static std::unordered_map<GLFWwindow*, Window*> s_windows;
    GLFWwindow* m_pWindow;

    friend class Viewport;
    friend class EventManager;
};

#endif //!ORE_WINDOW__H_
