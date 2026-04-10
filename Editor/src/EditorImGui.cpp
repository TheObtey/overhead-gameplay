#include "EditorImGui.h"
#include "Editor.h"
#include "EditorRaylib3D.h"
#include "Debug.h"
#include <Servers/EngineServer.h>

#include <iostream>

namespace
{
	constexpr char kHierarchyNodePayloadId[] = "EDITOR_HIERARCHY_NODE";
	bool TryExtractTrailingIndex(std::string const& name, std::string& outBaseName, int& outIndex)
	{
		outBaseName = name;
		outIndex = 0;

		if (name.size() < 3 || name.back() != ')')
			return false;

		size_t const openPos = name.find_last_of('(');
		if (openPos == std::string::npos || openPos + 1 >= name.size() - 1)
			return false;

		for (size_t i = openPos + 1; i < name.size() - 1; ++i)
		{
			if (!std::isdigit(static_cast<unsigned char>(name[i])))
				return false;
		}

		outBaseName = name.substr(0, openPos);
		outIndex = std::stoi(name.substr(openPos + 1, name.size() - openPos - 2));
		return true;
	}

	std::string BuildDuplicateName(Node& parent, std::string const& sourceName)
	{
		std::string baseName;
		int trailingIndex = 0;
		TryExtractTrailingIndex(sourceName, baseName, trailingIndex);

		if (baseName.empty())
			baseName = sourceName;

		int index = std::max(1, trailingIndex + 1);
		std::string candidate;

		do
		{
			candidate = baseName + "(" + std::to_string(index) + ")";
			++index;
		} while (parent.FindChild(candidate).has_value());

		return candidate;
	}
}

std::string EditorImGui::NormalizeScenePath(std::string path, bool saveAsNode)
{
	std::string extensions[] = { ".json", ".sc", ".nd" };
	for (auto& ext : extensions)
	{
		if (path.size() >= ext.size() &&
			path.substr(path.size() - ext.size()) == ext)
		{
			path = path.substr(0, path.size() - ext.size());
		}
	}

	if (saveAsNode)
		path += ".nd";
	else
		path += ".sc";

	return path;
}

EditorImGui::EditorImGui(Editor* pEditor, EditorRaylib3D* pRaylibEditor)
	: m_pEditor(pEditor), m_pRaylibEditor(pRaylibEditor), m_inspector(this)
{
}

EditorImGui::~EditorImGui()
{
}

void EditorImGui::Init()
{
	m_newNodeTypeSelector = Node::CreateNode<Node>("Node");
	auto node3D = Node::CreateNode<Node>("Node3D");
	auto rigibody = Node::CreateNode<Node>("NodeRigidBody");

	auto collider = Node::CreateNode<Node>("NodeCollider");
	auto colliderbox = Node::CreateNode<Node>("NodeBoxCollider");
	auto collidersphere = Node::CreateNode<Node>("NodeSphereCollider");
	auto collidercapsule = Node::CreateNode<Node>("NodeCapsuleCollider");

	auto nodeVisual = Node::CreateNode<Node>("NodeVisual");
	auto nodeMesh = Node::CreateNode<Node>("NodeMesh");

	auto nodeCamera = Node::CreateNode<Node>("NodeCamera");

	collider->AddChild(colliderbox);
	collider->AddChild(collidersphere);
	collider->AddChild(collidercapsule);
	nodeVisual->AddChild(nodeMesh);
	node3D->AddChild(nodeVisual);
	node3D->AddChild(collider);
	node3D->AddChild(rigibody);
	node3D->AddChild(nodeCamera);
	m_newNodeTypeSelector->AddChild(node3D);
	EngineServer::FlushCommands();

	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::FileBrowser SaveBrowseWindow(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir | ImGuiFileBrowserFlags_ConfirmOnEnter);
	ImGui::FileBrowser LoadBrowseWindow(ImGuiFileBrowserFlags_ConfirmOnEnter);
	m_saveBrowser = SaveBrowseWindow;
	m_loadBrowser = LoadBrowseWindow;
	m_saveBrowser.SetDirectory("../Game/res");
	m_loadBrowser.SetDirectory("../Game/res");
	m_assetBrowser.SetRoot("../");
	m_assetBrowser.SetThumbnailSize(64.0f);
	m_assetBrowser.SetTypeFilters({ ".png", ".jpg", ".fbx", ".obj", ".lua", ".json" });
	m_assetBrowser.SetFlags(ImGui::AssetBrowserFlags_MultiSelect | ImGui::AssetBrowserFlags_AutoRefresh);

	m_inspector.SetWindow(m_screenWidth, m_screenHeight);
}

