#include "EditorImGui.h"
#include <iostream>

EditorImGui::EditorImGui()
{
}

EditorImGui::~EditorImGui()
{
}

void EditorImGui::Init() 
{
	//Node Tree For Available Nodes
	m_newNodeTypeSelector = Node::CreateNode<Node>("Node");
	m_newNodeTypeSelector.get()->AddChild(Node::CreateNode<Node>("Node3DTest"));
	
	ImGui::GetIO().FontGlobalScale = 1.0f;
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::FileBrowser SaveBrowseWindow(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
	
	m_saveBrowser = SaveBrowseWindow;
	m_saveBrowser.SetDirectory("../res");
	m_loadBrowser.SetDirectory("../res");
}

void EditorImGui::SetSceneRoot(Node* root)
{
	m_sceneRoot = root;
	if (!m_viewRoot || m_viewRoot == m_sceneRoot)
	{
		m_viewRoot = root;
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

	// Popups Node
	ShowCreateNodePopup();
	ShowCreateChildPopup(m_pendingParent);
	ShowCreateSiblingPopup(m_pendingSibling);

	// File Browse 
	ShowSaveAsSceneBrowsing();
	ShowLoadSceneBrowsing();

	ImGui::SetNextWindowPos(ImVec2(m_screenWidth - 120.0f, m_screenHeight - 40.0f));
	ImGui::SetNextWindowSize(ImVec2(110, 30));
	ImGui::Begin("##FPS", nullptr, 
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoScrollbar | 
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBackground);
	
	//needs access to GetFPS() from Raylib
	// ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS: %d", GetFPS());
	ImGui::End();
}

void EditorImGui::DrawInspectorPanel()
{
	if (!m_showInspector) return;

	// Position to the right
	float menuBarHeight = ImGui::GetFrameHeight();
	ImGui::SetNextWindowPos(ImVec2(m_screenWidth - 400, menuBarHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(400, m_screenHeight - menuBarHeight), ImGuiCond_Always);

	ImGui::Begin("Inspector", &m_showInspector, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (m_selectedNode)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 1.0f, 1.0f));
		ImGui::Text("Node Properties");
		ImGui::PopStyleColor();
		ImGui::Separator();

		// editable node name
		char nameBuffer[128];
		strncpy_s(nameBuffer, m_selectedNode->GetName().c_str(), sizeof(nameBuffer));

		ImGui::Spacing();
		ImGui::Text("Name :");

		ImGui::SameLine();
		if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer)))
		{
			m_selectedNode->SetName(nameBuffer);
			std::cout << "[EditorImGui] Node renamed to: " << nameBuffer << std::endl;
		}

		ImGui::Spacing();

		ImGui::Text("Parent: %s",
			m_selectedNode->GetParent()
			? m_selectedNode->GetParent()->GetName().c_str()
			: "None");

		ImGui::Text("Children Count: %d", m_selectedNode->GetChildCount());

		ImGui::Separator();
		ImGui::Spacing();

		bool isSceneRoot = (m_selectedNode->GetParent() == nullptr);

		if (isSceneRoot)
		{
			ImGui::BeginDisabled();
		}

		if (ImGui::Button("Delete Node", ImVec2(-1, 0)))
		{
			if (!isSceneRoot)
			{
				m_command.type = EditorCommand::Type::DELETE_NODE;
				m_command.nodeParam = m_selectedNode;
				m_selectedNode = nullptr;
			}
		}

		if (isSceneRoot)
		{
			ImGui::EndDisabled();
			ImGui::TextDisabled("Cannot delete scene root");
		}
	}
	else
	{
		ImGui::TextDisabled("No node selected");
		ImGui::Spacing();
		ImGui::TextWrapped("Select a node in the Hierarchy panel to view and edit its properties.");
	}

	ImGui::End();
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
				m_selectedNode = nullptr;
				m_viewRoot = m_sceneRoot;
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

		ImGui::EndMainMenuBar();
	}
}

