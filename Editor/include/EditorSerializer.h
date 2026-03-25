#ifndef EDITOR_EDITOR_SERIALIZER_H__
#define EDITOR_EDITOR_SERIALIZER_H__

#include <Define.h>
#include <Node.h>
#include <string>

/*
	* Object used to save a SceneTree from Editor
	* Object used to load a SceneTree in Editor
	* Object used to load a SceneTree in Foundry
*/

struct LoadReturn {
	uptr<Node> uptrNode;
	bool IsRoot;
};

class EditorSerializer
{
public:
	static void SaveScene(std::string outPath, uptr<Node>& root);
	static void SaveNode(std::string outPath, uptr<Node>& Node);
	static LoadReturn LoadFromJson(std::string path);
};

#endif