void EditorImGui::SetSceneRoot(Node* pRoot)
{
	m_pSceneRoot = pRoot;
	if (!m_pViewRoot || m_pViewRoot == m_pSceneRoot)
	{
		m_pViewRoot = pRoot;
	}
}

void EditorImGui::SetScreenSize(int width, int height)
{
	m_screenWidth = width;
	m_screenHeight = height;
}

void EditorImGui::Render()
{
	DrawMenuBar();
	DrawHierarchyPanel();
	DrawInspectorPanel();
	DrawGizmoButtons();

	CreateNodePopup(nullptr, NodeCreationFlag::NONE, m_showCreatePopup);
	CreateNodePopup(m_pPendingParent, NodeCreationFlag::PARENT, m_showCreateChildPopup);
	CreateNodePopup(m_pPendingSibling, NodeCreationFlag::SIBLING, m_showCreateSiblingPopup);

	ShowSaveAsSceneBrowsing();
	ShowLoadSceneBrowsing();
	m_assetBrowser.Display();

	ImGui::SetNextWindowPos(ImVec2(m_screenWidth - 120.0f, m_screenHeight - 40.0f));
	ImGui::SetNextWindowSize(ImVec2(110, 30));
	ImGui::Begin("##FPS", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBackground);
	m_pSceneRoot;
	ImGui::End();
}

void EditorImGui::DrawInspectorPanel()
{
	if (!m_showInspector) return;
	m_inspector.DrawWindow(m_showInspector, m_pSelectedNode);
}

void EditorImGui::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
			{
				m_command.type = EditorCommand::Type::CREATE_NEW_SCENE;
				m_pSelectedNode = nullptr;
				m_pViewRoot = m_pSceneRoot;
			}

			if (ImGui::MenuItem("Load Scene", "Ctrl+O"))
			{
				m_showLoadPopup = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				SaveSceneNoSpecialisation();
			}
			if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S"))
			{
				m_showSaveAsPopup = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				m_command.type = EditorCommand::Type::EXIT_EDITOR;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Hierarchy", nullptr, &m_showHierarchy);
			ImGui::MenuItem("Inspector", nullptr, &m_showInspector);
			ImGui::MenuItem("Viewport", nullptr, &m_showViewport);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Empty Node"))
			{
				m_showCreatePopup = true;
				m_nodeNameBuffer[0] = '\0';
			}
			ImGui::EndMenu();
		}
		for (int x = 0; x < 90; x++)
			ImGui::Spacing();

		if (ImGui::Button(m_play ? "Stop" : "Play", ImVec2(60, 0)))
		{
			if (!m_play)
			{
				SaveSceneNoSpecialisation();
				m_play = true;
			}
			else
			{
				m_play = false;
				DEBUG("[EditorImGui] Stopping game..." << std::endl);
			}
		}

		if (m_haveFileSelected && m_scenePathBuffer.length() != 0 && m_play == true) {

			m_command.type = EditorCommand::Type::LUNCH_GAME;
			m_command.stringParam1 = m_scenePathBuffer + ".json";
			m_play = false;

		}
		ImGui::EndMainMenuBar();
	}
}

void EditorImGui::DrawHierarchyPanel()
{
	if (!m_showHierarchy) return;

	float menuBarHeight = ImGui::GetFrameHeight();
	ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(350, m_screenHeight - menuBarHeight), ImGuiCond_Always);

	ImGui::Begin("Hierarchy", &m_showHierarchy, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (m_pViewRoot != m_pSceneRoot)
	{
		if (m_pViewRoot->GetParent() != nullptr)
		{
			if (ImGui::Button("< Parent"))
			{
				SetViewRoot(m_pViewRoot->GetParent());
			}
			ImGui::SameLine();
		}

		if (ImGui::Button("< Scene Root"))
		{
			ResetViewRoot();
		}

		ImGui::Separator();
	}

	if (ImGui::Button("+ Create Node", ImVec2(-1, 0)))
	{
		m_showCreatePopup = true;
		m_nodeNameBuffer[0] = '\0';
	}

	ImGui::Separator();

	if (m_pViewRoot)
	{
		ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "Viewing: %s", m_pViewRoot->GetName().c_str());
		ImGui::Separator();
	}

	if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem("Add Child to Root"))
		{
			m_pPendingParent = m_pViewRoot;
			m_showCreateChildPopup = true;
			m_nodeNameBuffer[0] = '\0';
		}
		ImGui::EndPopup();
	}

	if (m_pViewRoot)
	{
		DrawHierarchyNodeTree(*m_pViewRoot);
	}
	else
	{
		ImGui::TextDisabled("No scene loaded");
	}

	if (m_pSceneRoot)
	{
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Selectable("Drop here for root node", false, ImGuiSelectableFlags_Disabled, ImVec2(300, 0));
		HandleHierarchyRootDropTarget();
	}

	ImGui::End();
}

