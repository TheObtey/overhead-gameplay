#include "Editor.h"
#include "EditorSerializer.h"
#include "Debug.h"

#include <Servers/EngineServer.h>
#include <Servers/PhysicsServer.h>
#include <Servers/GraphicServer.h>
#include <Servers/AudioServer.h>

#include <Serialization/SerializeObject.hpp>
#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <fstream>

#include <rlImGui.h>
#include <rlImGuiColors.h>
#include <Nodes/Node3D.h>
#include <Nodes/NodeWindow.h>

#if OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS

extern "C" __declspec(dllimport)
int __stdcall SetFileAttributesW(const wchar_t* lpFileName, unsigned long dwFileAttributes);
constexpr unsigned long FILE_ATTRIBUTE_HIDDEN = 0x2;

#endif


Editor::Editor() : m_editorRaylib() ,m_editorImgui(this,&m_editorRaylib)
{}

Editor::~Editor()
{

	Node::SetStatusEditor(false);
	Shutdown();
}

void Editor::Init() 
{
	Node::SetStatusEditor(true);
	m_editorRaylib.Init(m_screenWidth, m_screenHeight);
	PhysicsServer::Initialize();

	//m_hiddenWindowContext = Node::CreateNode<NodeWindow>("EditorHiddenWindow");
	//GraphicServer::Initialize();
	//m_hiddenWindowViewport = Node::CreateNode<NodeViewport>("EditorHiddenViewport");
	//m_hiddenWindowContext->AddChild(std::move(m_hiddenWindowViewport));
	//EngineServer::FlushCommands();
	//GraphicServer::FlushCommands();

	m_sceneRoot = Node::CreateNode<Node>("SceneRoot");

	rlImGuiSetup(true);

	m_editorImgui.Init();
	m_editorImgui.SetSceneRoot(m_sceneRoot.get());
	m_editorImgui.SetScreenSize(m_screenWidth, m_screenHeight);

	std::filesystem::path scriptStockDir = "ScriptStock";
	std::filesystem::create_directories(scriptStockDir);

	std::filesystem::path source = "../Game/res/scripts/.foundry";
	std::filesystem::path dest = scriptStockDir / ".foundry";

	std::error_code ec;

	if (!std::filesystem::exists(source, ec) || !std::filesystem::is_directory(source, ec))
	{
		DEBUG("[Editor] WARNING: Source .foundry directory not found: " << source.string() << std::endl);
	}
	else
	{
		if (std::filesystem::exists(dest, ec))
		{
			std::filesystem::remove_all(dest, ec);
			ec.clear();
		}

		std::filesystem::copy(
			source,
			dest,
			std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing,
			ec);

		if (ec)
		{
			DEBUG("[Editor] WARNING: Couldn't copy directory "
				<< source.string() << " to " << dest.string()
				<< " : " << ec.message() << std::endl);
		}
#if OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
		else
		{
			::SetFileAttributesW(dest.wstring().c_str(), FILE_ATTRIBUTE_HIDDEN);
		}
#endif
	}

	m_running = true;
	DEBUG("[Editor] Initialized successfully!" << std::endl);
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
		DEBUG( "[Editor] Shutdown successfully!" << std::endl);
	}
}

void Editor::Update(float deltaTime)
{
	m_sceneRoot->Update(deltaTime);
	m_editorRaylib.Update(deltaTime);
	m_editorRaylib.UpdateDisplay(m_sceneRoot.get());
	//m_hiddenWindowContext->OnUpdate(deltaTime);
	// Keyboard shortcuts
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
	{
		if (IsKeyDown(KEY_LEFT_SHIFT))
		{
			m_editorImgui.ShowSaveAs(false);
		}
		else
		{
			SaveSceneNoSpe();
		}
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O)) {
		m_editorImgui.ShowLoadPopup();

	}
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_N))
	{
		CreateNewScene();
	}

	ProcessUICommands();

	EngineServer::FlushCommands();
	//GraphicServer::FlushCommands();
	PhysicsServer::FlushCommands();
	AudioServer::FlushCommands();
}


