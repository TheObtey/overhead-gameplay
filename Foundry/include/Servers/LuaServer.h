#ifndef FOUNDRY_LUA_SERVER__H_
#define FOUNDRY_LUA_SERVER__H_

#include "Define.h"
#include "Server.hpp"
#include "Node.h"
#include "Scripting/ScriptingEngineImplementation.h"

#include <sol/sol.hpp>
#include <sol/state.hpp>

template <>
struct Command<class LuaServer> {};

class LuaServer : public Server<LuaServer>
{
public:
	LuaServer() { OnInitialize(); }
	~LuaServer() { s_scriptEngine.get()->GetScriptEngine().collect_garbage(); }

	//Send a non-owning raw pointer to lua and keep the uptr alive here
	static void RegisterUnattachedNode(uptr<Node>& uNode);
	static void UnregisterUnattachedNode(Node* node);

	static uptr<Node>& GetUnattachedNode(Node* ptr);
	static sol::state& GetLuaState() { return s_scriptEngine.get()->GetScriptEngine(); }

private:
    void FlushCommandsImpl() override {};
    void BuildTasksImpl(TaskGraph& graph) override {};

	void OnInitialize() override;

private:
	inline static uptr<ScriptingEngineImplementation> s_scriptEngine = std::make_unique<ScriptingEngineImplementation>();
	std::unordered_map<Node*, uptr<Node>> m_UnattachedNode {};
};

#endif // !FOUNDRY_LUA_SERVER__H_
