#ifndef __EDITOR_H
#define __EDITOR_H

#include "EditorRaylib3D.h"

#include <raylib.h>
#include <raymath.h>
#include <imgui.h>
#include <imfilebrowser.h>
#include <rlImGui.h>
#include <rlImGuiColors.h>
#include <Node.h>
#include <Define.h>

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

	// ImGui Panels
	void DrawHierarchyPanel();
	void DrawInspectorPanel();
	void DrawMenuBar();
	void DrawViewport3D();
	// 
	//void UpdateCam();

	// Scene Management	
	void CreateNewScene();

	void CreateNode(std::string Type, std::string const& name, Node* parent = nullptr);
	void DeleteNode(Node* node);

	//void AssignNode(Node* ToNode, Node* Thisnode);
	void SelectedNode(Node* Thisnode);
	void NewNodeSelected(Node* Thisnode);

	//Tree
	//void DrawNodeTree(Node& node);
	void DrawHierarchyNodeTree(Node& node);

	// Selector
	void DrawNodeSelector(Node& node);

	// Load/Save
	void LoadScene(std::string const& path);
	void SaveScene(std::string const& path);

	// View Root Management
	void SetViewRoot(Node* node);
	void ResetViewRoot();

	// Popup create
	/*TODO : Get Type of the Node and Create a Node from that type
	Example : CreateNode of type Node3D*/
	void ShowCreateNodePopup();
	void ShowCreateChildPopup(Node* parent);
	void ShowCreateSiblingPopup(Node* sibling);

	// Popup for save/load
	void SaveSceneNoSpe();

	void ShowSaveAsSceneBrowsing();
	void ShowLoadSceneBrowsing();

private:
	// Editor Parts
	EditorRaylib3D m_editorRaylib;

	// Editor State
	bool m_running = false;
	int m_screenWidth = 1900;
	int m_screenHeight = 900;

	// int m_viewportWidth
	// int m_viewportHeight

	// Scene
	uptr<Node> m_sceneRoot = nullptr;
	uptr<Node> m_newNodeTypeSelector = nullptr;

	Node* m_selectedNode = nullptr;
	Node* m_viewRoot = nullptr; // Current view root

	Node* m_newNodeTypeSelected = nullptr;

	// Creation Popup
	bool m_showCreatePopup = false;
	bool m_showCreateChildPopup = false;
	bool m_showCreateSiblingPopup = false;
	Node* m_pendingParent = nullptr;
	Node* m_pendingSibling = nullptr;
	char m_nodeNameBuffer[128] = "";

	// Popup save/load
	bool m_showSaveAsPopup = false;
	bool m_showLoadPopup = false;
	std::string m_scenePathBuffer;
	bool m_haveFileSelected = false;
	ImGui::FileBrowser m_saveBrowser;
	ImGui::FileBrowser m_loadBrowser;

	// 3D Camera

	// UI States
	bool m_showHierarchy = true;
	bool m_showInspector = true;
	bool m_showViewport = true;
};

#endif // __EDITOR_H