void Editor::ProcessUICommands()
{
	if (!m_editorImgui.HasCommand())
		return;

	EditorCommand& cmd = m_editorImgui.GetCommand();

	switch (cmd.type)
	{
	case EditorCommand::Type::CREATE_NODE:
		CreateNode(cmd.stringParam1, cmd.stringParam2, cmd.pNodeParam);
		break;

	case EditorCommand::Type::DELETE_NODE:
		DeleteNode(cmd.pNodeParam);
		break;

	case EditorCommand::Type::CREATE_NEW_SCENE:
		CreateNewScene();
		break;

	case EditorCommand::Type::SAVE_SCENE:
		SaveScene(cmd.stringParam1);
		break;

	case EditorCommand::Type::LOAD_SCENE:
		LoadScene(cmd.stringParam1);
		PhysicsServer::FlushCommands();
		break;

	case EditorCommand::Type::LUNCH_GAME:
		StartFoundry(cmd.stringParam1);
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
	m_editorRaylib.ClearWindow();
	m_editorImgui.SetSceneRoot(m_sceneRoot.get());
	m_editorImgui.ResetViewRoot();
	m_editorImgui.ResetSelectedNode();
	
	m_scenePathBuffer = "";
	DEBUG( "[Editor] New scene created" << std::endl);
}

void Editor::CreateNode(std::string const& type, std::string const& name, Node* pParent)
{
	if (!m_sceneRoot) 
	{
		std::cerr << "[Editor] Cannot create node: no scene root" << std::endl;
		return;
	}

	ISerializable* outObject = AutomaticRegisterISerializable<ISerializable>::create(type);
	uptr<Node> newNode = uptr<Node>(static_cast<Node*>(outObject));
	newNode.get()->SetName(name);
	if (pParent)
	{

		pParent->AddChild(newNode);
		DEBUG( "[Editor] Node '" << name << "' added as child of '" 
		          << pParent->GetName() << "'" << std::endl);
	}
	else
	{
		m_sceneRoot->AddChild(newNode);
		DEBUG( "[Editor] Node '" << name << "' added to scene root" << std::endl);
	}

	m_editorRaylib.AddDrawableObject(static_cast<Node*>(outObject));
}

void Editor::LoadDrawableObject(Node* pNode)
{
	m_editorRaylib.AddDrawableObject(pNode);
	for (uint32 i = 0; i < pNode->GetChildCount(); i++)
	{
		LoadDrawableObject(&pNode->GetChild(i));
	}
}

void Editor::RemoveDrawableRecursive(Node* pNode)
{
	if (pNode == nullptr) return;

	for (uint32 i = 0; i < pNode->GetChildCount(); ++i)
	{
		RemoveDrawableRecursive(&pNode->GetChild(i));
	}

	m_editorRaylib.RemoveDrawableElement(pNode);
}

void Editor::DeleteNode(Node* pNode)
{
	if (!pNode) return;

	std::string const nodeName = pNode->GetName();

	m_editorImgui.NotifyNodeWillBeDeleted(pNode);
	RemoveDrawableRecursive(pNode);

	if (pNode->GetParent())
	{
		pNode->Destroy();
		DEBUG("[Editor] Node '" << nodeName << "' deleted" << std::endl);
	}
}

void Editor::LoadScene(std::string const& path)
{
	try
	{
		LoadReturn tmp = EditorSerializer::LoadFromJson(path);
		if (tmp.IsRoot) {
			CreateNewScene();
			m_sceneRoot = std::move(tmp.uptrNode);
			m_editorImgui.SetSceneRoot(m_sceneRoot.get());
			m_editorImgui.ResetViewRoot();
			m_editorImgui.ResetSelectedNode();
			m_scenePathBuffer = path;
			// Update NodeList
			LoadDrawableObject(m_sceneRoot.get());
			DEBUG( "[Editor] Scene Root loaded: " << path << std::endl);
		}
		else if (tmp.IsRoot == false) {
			EngineServer::FlushCommands();
			m_sceneRoot.get()->AddChild(tmp.uptrNode);
			EngineServer::FlushCommands();

			m_editorImgui.ResetViewRoot();
			m_editorImgui.ResetSelectedNode();
			m_nodePathBuffer = path;

			LoadDrawableObject(m_sceneRoot.get());
			DEBUG("[Editor] Node loaded: " << path << std::endl);
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Editor] Failed to load scene: " << e.what() << std::endl;
	}
}

void Editor::StartFoundry(std::string const& scenePath)
{
	(void)scenePath;
	{
		std::filesystem::path const sourceScriptsDir = "res/scripts";
		std::filesystem::path const scriptStockDir = "ScriptStock";
		std::error_code ec;

		std::filesystem::create_directories(scriptStockDir, ec);
		ec.clear();

		if (std::filesystem::exists(sourceScriptsDir, ec) && std::filesystem::is_directory(sourceScriptsDir, ec))
		{
			std::filesystem::copy(
				sourceScriptsDir,
				scriptStockDir,
				std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing,
				ec);

			if (ec)
			{
				std::cerr << "[Editor] Failed to sync res/scripts to ScriptStock: " << ec.message() << std::endl;
			}
		}
		else
		{
			std::cerr << "[Editor] Scripts source folder not found: " << sourceScriptsDir << std::endl;
		}
	}


	std::filesystem::path gameExePath;

#if OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
	gameExePath = "../Game/Game.exe";
#elif OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX
	gameExePath = "../Game/Game";
#elif OPERATING_SYSTEM == OPERATING_SYSTEM_MACOS
	gameExePath = "../Game/Game.app/Contents/MacOS/Game";
	if (!std::filesystem::exists(gameExePath))
	{
		gameExePath = "../Game/Game";
	}
#else
    #error "Unsupported platform for launching the game."
#endif

	if (!std::filesystem::exists(gameExePath))
	{
		std::cerr << "[Editor] Game executable not found: " << gameExePath << std::endl;
		std::cerr << "[Editor] Make sure to build the Game project first!" << std::endl;
		return;
	}

	std::filesystem::path absoluteGamePath = std::filesystem::absolute(gameExePath);

	std::string command;

#if OPERATING_SYSTEM == OPERATING_SYSTEM_WINDOWS
	command = "start \"Foundry Game\" \"" + absoluteGamePath.string() + "\"";
#elif OPERATING_SYSTEM == OPERATING_SYSTEM_LINUX || OPERATING_SYSTEM == OPERATING_SYSTEM_MACOS
	command = "\"" + absoluteGamePath.string() + "\" &";
#else
    #error "Unsupported platform for launching the game."
#endif

	DEBUG("[Editor] Executing: " << command << std::endl);
	int result = std::system(command.c_str());

	if (result == 0)
		DEBUG("[Editor] Game launched successfully" << std::endl);
	else
		std::cerr << "[Editor] Failed to launch game (error code: " << result << ")" << std::endl;
}

void Editor::CollectLuaScripts(Node* pNode, std::vector<std::filesystem::path>& outScripts)
{
	if (!pNode) return;

	SerializedObject object;
	pNode->Serialize(object);

	auto const& data = object.GetJson();
	if (data.contains("PUBLIC_DATAS") && data["PUBLIC_DATAS"].contains("m_scriptPath"))
	{
		std::string scriptPath = data["PUBLIC_DATAS"]["m_scriptPath"];
		if (!scriptPath.empty())
		{
			outScripts.emplace_back(scriptPath);
		}
	}

	for (uint32 i = 0; i < pNode->GetChildCount(); ++i)
	{
		CollectLuaScripts(&pNode->GetChild(i), outScripts);
	}
}

Editor::ScriptPathMap Editor::CopyScriptIntoGame(std::filesystem::path const& scenePath, std::filesystem::path const& gameExePath)
{
	ScriptPathMap scriptMap;

	if (!m_sceneRoot) return scriptMap;

	std::filesystem::path scriptsDir = gameExePath.parent_path() / "Overhead" / "Scripts";
	std::filesystem::create_directories(scriptsDir);

	std::vector<std::filesystem::path> scripts;
	CollectLuaScripts(m_sceneRoot.get(), scripts);

	std::unordered_set<std::string> uniqueScripts;

	for (std::filesystem::path const& scriptPath : scripts)
	{
		std::filesystem::path resolvedPath;
		bool found = false;

		auto tryResolve = [&](std::filesystem::path const& candidate)
		{
			if (!found && std::filesystem::exists(candidate))
			{
				resolvedPath = std::filesystem::weakly_canonical(candidate);
				found = true;
			}
		};

		if (scriptPath.is_absolute())
		{
			tryResolve(scriptPath);
		}
		else
		{
			tryResolve(std::filesystem::absolute(scriptPath));
			tryResolve(std::filesystem::absolute(scenePath.parent_path() / scriptPath));
			tryResolve(std::filesystem::absolute(gameExePath.parent_path() / scriptPath));
		}

		if (!found)
		{
			std::cerr << "[Editor] Lua script not found: " << scriptPath << std::endl;
			continue;
		}

		if (!uniqueScripts.insert(resolvedPath.string()).second)
			continue;

		std::filesystem::path targetPath = scriptsDir / resolvedPath.filename();
		std::filesystem::copy_file(resolvedPath, targetPath, std::filesystem::copy_options::overwrite_existing);

		scriptMap[scriptPath.string()] = targetPath.string();
		scriptMap[resolvedPath.string()] = targetPath.string();
	}

	return scriptMap;
}

bool Editor::WritePlayScene(std::filesystem::path const& outputScenePath, ScriptPathMap const& scriptMap)
{
	try
	{
		if (!m_sceneRoot)
		{
			std::cerr << "[Editor] Cannot write play scene: no scene loaded" << std::endl;
			return false;
		}

		SerializedObject object;
		m_sceneRoot->Serialize(object);

		json jsonRoot = json::array();
		jsonRoot[0]["Root"] = object.GetJson();

		UpdateScriptPathsInJson(jsonRoot[0]["Root"], scriptMap);

		std::filesystem::create_directories(outputScenePath.parent_path());
		std::ofstream file(outputScenePath, std::ios::out | std::ios::trunc);
		file << jsonRoot;
		file.close();

		DEBUG( "[Editor] Play scene exported: " << outputScenePath << std::endl);
		return true;
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Editor] Failed to write play scene: " << e.what() << std::endl;
		return false;
	}
}

