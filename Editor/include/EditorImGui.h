#ifndef __EDITORIMGUI_H
#define __EDITORIMGUI_H

#include <imgui.h>
#include <imfilebrowser.h>

#include <Node.h>
#include <Define.h>

class EditorImGui {
public:
	EditorImGui();
	~EditorImGui();

	void Init();
	void Render();
	
	void SetViewRoot(Node* node);
	void ShowSaveAs() { m_showSaveAsPopup = true; }
private:
	void DrawMenuBar();
	void DrawHierarchyPanel();
	void DrawInspectorPanel();

	void DrawNodeSelector(Node& node);	
	void DrawHierarchyNodeTree(Node& node);

	void SetViewRoot(Node* node);
	void ResetViewRoot();

	void ShowCreateNodePopup();
	void ShowCreateChildPopup(Node* parent);
	void ShowCreateSiblingPopup(Node* sibling);
	void ShowSaveAsSceneBrowsing();
	void ShowLoadSceneBrowsing();

	void SelectedNode(Node* node);
	void NewNodeSelected(Node* node);

	void SaveSceneNoScpecialisation();

private:	
	uptr<Node> m_newNodeTypeSelector = nullptr;

	bool m_showCreatePopup = false;
	bool m_showCreateChildPopup = false;
	bool m_showCreateSiblingPopup = false;

	Node* m_pendingParent = nullptr;
	Node* m_pendingSibling = nullptr;
	char m_nodeNameBuffer[128] = "";

	bool m_showSaveAsPopup = false;
	bool m_showLoadPopup = false;
	ImGui::FileBrowser m_saveBrowser;
	ImGui::FileBrowser m_loadBrowser;

	bool m_haveFileSelected = false;

	Node* m_selectedNode = nullptr;
	Node* m_newNodeTypeSelected = nullptr;

	Node* m_sceneRoot = nullptr;
	Node* m_viewRoot = nullptr; // Current view root

	Node* m_newNodeTypeSelected = nullptr;


	// UI States
	bool m_showHierarchy = true;
	bool m_showInspector = true;
	bool m_showViewport = true;

};

#endif //__EDITORIMGUI_H