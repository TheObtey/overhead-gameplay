#include "GameLoop.h"

#include "Clock.hpp"
#include "Logger.hpp"
#include "Multithreading/TaskGraph.h"
#include "Nodes/NodeWindow.h"
#include "Servers/EngineServer.h"
#include "Servers/GraphicServer.h"
#include "Servers/AudioServer.h"
#include "Servers/PhysicsServer.h"
#include "ActionMap.h"

ActionMap* GameLoop::CurrentActionMap = nullptr;

void GameLoop::StartGame(SceneTree& defaultTree)
{
    try { dynamic_cast<NodeWindow&>(defaultTree.GetRoot()); }
    catch (std::bad_cast ex)
    {
        Logger::LogWithLevel(LogLevel::ERROR, "No window in default SceneTree");
        std::exit(-1);
    }

    InitServers();
    UpdateServers();

    m_pDefaultTree = &defaultTree;
    m_pDefaultTree->m_pCurrentScene = &LoadScene(defaultTree.GetRoot());
    UpdateServers();

    m_pDefaultTree->OnGameStarted();
    m_pDefaultTree->OnSceneChanged();
    LoopGame();
}

void GameLoop::LoopGame()
{
    Clock<> clock;
    //while ();
    auto const&  window = static_cast<NodeWindow const&>(m_pDefaultTree->GetRoot());
    while (window.IsOpen())
    {
        TaskGraph graph;
        double const dt = clock.Reset();
        Node& root = m_pDefaultTree->GetRoot();

        if (CurrentActionMap != nullptr)
            ActionMap::PollInputs(CurrentActionMap);

        m_accumulator += dt;
        
        while (m_accumulator > PHYSICS_DT)
        {
            m_accumulator -= PHYSICS_DT;
            root.PhysicsUpdate(PHYSICS_DT);
            PhysicsServer::UpdatePhysicsWorld(PHYSICS_DT); // !! si update fait ici, il semble beaucoup trop rapide !!
        }

        //PhysicsServer::UpdatePhysicsWorld(dt);
        root.Update(dt);
        ScriptingEngine::Update(dt);
        UpdateServers();
        BuildTasksGraph(graph);

        //graph.Execute();
    }

    EndGame();
}

void GameLoop::EndGame()
{
    m_pDefaultTree->OnGameEnded();
}

void GameLoop::InitServers()
{
    EngineServer::Initialize();
    GraphicServer::Initialize();
    AudioServer::Initialize();
    PhysicsServer::Initialize();
}

void GameLoop::UpdateServers()
{
    EngineServer::FlushCommands();
    GraphicServer::FlushCommands();
    PhysicsServer::FlushCommands();
}

void GameLoop::BuildTasksGraph(TaskGraph& graph)
{
    EngineServer::BuildTasks(graph);
    GraphicServer::BuildTasks(graph);
    PhysicsServer::BuildTasks(graph);
}