void Editor::UpdateScriptPathsInJson(json& nodeJson, ScriptPathMap const& scriptMap)
{
	if (nodeJson.contains("PUBLIC_DATAS") && nodeJson["PUBLIC_DATAS"].contains("m_scriptPath"))
	{
		std::string scriptPath = nodeJson["PUBLIC_DATAS"]["m_scriptPath"];
		if (!scriptPath.empty())
		{
			auto it = scriptMap.find(scriptPath);
			if (it != scriptMap.end())
			{
				nodeJson["PUBLIC_DATAS"]["m_scriptPath"] = it->second;
			}
		}
	}

	if (nodeJson.contains("PRIVATE_DATAS") && nodeJson["PRIVATE_DATAS"].contains("Children"))
	{
		for (auto& child : nodeJson["PRIVATE_DATAS"]["Children"])
		{
			UpdateScriptPathsInJson(child, scriptMap);
		}
	}
}

void Editor::RemoveStringClone(Node* pNode) {
	std::string thisnodeName = pNode->GetName();
	if (thisnodeName.size() >= 4 && thisnodeName.substr(thisnodeName.size() - 4) == "Copy") {
		pNode->SetName(thisnodeName.substr(0, thisnodeName.size() - 4));
	}
	for (uint32 i = 0; i < pNode->GetChildCount(); i++)
	{
		RemoveStringClone(&pNode->GetChild(i));
	}
}

