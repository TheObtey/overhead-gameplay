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
		std::cout << "[Game] Loading scene from: " << scenePath << std::endl;

		if (!std::filesystem::exists(scenePath))
		{
			std::cerr << "[Game] ERROR: Scene file not found: " << scenePath << std::endl;
			std::cerr << "[Game] Press Enter to exit..." << std::endl;
			std::cin.get();
			return 1;
		}

		try
		{
			uptr<Node> sceneRoot = EditorSerializer::LoadFromJson(scenePath);

			std::cout << "[Game] Scene loaded successfully: " << sceneRoot->GetName() << std::endl;
			std::cout << "[Game] Number of children: " << sceneRoot->GetChildCount() << std::endl;
			EngineServer::FlushCommands();

			std::cout << "[Game] Scene executed successfully" << std::endl;
			std::cout << "[Game] Press Enter to exit..." << std::endl;
			std::cin.get();
		}

		catch (std::exception const& e)
		{
			std::cerr << "[Game] ERROR: Failed to load scene: " << e.what() << std::endl;
			std::cerr << "[Game] Press Enter to exit..." << std::endl;
			std::cin.get();
			return 1;
		}
	}
	return 0;
}