void EditorImGui::DrawHierarchyPanel()
{
	if (!m_showHierarchy) return;

	// On the left just below the menu bar
	float menuBarHeight = ImGui::GetFrameHeight();
	ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(350, m_screenHeight - menuBarHeight), ImGuiCond_Always);

	ImGui::Begin("Hierarchy", &m_showHierarchy, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (m_viewRoot != m_sceneRoot)
	{
		// Button go back to parent
		if (m_viewRoot->GetParent() != nullptr)
		{
			if (ImGui::Button("< Parent"))
			{
				SetViewRoot(m_viewRoot->GetParent());
			}
			ImGui::SameLine();
		}

		// Button go back to root
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

	if (m_viewRoot)
	{
		ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "Viewing: %s", m_viewRoot->GetName().c_str());
		ImGui::Separator();
	}

	if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
	{
		if (ImGui::MenuItem("Add Child to Root"))
		{
			m_pendingParent = m_viewRoot;
			m_showCreateChildPopup = true;
			m_nodeNameBuffer[0] = '\0';
		}
		ImGui::EndPopup();
	}

	// Tree
	if (m_viewRoot)
	{
		DrawHierarchyNodeTree(*m_viewRoot);
	}
	else
	{
		ImGui::TextDisabled("No scene loaded");
	}

	ImGui::End();
}

void EditorImGui::DrawNodeSelector(Node& node) 
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ImGuiTreeNodeFlags_SpanAvailWidth;

	if (&node == m_newNodeTypeSelected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.5f, 0.8f, 0.8f));
	}

	if (node.GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx(node.GetName().c_str(), flags);

	if (&node == m_newNodeTypeSelected)
		ImGui::PopStyleColor();

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		NewNodeSelected(&node);
	}

	// Recursively draw children if open
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
		| ImGuiTreeNodeFlags_SpanAvailWidth;

	if (&node == m_selectedNode)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.5f, 0.8f, 0.8f));
	}

	// If no child == leaf
	if (node.GetChildCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool nodeOpen = ImGui::TreeNodeEx(node.GetName().c_str(), flags);

	if (&node == m_selectedNode)
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
			m_pendingParent = &node;
			m_showCreateChildPopup = true;
			m_nodeNameBuffer[0] = '\0';
		}

		// No sibling for default root 
		if (ImGui::MenuItem("Add Sibling", nullptr, false, !isSceneRoot))
		{
			if (!isSceneRoot)
			{
				m_pendingSibling = &node;
				m_showCreateSiblingPopup = true;
				m_nodeNameBuffer[0] = '\0';
			}
		}

		// no duplicate for default root
		ImGui::BeginDisabled(isSceneRoot);
		if (ImGui::MenuItem("Duplicate"))
		{
			if (!isSceneRoot)
			{
				auto clone = node.Clone();
				if (node.GetParent())
				{
					node.GetParent()->AddChild(clone);
				}
			}
		}
		ImGui::EndDisabled();

		if (ImGui::MenuItem("Set as View Root"))
		{
			SetViewRoot(&node);
		}

		ImGui::Separator();

		// no delete for default root
		if (ImGui::MenuItem("Delete", nullptr, false, !isSceneRoot))
		{
			if (!isSceneRoot)
			{
				m_command.type = EditorCommand::Type::DELETE_NODE;
				m_command.nodeParam = &node;
			}
		}

		ImGui::EndPopup();
	}
	// Recursively draw children if open
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
		m_viewRoot = node;
		std::cout << "[EditorImGui] View root changed to: " << node->GetName() << std::endl;
	}
}

void EditorImGui::ResetViewRoot()
{
	m_viewRoot = m_sceneRoot;
	std::cout << "[EditorImGui] View root reset to scene root" << std::endl;
}

