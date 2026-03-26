#ifndef ENGINE_SERVER__H_
#define ENGINE_SERVER__H_

#include "Server.hpp"
#include "Node.h"

#include <iostream>
#include <memory>

template <>
struct Command<class EngineServer>
{
    enum class CmdType {FREE, ATTACH} Type;
    std::unique_ptr<Node> Child; //The command now owns the node, it will be deleted when the command is executed
    Node* const To = nullptr;
};

using CommandType = Command<EngineServer>::CmdType;

class EngineServer : public Server<EngineServer>
{
public:
    static void QueueAttach(std::unique_ptr<Node>& node, Node* const to);
    static void QueueFree(std::unique_ptr<Node>& node);

    //Send a non-owning raw pointer to lua and keep the uptr alive here
    static void RegisterUnattachedNode(uptr<Node>& uNode);
    static void UnregisterUnattachedNode(Node* node);

    static uptr<Node>& GetUnattachedNode(Node* ptr);

    void TestFunct() { std::cout << ""; }

private:
    void FlushCommandsImpl() override;
    void BuildTasksImpl(TaskGraph& graph) override;
	void OnInitialize() override {}

private:
    std::unordered_map<Node*, uptr<Node>> m_UnattachedNode {};

};

#endif
