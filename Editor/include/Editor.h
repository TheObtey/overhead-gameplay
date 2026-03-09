#ifndef __EDITOR_H
#define __EDITOR_H

#include "EditorImGui.h"
#include "EditorRaylib3D.h"

#include <Define.h>
#include <Node.h>

class Editor
{
public:
	Editor();
	~Editor();

	void Init();
	void Run();
	void Shutdown();

private:
	// Main Loop
	void Update(float deltaTime);
	void Render3D();
	void RenderUI();

	// Command processing
	void ProcessUICommands();

	// Scene Management	
	void CreateNewScene();
	void CreateNode(std::string type, std::string const& name, Node* parent = nullptr);
	void UpdateNode(std::string name, json const& newDatas);
	void DeleteNode(Node* node);

	// Load/Save
	void LoadScene(std::string const& path);
	void SaveScene(std::string const& path);
	void SaveSceneNoSpe();

	void LoadDrawableObject(Node* pNode);

	json& GetNodeJson(std::string const& name);
	void RemoveNode(std::string const& name);

private:
	// Editor Parts
	EditorRaylib3D m_editorRaylib;
	EditorImGui m_editorImgui;

	// Editor State
	bool m_running = false;
	int m_screenWidth = 1900;
	int m_screenHeight = 900;

	// Scene
	uptr<Node> m_sceneRoot = nullptr;
	std::string m_scenePathBuffer;
};

#endif // __EDITOR_H