void EditorImGui::DrawNodeSelector(Node& node)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ImGuiTreeNodeFlags_SpanAvailWidth;

	if (&node == m_pNewNodeTypeSelected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.5f, 0.8f, 0.8f));
	}

	if (node.GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx(node.GetName().c_str(), flags);

	if (&node == m_pNewNodeTypeSelected)
		ImGui::PopStyleColor();

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		NewNodeSelected(&node);
	}

	if (nodeOpen)
	{
		for (uint32 i = 0; i < node.GetChildCount(); ++i)
		{
			DrawNodeSelector(node.GetChild(i));
		}
		ImGui::TreePop();
	}
}

void EditorImGui::DrawHierarchyNodeTree(Node& node)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (&node == m_pSelectedNode)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.5f, 0.8f, 0.8f));
	}

	if (node.GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx(node.GetName().c_str(), flags);

	if (&node == m_pSelectedNode)
		ImGui::PopStyleColor();

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		SelectedNode(&node);
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
	{
		SetViewRoot(&node);
	}

	if (ImGui::BeginPopupContextItem())
	{
		bool isSceneRoot = (node.GetParent() == nullptr);

		if (ImGui::MenuItem("Add Child"))
		{
			m_pPendingParent = &node;
			m_showCreateChildPopup = true;
			m_nodeNameBuffer[0] = '\0';
		}

		if (ImGui::MenuItem("Add Sibling", nullptr, false, !isSceneRoot))
		{
			if (!isSceneRoot)
			{
				m_pPendingSibling = &node;
				m_showCreateSiblingPopup = true;
				m_nodeNameBuffer[0] = '\0';
			}
		}

		ImGui::BeginDisabled(isSceneRoot);
		if (ImGui::MenuItem("Duplicate"))
		{
			if (!isSceneRoot)
			{
				auto clone = node.Clone();
				if (Node* pParent = node.GetParent())
				{
					clone->SetName(Node::BuildDuplicateName(*pParent, node.GetName()));
					pParent->AddChild(clone);
				}
			}
		}

		if (ImGui::MenuItem("Save From Here")) {
			m_showSaveAsPopup = true;
			m_saveAsNode = true;
		}



		ImGui::EndDisabled();

		if (ImGui::MenuItem("Set as View Root"))
		{
			SetViewRoot(&node);
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Delete", nullptr, false, !isSceneRoot))
		{
			if (!isSceneRoot)
			{
				m_command.type = EditorCommand::Type::DELETE_NODE;
				m_command.pNodeParam = &node;
			}
		}

		ImGui::EndPopup();
	}

	BeginHierarchyDragSource(node);
	HandleHierarchyDropTarget(node);

	if (nodeOpen)
	{
		for (uint32 i = 0; i < node.GetChildCount(); ++i)
		{
			DrawHierarchyNodeTree(node.GetChild(i));
		}
		ImGui::TreePop();
	}
}

void EditorImGui::SetViewRoot(Node* node)
{
	if (node)
	{
		m_pViewRoot = node;
		DEBUG("[EditorImGui] View root changed to: " << node->GetName() << std::endl);
	}
}

void EditorImGui::ResetViewRoot()
{
	m_pViewRoot = m_pSceneRoot;
	DEBUG("[EditorImGui] View root reset to scene root" << std::endl);
}

void EditorImGui::ResetSelectedNode() {

	m_pSelectedNode = nullptr;
	DEBUG("[EditorImGui] Selected root reset" << std::endl);
}

void EditorImGui::CreateNodePopup(Node* from, NodeCreationFlag flag, bool& open)
{
	std::string popupDetails = "";
	switch (flag)
	{
	case EditorImGui::NodeCreationFlag::NONE:
		break;
	case EditorImGui::NodeCreationFlag::PARENT:
		popupDetails = "Child";
		break;
	case EditorImGui::NodeCreationFlag::SIBLING:
		popupDetails = "Sibling";
		break;
	default:
		break;
	}
	std::string title = "Create " + popupDetails + " Node";
	if (open)
	{
		ImGui::OpenPopup(title.c_str());
		open = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (from)
		{
			std::string txtTitle = "Add " + popupDetails + " to: % s";
			ImGui::Text(txtTitle.c_str(), from->GetName().c_str());
			ImGui::Spacing();
		}

		DrawNodeSelector(*m_newNodeTypeSelector);

		ImGui::Text("Enter node name:");
		ImGui::Spacing();
		std::string inputText = "##" + popupDetails + "Name";
		bool enterPressed = ImGui::InputText(inputText.c_str(), m_nodeNameBuffer, sizeof(m_nodeNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Spacing();

		if ((ImGui::Button("Create", ImVec2(120, 0)) || enterPressed) && m_pNewNodeTypeSelected != nullptr)
		{
			bool flagCheck = false;
			if (flag == NodeCreationFlag::NONE) flagCheck = true;
			if (flag == NodeCreationFlag::PARENT) flagCheck = (from != nullptr);
			if (flag == NodeCreationFlag::SIBLING) flagCheck = (from != nullptr && from->GetParent() != nullptr);

			if (strlen(m_nodeNameBuffer) > 0 && flagCheck)
			{
				m_command.type = EditorCommand::Type::CREATE_NODE;
				m_command.stringParam1 = m_pNewNodeTypeSelected->GetName();
				m_command.stringParam2 = m_nodeNameBuffer;
				m_command.pNodeParam = flag == NodeCreationFlag::SIBLING ? from->GetParent() : from;
				m_pPendingParent = nullptr;
				m_pNewNodeTypeSelected = nullptr;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_pPendingParent = nullptr;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void EditorImGui::ShowSaveAsSceneBrowsing()
{
	if (m_showSaveAsPopup)
	{
		m_saveBrowser.Open();
		m_showSaveAsPopup = false;
	}

	m_saveBrowser.SetWindowSize(m_fileBrowsingSizeX, m_fileBrowsingSizeY);
	m_saveBrowser.SetWindowPos(m_screenWidth / 2 - m_fileBrowsingSizeX / 2,
		m_screenHeight / 2 - m_fileBrowsingSizeY / 2);

	m_saveBrowser.SetTitle("Save scene to Json file");
	m_saveBrowser.SetTypeFilters({ ".json" });
	m_saveBrowser.Display();

	if (m_saveBrowser.HasSelected())
	{
		std::string path = m_saveBrowser.GetSelected().string();
		if (!path.empty())
		{
			m_haveFileSelected = true;
			path = NormalizeScenePath(path, m_saveAsNode);

			m_scenePathBuffer = path;
			m_command.type = EditorCommand::Type::SAVE_SCENE;
			m_command.stringParam1 = m_scenePathBuffer;

			m_saveAsNode = false;
		}

		m_saveBrowser.ClearSelected();
		m_saveBrowser.Close();
	}
}
void EditorImGui::ShowLoadSceneBrowsing()
{
	if (m_showLoadPopup)
	{
		m_loadBrowser.Open();
		m_showLoadPopup = false;
	}

	m_loadBrowser.SetWindowSize(m_fileBrowsingSizeX, m_fileBrowsingSizeY);
	m_loadBrowser.SetWindowPos(m_screenWidth / 2 - m_fileBrowsingSizeX / 2, m_screenHeight / 2 - m_fileBrowsingSizeY / 2);

	m_loadBrowser.SetTitle("Load scene from Json file");
	m_loadBrowser.SetTypeFilters({ ".json" });
	m_loadBrowser.Display();

	if (m_loadBrowser.HasSelected())
	{
		std::filesystem::path const selected = m_loadBrowser.GetSelected();
		std::string selectedLogicalPath;

		if (selected.extension() == ".json")
		{
			std::filesystem::path const logical = selected.parent_path() / selected.stem();

			if (logical.extension() == ".sc")
			{
				m_scenePathBuffer = logical.string();
				selectedLogicalPath = m_scenePathBuffer;
			}
			else if (logical.extension() == ".nd")
			{
				m_nodeSavePathBuffer = logical.string();
				selectedLogicalPath = m_nodeSavePathBuffer;
			}
		}

		if (!selectedLogicalPath.empty())
		{
			m_haveFileSelected = true;
			m_command.type = EditorCommand::Type::LOAD_SCENE;
			m_command.stringParam1 = selectedLogicalPath + ".json";
			m_pSelectedNode = nullptr;
			m_pViewRoot = m_pSceneRoot;
		}

		m_loadBrowser.ClearSelected();
		m_loadBrowser.Close();
	}
}

void EditorImGui::DrawGizmoButtons()
{
	if (m_pSelectedNode == nullptr) return;
	bool translate = m_pRaylibEditor->IsGizmoTranslate();
	bool scale = m_pRaylibEditor->IsGizmoScale();
	bool rotate = m_pRaylibEditor->IsGizmoRotate();
	bool all = translate && rotate && scale;
	bool isopen = true;
	ImGui::SetNextWindowSize(ImVec2(700, 100), ImGuiCond_Always);
	ImGui::Begin("Gizmo", &isopen, ImGuiWindowFlags_NoResize);

	if (ImGui::Checkbox("All", &all) && m_pSelectedNode != nullptr)
	{
		m_pRaylibEditor->SetTranslateGizmo(all);
		m_pRaylibEditor->SetScaleGizmo(all);
		m_pRaylibEditor->SetRotateGizmo(all);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Translate", &translate))
	{
		m_pRaylibEditor->SetTranslateGizmo(translate);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Scale", &scale))
	{
		m_pRaylibEditor->SetScaleGizmo(scale);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Rotate", &rotate))
	{
		m_pRaylibEditor->SetRotateGizmo(rotate);
	}

	ImGui::Text("Set Camera On Axis:");
	ImGui::SameLine();
	ImGui::Text("\t \t Choose Camera State:");

	if (ImGui::Button("Axis X")) //ImVec4(0.90f,0.16f,0.21f,1.0f)
	{
		m_pRaylibEditor->SetCameraOnAxis(EditorRaylib3D::RaylibAxis::X);
	}
	ImGui::SameLine();
	if (ImGui::Button("Axis Y")) // ImVec4(0.0f, 0.89f, 0.18f, 1.0f)
	{
		m_pRaylibEditor->SetCameraOnAxis(EditorRaylib3D::RaylibAxis::Y);
	}
	ImGui::SameLine();
	if (ImGui::Button("Axis Z")) // ImVec4(0.0f, 0.47f, 0.94f, 1.0f)
	{
		m_pRaylibEditor->SetCameraOnAxis(EditorRaylib3D::RaylibAxis::Z);
	}
	ImGui::SameLine();
	ImGui::Text("\t  ");
	ImGui::SameLine();
	if (ImGui::Button("Camera OrthoGraphic"))
	{
		m_pRaylibEditor->ChangeCamera(EditorRaylib3D::CameraState::ORTHOGRAPHIC);
	}
	ImGui::SameLine();
	if (ImGui::Button("Camera PERSPECTIVE"))
	{
		m_pRaylibEditor->ChangeCamera(EditorRaylib3D::CameraState::PERSPECTIVE);
	}



	//if (ImGui::Button("Show Axis X", ImVec2(50, 20)))
	//{
	//	m_pRaylibEditor->SetTranslateGizmo(all);
	//	m_pRaylibEditor->SetScaleGizmo(all);
	//	m_pRaylibEditor->SetRotateGizmo(all);
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Show Axis Y", ImVec2(50, 20)))
	//{
	//	m_pRaylibEditor->SetTranslateGizmo(translate);
	//}
	//ImGui::SameLine();
	//if (ImGui::Button("Show Axis Z", ImVec2(50, 20)))
	//{
	//	m_pRaylibEditor->SetScaleGizmo(scale);
	//}

	ImGui::End();
}

void EditorImGui::SaveSceneNoSpecialisation()
{
	if (m_haveFileSelected && !m_scenePathBuffer.empty())
	{
		bool saveAsNode = (m_pSelectedNode != nullptr && m_pSelectedNode != m_pSceneRoot);
		m_scenePathBuffer = NormalizeScenePath(m_scenePathBuffer, saveAsNode);

		m_command.type = EditorCommand::Type::SAVE_SCENE;
		m_command.stringParam1 = m_scenePathBuffer;
	}
	else
	{
		m_showSaveAsPopup = true;
	}
}

void EditorImGui::SelectedNode(Node* pNode)
{
	m_pSelectedNode = pNode;
	m_pRaylibEditor->SetSelectedNode(pNode->GetName());
	if (pNode)
	{
		DEBUG("[EditorImGui] Selected: " << pNode->GetName() << std::endl);
	}
}

void EditorImGui::NewNodeSelected(Node* pNode)
{
	m_pNewNodeTypeSelected = pNode;
	if (pNode)
	{
		DEBUG("[EditorImGui] Node type selected: " << pNode->GetName() << std::endl);
	}
}

json& EditorImGui::LoadInspectorData()
{
	m_selectedNodeData = SerializedObject();
	m_pSelectedNode->Serialize(m_selectedNodeData);
	m_selectedNodeDataJson = m_selectedNodeData.GetJson();

	DEBUG("[EditorImGui] Loaded inspector data for: " << m_pSelectedNode->GetName() << std::endl);

	return m_selectedNodeDataJson["PUBLIC_DATAS"];
}

void EditorImGui::ApplyInspectorChanges(json& datas)
{
	if (!m_pSelectedNode) return;

	m_selectedNodeDataJson["PUBLIC_DATAS"] = datas;

	m_selectedNodeData.SetJson(m_selectedNodeDataJson);

	json tempJson = m_selectedNodeDataJson;

	json cleanJson;
	cleanJson["PUBLIC_DATAS"] = tempJson["PUBLIC_DATAS"];
	cleanJson["PRIVATE_DATAS"] = tempJson["PRIVATE_DATAS"];
	cleanJson["PRIVATE_DATAS"]["Children"] = json::array();

	std::string oldName = m_pSelectedNode->GetName();
	m_selectedNodeData.SetJson(cleanJson);

	m_pSelectedNode->Deserialize(m_selectedNodeData);
	std::cout << "Name from m_sceneroor" << m_pSceneRoot->GetChild(0).GetName() << std::endl;

	if (oldName != m_pSelectedNode->GetName())
	{
		m_pRaylibEditor->UpdateElementName(oldName, m_pSelectedNode);
	}

	//DEBUG("[EditorImGui] Applied inspector changes" << std::endl);
}

void EditorImGui::BeginHierarchyDragSource(Node& node)
{
	if (node.GetParent() == nullptr)
	{
		return;
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		Node* payload = &node;
		ImGui::SetDragDropPayload(kHierarchyNodePayloadId, &payload, sizeof(Node*));
		ImGui::TextUnformatted(node.GetName().c_str());
		ImGui::EndDragDropSource();
	}
}

void EditorImGui::HandleHierarchyDropTarget(Node& targetNode)
{
	if (!ImGui::BeginDragDropTarget())
	{
		return;
	}

	if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload(kHierarchyNodePayloadId))
	{
		if (payload && payload->DataSize == sizeof(Node*))
		{
			Node* dragged = *static_cast<Node* const*>(payload->Data);
			bool const canDrop =
				dragged != nullptr &&
				dragged != &targetNode &&
				!IsDescendant(*dragged, targetNode) &&
				dragged->GetParent() != &targetNode;

			if (canDrop)
			{
				dragged->Reparent(targetNode, true);
				m_pRaylibEditor->UpdateDrawableElement(dragged);
				m_inspector.SetDirty();
				SelectedNode(dragged);
				dragged->Update(1.0f / 60.0f);
			}
		}
	}

	ImGui::EndDragDropTarget();
}

void EditorImGui::HandleHierarchyRootDropTarget()
{
	if (!m_pSceneRoot)
	{
		return;
	}

	if (!ImGui::BeginDragDropTarget())
	{
		return;
	}

	if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload(kHierarchyNodePayloadId))
	{
		if (payload && payload->DataSize == sizeof(Node*))
		{
			Node* dragged = *static_cast<Node* const*>(payload->Data);
			bool const canDrop =
				dragged != nullptr &&
				dragged != m_pSceneRoot &&
				dragged->GetParent() != m_pSceneRoot;

			if (canDrop)
			{
				dragged->Reparent(*m_pSceneRoot, true);
				SelectedNode(dragged);
			}
		}
	}

	ImGui::EndDragDropTarget();
}

bool EditorImGui::IsDescendant(Node const& potentialAncestor, Node const& node) const
{
	Node const* currentConst = &node;
	while (currentConst != nullptr)
	{
		if (currentConst == &potentialAncestor)
		{
			return true;
		}

		Node* current = const_cast<Node*>(currentConst);
		currentConst = current->GetParent();
	}

	return false;
}