#include "Servers/EngineServer.h"
#include "Multithreading/TaskGraph.h"

#include <memory>

void EngineServer::QueueAttach(std::unique_ptr<Node>& node, Node* const To)
{
    Instance().m_commands.push({CommandType::ATTACH, std::move(node), To});
    Instance().m_initialized = true;
}

void EngineServer::QueueFree(std::unique_ptr<Node>& node)
{
    Instance().m_commands.push({CommandType::FREE, std::move(node)});
}

void EngineServer::RegisterUnattachedNode(uptr<Node> &uNode)
{
    Instance().m_UnattachedNode[uNode.get()] = std::move(uNode);
}

void EngineServer::UnregisterUnattachedNode(Node *node)
{
    Instance().m_UnattachedNode.erase(node);
}

uptr<Node>& EngineServer::GetUnattachedNode(Node *ptr)
{
    return Instance().m_UnattachedNode[ptr];
}

void EngineServer::BuildTasksImpl(TaskGraph& graph)
{
    uptr<Task> t = std::make_unique<Task>();
    t->TaskFunction = [this]{ TestFunct(); };
    t->Name = "Dummy task";
    graph.AddTask(t);
}

void EngineServer::FlushCommandsImpl()
{
    while (m_commands.empty() == false)
    {
        Command<EngineServer>& command = m_commands.front();
        switch (command.Type)
        {
        case CommandType::ATTACH:
            command.To->AttachChildImmediate(command.Child);
            break;
        case CommandType::FREE:
            break;
        }
        m_commands.pop();
    }
}
