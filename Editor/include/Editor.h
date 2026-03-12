#ifndef EDITOR_EDITOR_H__
#define EDITOR_EDITOR_H__

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
	void CreateNode(std::string const& type, std::string const& name, Node* pParent = nullptr);
	void DeleteNode(Node* pNode);

	// Load/Save
	void LoadScene(std::string const& path);
	void SaveScene(std::string const& path);
	void SaveSceneNoSpe();

	void LoadDrawableObject(Node* pNode);

	void StartFoundry(std::string const& path);
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