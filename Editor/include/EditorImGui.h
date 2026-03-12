#ifndef EDITOR_EDITOR_IMGUI_H__
#define EDITOR_EDITOR_IMGUI_H__

#include "InspectorNodePropreties.h"

#include <imgui.h>
#include <imfilebrowser.h>
#include <string>
#include <Define.h>
#include <Serialization/SerializeObject.hpp>
#include <Node.h>

class Editor;
class EditorRaylib3D;

struct EditorCommand
{
	enum class Type
	{
		NONE,
		CREATE_NODE,
		DELETE_NODE,
		CREATE_NEW_SCENE,
		SAVE_SCENE,
		LOAD_SCENE,
		EXIT_EDITOR,
		LUNCH_GAME,
		COUNT
	};

	Type type = Type::NONE;
	std::string stringParam1;  // For node type or file path
	std::string stringParam2;  // For node name
	Node* pNodeParam = nullptr; // For parent or node to delete

	void Reset() 
	{
		type = Type::NONE;
		stringParam1.clear();
		stringParam2.clear();
		pNodeParam = nullptr;
	}
};

class EditorImGui
{

public:
	EditorImGui(Editor* pEditor, EditorRaylib3D* pRaylibEditor);
	~EditorImGui();

	void Init();
	void Render();
	
	// Setter
	void SetSceneRoot(Node* pRoot);
	void SetScreenSize(int width, int height);
	void ShowSaveAs() { m_showSaveAsPopup = true; }

	void ResetViewRoot();
	void ResetSelectedNode();

	// Command handling
	EditorCommand& GetCommand() { return m_command; }
	void SetCommand(EditorCommand const& command) { m_command = command; }
	bool HasCommand() const { return m_command.type != EditorCommand::Type::NONE; }

private:
	void DrawMenuBar();
	void DrawHierarchyPanel();
	void DrawInspectorPanel();

	void DrawNodeSelector(Node& node);	
	void DrawHierarchyNodeTree(Node& node);

	void SetViewRoot(Node* pNode);

	enum class NodeCreationFlag
	{
		NONE,
		PARENT,
		SIBLING
	};

	void CreateNodePopup(Node* from, NodeCreationFlag flag, bool& open);
	
	// Merge into Create PopUp
	//void ShowCreateNodePopup();
	//void ShowCreateChildPopup(Node* pParent);
	//void ShowCreateSiblingPopup(Node* pSibling);
	
	void ShowSaveAsSceneBrowsing();
	void ShowLoadSceneBrowsing();

	void SelectedNode(Node* pNode);
	void NewNodeSelected(Node* pNode);

	void SaveSceneNoSpecialisation();

	json& LoadInspectorData();
	void ApplyInspectorChanges(json& datas);

private:
	EditorRaylib3D* m_pRaylibEditor;
	Editor* m_pEditor;
	uptr<Node> m_newNodeTypeSelector = nullptr;

	bool m_showCreatePopup = false;
	bool m_showCreateChildPopup = false;
	bool m_showCreateSiblingPopup = false;

	Node* m_pPendingParent = nullptr;
	Node* m_pPendingSibling = nullptr;
	char m_nodeNameBuffer[128] = "";

	bool m_showSaveAsPopup = false;
	bool m_showLoadPopup = false;
	ImGui::FileBrowser m_saveBrowser;
	ImGui::FileBrowser m_loadBrowser;
	int m_fileBrowsingSizeX = 800;
	int m_fileBrowsingSizeY = 450;

	bool m_haveFileSelected = false;
	std::string m_scenePathBuffer;

	Node* m_pSelectedNode = nullptr;
	Node* m_pLastSelectedNode = nullptr; 

	SerializedObject m_selectedNodeData;
	json m_selectedNodeDataJson;
	bool m_inspectorDirty = false;

	Node* m_pNewNodeTypeSelected = nullptr;

	Node* m_pSceneRoot = nullptr;
	Node* m_pViewRoot = nullptr;

	// UI States
	bool m_showHierarchy = true;
	bool m_showInspector = true;
	bool m_showViewport = true;

	// Screen size for UI positioning
	int m_screenWidth = 1900;
	int m_screenHeight = 900;

	bool m_play = false;

	EditorCommand m_command;
	InspectorNodePropreties m_inspector;

	friend InspectorNodePropreties;
};

#endif //EDITOR_EDITOR_IMGUI_H__