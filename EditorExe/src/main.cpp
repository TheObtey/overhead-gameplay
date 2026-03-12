
#include <EditorSerializer.h>
#include <Editor.h>
#include <Scripting/Lua/LuaBindings.h>

int main()
{
	uptr<Node> node = Node::CreateNode<Node>("C++Node");

	Node* t1 = (Node*)AutomaticRegisterISerializable<ISerializable>::create("Node");
	Node3D* t2 = (Node3D*)AutomaticRegisterISerializable<ISerializable>::create("Node3D");

	uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>("../Game/res/test.lua");
	Node::AttachScript(script, *node);

	EditorSerializer::Save("ScriptScene", node);

	Editor editor;
	editor.Init();
	editor.Run();
	editor.Shutdown();

	return 0;
}