void EditorImGui::ShowCreateNodePopup()
{
	if (m_showCreatePopup)
	{
		ImGui::OpenPopup("Create Node");
		m_showCreatePopup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create Node", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		DrawNodeSelector(*m_newNodeTypeSelector);

		ImGui::Text("Enter node name:");
		ImGui::Spacing();

		bool enterPressed = ImGui::InputText("##NodeName", m_nodeNameBuffer, sizeof(m_nodeNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Spacing();

		if ((ImGui::Button("Create", ImVec2(120, 0)) || enterPressed) && m_newNodeTypeSelected != nullptr)
		{
			if (strlen(m_nodeNameBuffer) > 0)
			{
				m_command.type = EditorCommand::Type::CREATE_NODE;
				m_command.stringParam1 = m_newNodeTypeSelected->GetName();
				m_command.stringParam2 = m_nodeNameBuffer;
				m_command.nodeParam = nullptr;
				m_newNodeTypeSelected = nullptr;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void EditorImGui::ShowCreateChildPopup(Node* parent)
{
	if (m_showCreateChildPopup)
	{
		ImGui::OpenPopup("Create Child Node");
		m_showCreateChildPopup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create Child Node", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (parent)
		{
			ImGui::Text("Add child to: %s", parent->GetName().c_str());
			ImGui::Spacing();
		}

		DrawNodeSelector(*m_newNodeTypeSelector);

		ImGui::Text("Enter node name:");
		ImGui::Spacing();

		bool enterPressed = ImGui::InputText("##ChildName", m_nodeNameBuffer, sizeof(m_nodeNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Spacing();

		if ((ImGui::Button("Create", ImVec2(120, 0)) || enterPressed) && m_newNodeTypeSelected != nullptr)
		{
			if (strlen(m_nodeNameBuffer) > 0 && parent)
			{
				m_command.type = EditorCommand::Type::CREATE_NODE;
				m_command.stringParam1 = m_newNodeTypeSelected->GetName();
				m_command.stringParam2 = m_nodeNameBuffer;
				m_command.nodeParam = parent;
				m_pendingParent = nullptr;
				m_newNodeTypeSelected = nullptr;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_pendingParent = nullptr;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void EditorImGui::ShowCreateSiblingPopup(Node* sibling)
{
	if (m_showCreateSiblingPopup)
	{
		ImGui::OpenPopup("Create Sibling Node");
		m_showCreateSiblingPopup = false;
	}

	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Create Sibling Node", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (sibling)
		{
			ImGui::Text("Add sibling to: %s", sibling->GetName().c_str());
			ImGui::Spacing();
		}

		DrawNodeSelector(*m_newNodeTypeSelector);

		ImGui::Text("Enter node name:");
		ImGui::Spacing();

		bool enterPressed = ImGui::InputText("##SiblingName", m_nodeNameBuffer, sizeof(m_nodeNameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::Spacing();

		if ((ImGui::Button("Create", ImVec2(120, 0)) || enterPressed) && m_newNodeTypeSelected != nullptr)
		{
			if (strlen(m_nodeNameBuffer) > 0 && sibling && sibling->GetParent())
			{
				m_command.type = EditorCommand::Type::CREATE_NODE;
				m_command.stringParam1 = m_newNodeTypeSelected->GetName();
				m_command.stringParam2 = m_nodeNameBuffer;
				m_command.nodeParam = sibling->GetParent();
				m_pendingSibling = nullptr;
				m_newNodeTypeSelected = nullptr;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_pendingSibling = nullptr;
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

	m_saveBrowser.SetTitle("Save scene to Json file");
	m_saveBrowser.SetTypeFilters({ ".json" });
	m_saveBrowser.Display();

	if (m_saveBrowser.HasSelected())
	{
		m_scenePathBuffer = m_saveBrowser.GetSelected().string();
		if (m_scenePathBuffer.length() > 0)
		{
			m_haveFileSelected = true;
			m_command.type = EditorCommand::Type::SAVE_SCENE;
			m_command.stringParam1 = m_scenePathBuffer;
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

	m_loadBrowser.SetTitle("Load scene from Json file");
	m_loadBrowser.SetTypeFilters({ ".json" });
	m_loadBrowser.Display();

	if (m_loadBrowser.HasSelected())
	{
		m_scenePathBuffer = m_loadBrowser.GetSelected().string();
		if (m_scenePathBuffer.length() > 0)
		{
			m_haveFileSelected = true;
			m_command.type = EditorCommand::Type::LOAD_SCENE;
			m_command.stringParam1 = m_scenePathBuffer;
			m_selectedNode = nullptr;
			m_viewRoot = m_sceneRoot;
		}
		m_loadBrowser.ClearSelected();
		m_loadBrowser.Close();
	}
}

void EditorImGui::SaveSceneNoSpecialisation()
{
	if (m_haveFileSelected && m_scenePathBuffer.length() != 0) 
	{
		m_command.type = EditorCommand::Type::SAVE_SCENE;
		m_command.stringParam1 = m_scenePathBuffer;
	}
	else 
	{
		m_showSaveAsPopup = true;
	}
}

void EditorImGui::SelectedNode(Node* node)
{
	m_selectedNode = node;
	if (node)
	{
		std::cout << "[EditorImGui] Selected: " << node->GetName() << std::endl;
	}
}

void EditorImGui::NewNodeSelected(Node* node)
{
	m_newNodeTypeSelected = node;
	if (node)
	{
		std::cout << "[EditorImGui] Node type selected: " << node->GetName() << std::endl;
	}
}