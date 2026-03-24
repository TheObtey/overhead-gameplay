#ifndef GRAPHIC_SERVER__H_
#define GRAPHIC_SERVER__H_

#include "Server.hpp"

class Window;
class Viewport;

template <>
struct Command<class GraphicServer>
{
    enum class CmdType { OPENWINDOW, CLEAR, PRESENT } Type;
    Window* pWindow = nullptr;
};

class GraphicServer : public Server<GraphicServer>
{
public:
    using CommandType = Command<GraphicServer>::CmdType;

    static void OpenWindow(Window* pWindow);
    static void Present(Window* pWindow);
    static void Clear(Window* pWindow);

private:
    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override;
	void OnInitialize() override {}
};

#endif
