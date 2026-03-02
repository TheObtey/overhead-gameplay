////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Node.h"
#include "ProxyRegistration.h"
#include "Scripting/Lua/LuaScriptInstance.hpp"

#include <memory>

int main()
{	
	REGISTER_PROXY();

	uptr<Node> node = Node::CreateNode<Node>("Ca marche nickel");

	uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("res/test.lua");
	Node::AttachScript(script, *node);


	node->Update(10.0f);
}
