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

	// DefaultNode
	m_sceneRoot = Node::CreateNode<Node>("SceneRoot");

	rlImGuiSetup(true);

	m_editorImgui.Init();
	m_editorImgui.SetSceneRoot(m_sceneRoot.get());
	m_editorImgui.SetScreenSize(m_screenWidth, m_screenHeight);

	// Static Cam 
	m_camera.position = { 10.0f, 10.0f, 10.0f };
	m_camera.target = { 0.0f, 0.0f, 0.0f };
	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 45.0f;
	m_camera.projection = CAMERA_PERSPECTIVE;

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
	// Camera controls
	if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) 
	{
		UpdateCamera(&m_camera, CAMERA_FREE);
	}

	// Keyboard shortcuts
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) 
	{
		if (IsKeyDown(KEY_LEFT_SHIFT))
		{
			m_editorImgui.ShowSaveAs();
		}
		else
		{
			SaveSceneNoSpe();
		}
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_N)) 
	{
		CreateNewScene();
	}

	ProcessUICommands();

	EngineServer::FlushCommands();
}

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
	DrawViewport3D();
}

void Editor::RenderUI()
{
	rlImGuiBegin();
	m_editorImgui.Render();
	rlImGuiEnd();
}

void Editor::DrawViewport3D()
{
	BeginMode3D(m_camera);

	DrawGrid(20, 1.0f);
	DrawLine3D({ 0, 0, 0 }, { 500, 0, 0 }, RED);
	DrawLine3D({ 0, 0, 0 }, { 0, 500, 0 }, GREEN);
	DrawLine3D({ 0, 0, 0 }, { 0, 0, 500 }, BLUE);
	DrawLine3D({ 0, 0, 0 }, { -500, 0, 0 }, RED);
	DrawLine3D({ 0, 0, 0 }, { 0, -500, 0 }, GREEN);
	DrawLine3D({ 0, 0, 0 }, { 0, 0, -500 }, BLUE);

	EndMode3D();
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

	uptr<Node> newNode = Node::CreateNode<Node>(name);

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
		m_sceneRoot = SerializeObject::LoadFromJson(path);
		m_editorImgui.SetSceneRoot(m_sceneRoot.get());
		m_scenePathBuffer = path;
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