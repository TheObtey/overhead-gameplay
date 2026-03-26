////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include "Define.h"
#include "Node.h"
#include "Nodes/Node3D.h"
#include "Scripting/Lua/LuaBindings.h"
#include "Servers/EngineServer.h"
#include "Serialization/SerializeObject.hpp"

#include <EditorSerializer.h>
#include <GameLoop.h>
#include <Registries/AutomaticRegister.hpp>
#include <memory>
#include <iostream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[])
{
	std::cout << "[Game] Starting Foundry..." << std::endl;

	if (argc > 1)
	{
		std::string scenePath = argv[1];
		std::cout << "[Game] Loading scene from: " << scenePath << "\n";

		if (!std::filesystem::exists(scenePath))
		{
			std::cerr << "[Game] ERROR: Scene file not found: " << scenePath << "\n";
			std::cerr << "[Game] Press Enter to exit..." << std::endl;
			std::cin.get();
			return 1;
		}

		try
		{
			Node::SetStatusEditor(false);

			uptr<Node> sceneRoot = std::move(EditorSerializer::LoadFromJson(scenePath).uptrNode);

			std::cout << "[Game] Scene loaded successfully: " << sceneRoot->GetName() << "\n";
			std::cout << "[Game] Number of children: " << sceneRoot->GetChildCount() << "\n";

			SceneTree defaultSceneTree(sceneRoot);
			GameLoop loop;
			loop.StartGame(defaultSceneTree);

			std::cout << "[Game] Scene executed successfully" << "\n";
			std::cout << "[Game] Press Enter to exit..." << std::endl;
			std::cin.get();
		}

		catch (std::exception const& e)
		{
			std::cerr << "[Game] ERROR: Failed to load scene: " << e.what() << "\n";
			std::cerr << "[Game] Press Enter to exit..." << std::endl;
			std::cin.get();
			return 1;
		}
	}
	return 0;
}
