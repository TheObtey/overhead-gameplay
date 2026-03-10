#ifndef _EDITORSERIALIZER__H
#define _EDITORSERIALIZER__H

#include <Define.h>
#include <Node.h>
#include <string>

/*
	* Object used to save a SceneTree from Editor
	* Object used to load a SceneTree in Editor
	* Object used to load a SceneTree in Foundry
*/
class EditorSerializer
{
public:
	static void Save(std::string outPath, uptr<Node>& root);
	static uptr<Node> LoadFromJson(std::string path);
};

#endif

