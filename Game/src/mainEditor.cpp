////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Node.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeViewport.h"
#include "Nodes/NodeWindow.h"
#include "Scripting/Lua/LuaBindings.h"
#include "Servers/EngineServer.h"
#include "Servers/GraphicServer.h"
#include "Serialization/SerializeObject.hpp"

#include <EditorSerializer.h>
#include <GameLoop.h>
#include <Registries/AutomaticRegister.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <filesystem>

namespace
{
	uptr<Node> BuildRuntimeScene(std::string const& scenePath)
	{
		LoadReturn loaded = EditorSerializer::LoadFromJson(scenePath);
		uptr<Node> loadedScene = std::move(loaded.uptrNode);

		uptr<Node> runtimeScene = Node::CreateNode<Node>("RuntimeScene");
		uptr<NodeViewport> viewport = Node::CreateNode<NodeViewport>("Viewport");

		viewport->AddChild(std::move(loadedScene));
		runtimeScene->AddChild(std::move(viewport));

		return runtimeScene;
	}
}

int main(int argc, char* argv[])
{
	std::cout << "[Game] Starting Foundry..." << std::endl;

	if (argc <= 1)
	{
		std::cerr << "[Game] ERROR: Missing scene path argument." << std::endl;
		return 1;
	}

	std::string const scenePath = argv[1];
	std::cout << "[Game] Loading scene from: " << scenePath << std::endl;

	if (!std::filesystem::exists(scenePath))
	{
		std::cerr << "[Game] ERROR: Scene file not found: " << scenePath << std::endl;
		return 1;
	}

	try
	{
		Node::SetStatusEditor(false);

		uptr<Node> windowRoot = Node::CreateNode<NodeWindow>("Window");
		GraphicServer::Initialize();
		SceneTree defaultSceneTree(windowRoot);

		GameLoop loop;
		loop.LoadScene = [scenePath]()
		{
			return BuildRuntimeScene(scenePath);
		};

		loop.StartGame(defaultSceneTree);
		EngineServer::FlushCommands();
		GraphicServer::FlushCommands();
		std::cout << "[Game] Scene executed successfully" << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "[Game] ERROR: Failed to load/start scene: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
