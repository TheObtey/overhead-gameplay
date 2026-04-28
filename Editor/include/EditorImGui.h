#ifndef EDITOR_EDITOR_IMGUI_H__
#define EDITOR_EDITOR_IMGUI_H__

#include "InspectorNodeProperties.h"

#include <imgui.h>
#include <imfilebrowser.h>
#include <imguiAssetBrowser.h>
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
		IMPORT_FBX_TO_ND,
		EXIT_EDITOR,
		LUNCH_GAME,
		COUNT
	};

	Type type = Type::NONE;
	std::string stringParam1;
	std::string stringParam2;
	Node* pNodeParam = nullptr;

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

	void SetSceneRoot(Node* pRoot);
	void SetScreenSize(int width, int height);
	void ShowSaveAs(bool node) { m_showSaveAsPopup = true; m_saveAsNode = node; }

	void ResetViewRoot();
	void ResetSelectedNode();
	void NotifyNodeWillBeDeleted(Node* pNode);

	EditorCommand& GetCommand() { return m_command; }
	void SetCommand(EditorCommand const& command) { m_command = command; }
	bool HasCommand() const { return m_command.type != EditorCommand::Type::NONE; }
	Node* GetSelectedNode() { return m_pSelectedNode; }
	Node* GetSceneRoot() { return m_pSceneRoot; }

	void ShowLoadPopup() { m_showLoadPopup = true; }

private:
	void DrawMenuBar();
	void DrawHierarchyPanel();
	void DrawInspectorPanel();

	void DrawNodeSelector(Node& node);
	void DrawHierarchyNodeTree(Node& node);

	void DrawGizmoButtons();

	void SetViewRoot(Node* pNode);

	enum class NodeCreationFlag
	{
		NONE,
		PARENT,
		SIBLING
	};

	std::string NormalizeScenePath(std::string path, bool saveAsNode);

	void CreateNodePopup(Node* from, NodeCreationFlag flag, bool& open);

	void ShowSaveAsSceneBrowsing();
	void ShowLoadSceneBrowsing();
	void ShowImportFbxBrowsing();

	void SelectedNode(Node* pNode);
	void NewNodeSelected(Node* pNode);

	void SaveSceneNoSpecialisation();

	json& LoadInspectorData();
	void ApplyInspectorChanges(json& datas);

	void BeginHierarchyDragSource(Node& node);
	void HandleHierarchyDropTarget(Node& targetNode);
	void HandleHierarchyRootDropTarget();
	bool IsDescendant(Node const& potentialAncestor, Node const& node) const;

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
	bool m_showImportFbxPopup = false;
	ImGui::FileBrowser m_saveBrowser;
	ImGui::FileBrowser m_loadBrowser;
	ImGui::FileBrowser m_importFbxBrowser;
	ImGui::AssetBrowser m_assetBrowser;
	int m_fileBrowsingSizeX = 800;
	int m_fileBrowsingSizeY = 450;

	bool m_haveFileSelected = false;
	std::string m_scenePathBuffer;
	std::string m_nodeSavePathBuffer;

	Node* m_pSelectedNode = nullptr;

	SerializedObject m_selectedNodeData;
	json m_selectedNodeDataJson;
	bool m_inspectorDirty = false;

	Node* m_pNewNodeTypeSelected = nullptr;

	Node* m_pSceneRoot = nullptr;
	Node* m_pViewRoot = nullptr;

	bool m_showHierarchy = true;
	bool m_showInspector = true;
	bool m_showViewport = true;

	int m_screenWidth = 1900;
	int m_screenHeight = 900;

	bool m_saveAsNode = false;


	bool m_play = false;

	EditorCommand m_command;
	InspectorNodeProperties m_inspector;

	friend InspectorNodeProperties;
};

#endif //EDITOR_EDITOR_IMGUI_H__