void Editor::SaveScene(std::string const& path)
{
	try
	{
		std::string tmp = path;

		if (tmp.size() >= 5 && tmp.ends_with(".json"))
			tmp = tmp.substr(0, tmp.size() - 5);

		bool isScene = (tmp.size() >= 3 && tmp.ends_with(".sc"));

		if (isScene)
		{
			if (m_sceneRoot)
			{
				EditorSerializer::SaveScene(tmp, m_sceneRoot);
				m_scenePathBuffer = tmp;
				DEBUG("[Editor] Scene saved: " << m_scenePathBuffer << ".json" << std::endl);
			}
			else
			{
				std::cerr << "[Editor] Cannot save scene: no scene loaded" << std::endl;
			}
		}
		else if (!isScene && m_editorImgui.GetSelectedNode() != nullptr)
		{
			Node* selected = m_editorImgui.GetSelectedNode();
			if (!selected)
			{
				std::cerr << "[Editor] Cannot save node: no node selected" << std::endl;
				return;
			}

			uptr<Node> clone = selected->Clone();

			EngineServer::FlushCommands();
			RemoveStringClone(clone.get());
			/*TODO Enlever le Copy derriere en recursive pour les enfants*/
			std::string test = clone.get()->GetName();
			EditorSerializer::SaveNode(tmp, clone);
			m_nodePathBuffer = tmp;
			DEBUG("[Editor] Node saved: " << m_nodePathBuffer << ".json" << std::endl);
		}
		else
		{
			std::cerr << "[Editor] Cannot save: unknown type or no node selected" << std::endl;
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Editor] Failed to save: " << e.what() << std::endl;
	}
}

void Editor::SaveSceneNoSpe()
{

	if (!m_scenePathBuffer.empty())
		SaveScene(m_scenePathBuffer);
	else
		m_editorImgui.ShowSaveAs(false);
	
}