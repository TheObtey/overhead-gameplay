#include <Editor.h>
#include <EditorSerializer.h>

#include <Nodes/AllNodes.h>

int main()
{
    uptr<Node> rootNode = Node::CreateNode<Node>("Root");
    uptr<Node3D> rootNode1 = Node::CreateNode<Node3D>("Node3D");

	rootNode1->SetWorldPosition({ 5.0f, 2.0f, 0.0f });
	uptr<Node> castedNode = std::move(rootNode1);
    rootNode->AddChild(castedNode);

    EditorSerializer::Save("testJson", rootNode);

	Editor editor;
	editor.Init();
	editor.Run();
	editor.Shutdown();

	return 0;
}
