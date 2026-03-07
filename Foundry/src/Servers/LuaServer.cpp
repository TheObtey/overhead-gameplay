#include "Servers/LuaServer.h"
#include "Scripting/Binder.h"
#include "ProxyRegistration.h"
#include "Node.h"

#include <sol/state.hpp>
#include <sol/types.hpp>

void LuaServer::OnInitialize()
{
	//LuaState::GetLuaState().open_libraries(sol::lib::base, sol::lib::package, sol::lib::debug);
	//Binder b(m_luaState);
	//REGISTER_PROXY(b);
}

void LuaServer::RegisterUnattachedNode(uptr<Node> &uNode)
{
	Instance().m_UnattachedNode[uNode.get()] = std::move(uNode);
}

void LuaServer::UnregisterUnattachedNode(Node *node)
{
	Instance().m_UnattachedNode.erase(node);
}

uptr<Node>& LuaServer::GetUnattachedNode(Node *ptr)
{
	return Instance().m_UnattachedNode[ptr];
}

	//Bind each of the proxy function to a new lua type
//	m_luaState.new_usertype<Node::Proxy>("Node",
//			sol::meta_function::garbage_collect, &Node::Proxy::GCNodeProxy,
//			"AddChild", &Node::Proxy::AddChild,
//			"GetName", &Node::Proxy::GetName);
