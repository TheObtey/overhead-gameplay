#include <Editor.h>
#include <EditorSerializer.h>

#include <Nodes/AllNodes.h>

int main()
{
    uptr<Node> rootNode = Node::CreateNode<Node>("Root");
    uptr<Node3D> rootNode1 = Node::CreateNode<Node3D>("Node3D");
	
	rootNode1.get()->SetWorldPosition({ 5.0f, 2.0f, 0.0f });
	uptr<Node> castedNode = uptr<Node>(rootNode1.get());

    rootNode->AddChild(castedNode);

    EditorSerializer::Save("testJson", rootNode);

	Editor editor;
	editor.Init();
	editor.Run();
	editor.Shutdown();

	return 0;
}
