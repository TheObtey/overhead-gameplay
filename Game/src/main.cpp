////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Scripting/Lua/LuaBindings.h"
#include "Servers/EngineServer.h"

#include <Registries/AutomaticRegister.hpp>
#include <memory>

int main()
{
	uptr<Node> node = Node::CreateNode<Node>("C++Node");

	Node* t1 = (Node*)AutomaticRegisterISerializable<ISerializable>::create("Node");
	Node3D* t2 = (Node3D*)AutomaticRegisterISerializable<ISerializable>::create("Node3D");

	t2->SetX(22.0f);
	std::cout << t1->GetName() << std::endl;
	std::cout << t2->GetPosition().x << std::endl;

	uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("res/test.lua");
	Node::AttachScript(script, *node);

	node->Update(10.0f);

	EngineServer::FlushCommands();
}
