#ifndef FOUNDRY_LUA_SERVER__H_
#define FOUNDRY_LUA_SERVER__H_

#include "Define.h"
#include "Server.hpp"
#include "Node.h"

#include <sol/sol.hpp>
#include <sol/state.hpp>

template <>
struct Command<class LuaServer> {};

class LuaServer : public Server<LuaServer>
{
public:
	LuaServer() { OnInitialize(); }
	~LuaServer() { m_luaState.collect_garbage(); }

	//Send a non-owning raw pointer to lua and keep the uptr alive here
	static void RegisterUnattachedNode(uptr<Node>& uNode);
	static void UnregisterUnattachedNode(Node* node);

	static uptr<Node>& GetUnattachedNode(Node* ptr);
	static sol::state& GetLuaState() { return Instance().m_luaState; }

private:
    void FlushCommandsImpl() override {};
    void BuildTasksImpl(TaskGraph& graph) override {};

	void OnInitialize() override;

private:
	sol::state m_luaState;
	std::unordered_map<Node*, uptr<Node>> m_UnattachedNode {};
};

#endif // !FOUNDRY_LUA_SERVER__H_
