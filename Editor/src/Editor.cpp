#include "Editor.h"
#include "SerializeObject.h"
#include "Servers/EngineServer.h"

#include <iostream>
#include <cstring>

Editor::Editor()
{
}

Editor::~Editor()
{
	Shutdown();
}

void Editor::Init() 
{
	// Initialize Raylib window
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
	InitWindow(m_screenWidth, m_screenHeight, "Foundry Editor");
	SetTargetFPS(144);

	// Setup ImGui
	rlImGuiSetup(true);
	
	ImGui::GetIO().FontGlobalScale = 1.0f;
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

	//Node Tree For Avaible Node
	m_newNodeTypeSelector = Node::CreateNode<Node>("Node");
	m_newNodeTypeSelector.get()->AddChild(Node::CreateNode<Node>("Node3DTest"));


	// DefaultNode
	m_sceneRoot = Node::CreateNode<Node>("SceneRoot");
	m_viewRoot = m_sceneRoot.get(); // Default view on the root
	
	ImGui::FileBrowser SaveBrowseWindow(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
	m_saveBrowser = SaveBrowseWindow;
	m_saveBrowser.SetDirectory("../res");
	m_loadBrowser.SetDirectory("../res");
	
	m_running = true;
	std::cout << "[Editor] Initialized successfully!" << std::endl;
}

void Editor::Run()
{
	while (m_running && !WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		Update(deltaTime);
		
		BeginDrawing();
		ClearBackground(DARKGRAY);

		Render3D();
		RenderUI();
		
		EndDrawing();
	}
}

void Editor::Shutdown()
{
	if (m_running)
	{
		rlImGuiShutdown();
		CloseWindow();
		m_running = false;
		std::cout << "[Editor] Shutdown successfully!" << std::endl;
	}
}

void Editor::Update(float deltaTime)
{
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S)) {
		SaveSceneNoSpe();
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_S)) {
		m_showSaveAsPopup = true;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_N)) {
		CreateNewScene();
	}

	EngineServer::FlushCommands();
}

void EditorRaylib3D::UpdateJson(json const& newJson)
{
}

void Editor::Render3D()
{
	// switch 2d/32
	m_editorRaylib.Render();
}

void Editor::RenderUI()
{
	rlImGuiBegin();

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
	
	ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "FPS: %d", GetFPS());
	ImGui::End();

	rlImGuiEnd();
}
void Editor::CreateNewScene() {
	m_sceneRoot = Node::CreateNode<Node>("SceneRoot");
	m_viewRoot = m_sceneRoot.get();
	m_selectedNode = nullptr;
	m_scenePathBuffer = "";
	m_haveFileSelected = false;
	std::cout << "[Editor] New scene created" << std::endl;
}

