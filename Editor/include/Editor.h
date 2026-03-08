#ifndef __EDITOR_H
#define __EDITOR_H

#include "EditorImGui.h"

#include <raylib.h>
#include <raymath.h>

#include <rlImGui.h>
#include <rlImGuiColors.h>

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

	void DrawViewport3D();

	// Command processing
	void ProcessUICommands();

	// Scene Management	
	void CreateNewScene();
	void CreateNode(std::string type, std::string const& name, Node* parent = nullptr);
	void DeleteNode(Node* node);

	// Load/Save
	void LoadScene(std::string const& path);
	void SaveScene(std::string const& path);
	void SaveSceneNoSpe();

private:
	EditorImGui m_editorImgui;

	// Editor State
	bool m_running = false;
	int m_screenWidth = 1900;
	int m_screenHeight = 900;

			// Scene
	uptr<Node> m_sceneRoot = nullptr;
	std::string m_scenePathBuffer;

	// 3D Camera
	Camera3D m_camera = {};
};

#endif // __EDITOR_H