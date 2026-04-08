#ifndef EDITOR_EDITOR_H__
#define EDITOR_EDITOR_H__

#include "EditorImGui.h"
#include "EditorRaylib3D.h"

#include <Define.h>
#include <Node.h>
#include <filesystem>
#include <vector>
#include <unordered_map>


class Editor
{
public:
	Editor();
	~Editor();

	void Init();
	void Run();
	void Shutdown();

private:
	using ScriptPathMap = std::unordered_map<std::string, std::string>;

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
	void SaveFromNode(std::string const& path);
	void SaveSceneNoSpe();

	void LoadDrawableObject(Node* pNode);

	void StartFoundry(std::string const& path);

	void CollectLuaScripts(Node* pNode, std::vector<std::filesystem::path>& outScripts);
	ScriptPathMap CopyScriptIntoGame(std::filesystem::path const& scenePath, std::filesystem::path const& gameExePath);
	bool WritePlayScene(std::filesystem::path const& outputScenePath, ScriptPathMap const& scriptMap);
	void UpdateScriptPathsInJson(json& nodeJson, ScriptPathMap const& scriptMap);
	void RemoveStringClone(Node* pNode);
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
	std::string m_nodePathBuffer;
};

#endif // __EDITOR_H