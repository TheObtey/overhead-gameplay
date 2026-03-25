#ifndef GRAPHIC_SERVER__H_
#define GRAPHIC_SERVER__H_

#include "Server.hpp"

class Window;
class NodeWindow;
class Viewport;

template <>
struct Command<class GraphicServer>
{
    enum class CmdType { OPENWINDOW, CLEAR, PRESENT } Type;
    NodeWindow* pNodeWindow = nullptr;
};

class GraphicServer : public Server<GraphicServer>
{
public:
    using CommandType = Command<GraphicServer>::CmdType;

    static void OpenWindow(NodeWindow* pWindow);
    static void Present(NodeWindow* pWindow);
    static void Clear(NodeWindow* pWindow);

private:
    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override;
	void OnInitialize() override {}
};

#endif
