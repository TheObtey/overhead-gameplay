#include "Editor.h"
#include "EditorSerializer.h"

#include <Servers/EngineServer.h>
#include <Serialization/SerializeObject.hpp>
#include <iostream>
#include <cstring>
using namespace rl;
#include <rlImGui.h>
#include <rlImGuiColors.h>

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
	m_editorRaylib.InitWindow(m_screenWidth, m_screenHeight);

	// DefaultNode
	m_sceneRoot = Node::CreateNode<Node>("SceneRoot");

	rlImGuiSetup(true);

	m_editorImgui.Init();
	m_editorImgui.SetSceneRoot(m_sceneRoot.get());
	m_editorImgui.SetScreenSize(m_screenWidth, m_screenHeight);


	m_running = true;
	std::cout << "[Editor] Initialized successfully!" << std::endl;
}

void Editor::Run()
{
	while (m_running && !rl::WindowShouldClose())
	{
		float deltaTime = rl::GetFrameTime();
		Update(deltaTime);
		
		rl::BeginDrawing();
		rl::ClearBackground(rl::DARKGRAY);

		Render3D();
		RenderUI();
		
		rl::EndDrawing();
	}
}

void Editor::Shutdown()
{
	if (m_running)
	{
		rlImGuiShutdown();
		rl::CloseWindow();
		m_running = false;
		std::cout << "[Editor] Shutdown successfully!" << std::endl;
	}
}

void Editor::Update(float deltaTime)
{

	// Keyboard shortcuts
	if (rl::IsKeyDown(rl::KEY_LEFT_CONTROL) && rl::IsKeyPressed(rl::KEY_S))
	{
		if (rl::IsKeyDown(rl::KEY_LEFT_SHIFT))
		{
			m_editorImgui.ShowSaveAs();
		}
		else
		{
			SaveSceneNoSpe();
		}
	}

	if (rl::IsKeyDown(rl::KEY_LEFT_CONTROL) && rl::IsKeyPressed(rl::KEY_N))
	{
		CreateNewScene();
	}

	ProcessUICommands();

	EngineServer::FlushCommands();
}

// void EditorRaylib3D::UpdateJson(json const& newJson)
// {}

void Editor::ProcessUICommands()
{
	if (!m_editorImgui.HasCommand())
		return;

	EditorCommand& cmd = m_editorImgui.GetCommand();

	switch (cmd.type)
	{
	case EditorCommand::Type::CREATE_NODE:
		CreateNode(cmd.stringParam1, cmd.stringParam2, cmd.nodeParam);
		break;

	case EditorCommand::Type::DELETE_NODE:
		DeleteNode(cmd.nodeParam);
		break;

	case EditorCommand::Type::CREATE_NEW_SCENE:
		CreateNewScene();
		break;

	case EditorCommand::Type::SAVE_SCENE:
		SaveScene(cmd.stringParam1);
		break;

	case EditorCommand::Type::LOAD_SCENE:
		LoadScene(cmd.stringParam1);
		break;

	case EditorCommand::Type::EXIT_EDITOR:
		m_running = false;
		break;

	default:
		break;
	}


	cmd.Reset();
}


void Editor::Render3D()
{
	// switch 2d/32
	m_editorRaylib.Render();
}

void Editor::RenderUI()
{
	rlImGuiBegin();
	m_editorImgui.Render();
	rlImGuiEnd();
}

void Editor::CreateNewScene() 
{
	m_sceneRoot = Node::CreateNode<Node>("SceneRoot");
	m_editorImgui.SetSceneRoot(m_sceneRoot.get());
	m_scenePathBuffer = "";
	std::cout << "[Editor] New scene created" << std::endl;
}

void Editor::CreateNode(std::string type, std::string const& name, Node* parent)
{
	if (!m_sceneRoot) 
	{
		std::cerr << "[Editor] Cannot create node: no scene root" << std::endl;
		return;
	}
	std::string typefi = "class " + type;
	ISerializable* outObject = ISerializable::s_constructors[typefi]();
	uptr<Node> newNode = uptr<Node>(static_cast<Node*>(outObject));
	newNode.get()->SetName(name);
	m_editorRaylib.AddDrawableObject(name, newNode.get());

	if (parent)
	{
		parent->AddChild(newNode);
		std::cout << "[Editor] Node '" << name << "' added as child of '" 
		          << parent->GetName() << "'" << std::endl;
	}
	else
	{
		m_sceneRoot->AddChild(newNode);
		std::cout << "[Editor] Node '" << name << "' added to scene root" << std::endl;
	}
}

void Editor::DeleteNode(Node* node)
{
	if (!node) return;

	std::string nodeName = node->GetName();
	
	if (node && node->GetParent())
	{
		node->Destroy();
		std::cout << "[Editor] Node '" << nodeName << "' deleted" << std::endl;
	}
}

void Editor::LoadScene(std::string const& path)
{
	try
	{
		m_sceneRoot = EditorSerializer::LoadFromJson(path);
		m_editorImgui.SetSceneRoot(m_sceneRoot.get());
		m_scenePathBuffer = path;

		// Update NodeList
		LoadDrawableObject(m_sceneRoot.get());
		
		std::cout << "[Editor] Scene loaded: " << path << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Editor] Failed to load scene: " << e.what() << std::endl;
	}
}

void Editor::LoadDrawableObject(Node* pNode)
{
	m_editorRaylib.AddDrawableObject(pNode->GetName(), pNode);
	for (uint32 i = 0; i < pNode->GetChildCount(); i++)
	{
		LoadDrawableObject(&pNode->GetChild(i));
	}
}

void Editor::SaveScene(std::string const& path)
{
	try
	{
		if (m_sceneRoot)
		{
			EditorSerializer::Save(path, m_sceneRoot);
			m_scenePathBuffer = path;
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

void Editor::SaveSceneNoSpe() 
{
	if (!m_scenePathBuffer.empty()) 
	{
		SaveScene(m_scenePathBuffer);
	}
	else 
	{
		m_editorImgui.ShowSaveAs();
	}
}