void Editor::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+N")) 
			{
				CreateNewScene();
			}
			
			if (ImGui::MenuItem("Load Scene", "Ctrl+O")) 
			{
				m_showLoadPopup = true;
			}

			ImGui::Separator();
			
			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) 
			{
				SaveSceneNoSpe();
			}
			if (ImGui::MenuItem("Save Scene As", "Ctrl+Shift+S"))
			{
				m_showSaveAsPopup = true;
			}
			
			ImGui::Separator();
			
			if (ImGui::MenuItem("Exit", "Alt+F4")) 
			{
				m_running = false;
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

void Editor::DrawHierarchyPanel()
{
	if (!m_showHierarchy) return;

	// On the left just below the menu bar
	float menuBarHeight = ImGui::GetFrameHeight();
	ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(350, m_screenHeight - menuBarHeight), ImGuiCond_Always);
	
	ImGui::Begin("Hierarchy", &m_showHierarchy, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

	if (m_viewRoot != m_sceneRoot.get())
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

	// Button to create New node
	if (ImGui::Button("+ Create Node", ImVec2(-1, 0)))
	{
		m_showCreatePopup = true;
		m_nodeNameBuffer[0] = '\0';
	}

	ImGui::Separator();

	// Display the current root
	if (m_viewRoot)
	{
		ImGui::TextColored(ImVec4(0.5f, 0.8f, 1.0f, 1.0f), "Viewing: %s", m_viewRoot->GetName().c_str());
		ImGui::Separator();
	}

	// the right click menu
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

void Editor::DrawNodeSelector(Node& node) {

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

void Editor::DrawHierarchyNodeTree(Node& node)
{

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
	                         | ImGuiTreeNodeFlags_SpanAvailWidth;

	// Highlight (selected)
	if (&node == m_selectedNode)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.5f, 0.8f, 0.8f));
	}

	// Sif no child == leaf
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

	// Context menu (right click)
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
				DeleteNode(&node);
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

void Editor::DrawInspectorPanel()
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
			std::cout << "[Editor] Node renamed to: " << nameBuffer << std::endl;
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
				DeleteNode(m_selectedNode);
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


void Editor::DrawViewport3D()
{
	if (!m_showViewport) return;
}

void Editor::CreateNode(std::string Type,std::string const& name, Node* parent)
{
	if (!m_sceneRoot) 
	{
		std::cerr << "[Editor] Cannot create node: no scene root" << std::endl;
		return;
	}
	uptr<Node> newNode = Node::CreateNode<Node>(name);
	//auto& typeinfo = typeid(m_newNodeTypeSelected);
	//dynamic_cast<newNode*>(typeinfo);

	if (parent)
	{
		parent->AddChild(newNode);
		std::cout << "[Editor] Node '" << name << "' added as child of '" 
		          << parent->GetName() << "'" << std::endl;
	}
	else if (m_selectedNode)
	{
		m_selectedNode->AddChild(newNode);
		std::cout << "[Editor] Node '" << name << "' added as child of '" 
		          << m_selectedNode->GetName() << "'" << std::endl;
	}
	else
	{
		m_viewRoot->AddChild(newNode);
		std::cout << "[Editor] Node '" << name << "' added to view root" << std::endl;
	}

	m_newNodeTypeSelected = nullptr;

}

void Editor::DeleteNode(Node* node)
{
	if (!node) return;

	std::string nodeName = node->GetName();
	
	// If deleting the view root, revert to the main root
	if (node == m_viewRoot)
	{
		ResetViewRoot();
	}
	
	if (node == m_selectedNode)
		m_selectedNode = nullptr;
		
	if (node && node->GetParent())
	{
		node->Destroy();
		std::cout << "[Editor] Node '" << nodeName << "' deleted" << std::endl;
	}
}
void Editor::SelectedNode(Node* node)
{

	m_selectedNode = node;
	if (node)
	{
		std::cout << "[Editor] Selected: " << node->GetName() << std::endl;
	}
}

void Editor::NewNodeSelected( Node* node)
{
	m_newNodeTypeSelected = node;

	if (node)
	{
		std::cout << "[Editor] Assigned: " << node->GetName() << std::endl;
	}
}

//
//
//void Editor::AssignNode(Node* ToNode ,Node* ThisNode)
//{
//
//	ToNode->SetName(ThisNode->GetName());
//
//	if (ThisNode)
//	{
//		std::cout << "[Editor] Assigned: " << ThisNode->GetName() << " to " << ToNode->GetName() << std::endl;
//	}
//}

void Editor::SetViewRoot(Node* node)
{
	if (node)
	{
		m_viewRoot = node;
		std::cout << "[Editor] View root changed to: " << node->GetName() << std::endl;
	}
}

void Editor::ResetViewRoot()
{
	m_viewRoot = m_sceneRoot.get();
	std::cout << "[Editor] View root reset to scene root" << std::endl;
}
/*
Todo:
add a research bar for all possible Nodes
add a tree to see all possible Nodes
-> Maybe use the DrawNodeTree with a node tree made in advance ? jsp
  
-> Wait for the Big Enum planned to be added
*/

void Editor::ShowCreateNodePopup()
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
				CreateNode(m_newNodeTypeSelected->GetName(),m_nodeNameBuffer);
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

void Editor::ShowCreateChildPopup(Node* parent)
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
				CreateNode(m_newNodeTypeSelected->GetName(), m_nodeNameBuffer, parent);
				m_pendingParent = nullptr;
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

void Editor::ShowCreateSiblingPopup(Node* sibling)
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
				CreateNode(m_newNodeTypeSelected->GetName(), m_nodeNameBuffer, sibling->GetParent());
				m_pendingSibling = nullptr;
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

void Editor::SaveSceneNoSpe() {
	if (m_haveFileSelected && m_scenePathBuffer.length() != 0) {
		SaveScene(m_scenePathBuffer);
	}
	else {
		m_showSaveAsPopup = true;
	}
}

void Editor::ShowSaveAsSceneBrowsing()
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
			SaveScene(m_scenePathBuffer);
		}
		m_saveBrowser.ClearSelected();
		m_saveBrowser.Close();
	}
}

void Editor::ShowLoadSceneBrowsing()
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
			LoadScene(m_scenePathBuffer);
		}
		m_loadBrowser.ClearSelected();
		m_loadBrowser.Close();
	}
}

void Editor::LoadScene(std::string const& path)
{
	try
	{
		m_sceneRoot = SerializeObject::LoadFromJson(path);
		m_viewRoot = m_sceneRoot.get();
		m_selectedNode = nullptr;
		std::cout << "[Editor] Scene loaded: " << path << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Editor] Failed to load scene: " << e.what() << std::endl;
	}
}

void Editor::SaveScene(std::string const& path)
{
	try
	{
		if (m_sceneRoot)
		{
			SerializeObject::Save(path, m_sceneRoot);
			std::cout << "[Editor] Scene saved: " << path << std::endl;
		}
		else
		{
			std::cerr << "[Editor] Cannot save: no scene loaded" << std::endl;
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Editor] Failed to save scene: " << e.what() << std::endl;
	}
}