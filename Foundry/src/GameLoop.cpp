#include "GameLoop.h"

#include "Clock.hpp"
#include "Logger.hpp"
#include "Multithreading/TaskGraph.h"
#include "Nodes/NodeWindow.h"
#include "Servers/EngineServer.h"
#include "Servers/GraphicServer.h"

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

        m_accumulator += dt;
        do
        {
            m_accumulator -= PHYSICS_DT;
            root.PhysicsUpdate(PHYSICS_DT);
        }
        while (m_accumulator > PHYSICS_DT);

        root.Update(dt);
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
}

void GameLoop::UpdateServers()
{
    EngineServer::FlushCommands();
    GraphicServer::FlushCommands();
}

void GameLoop::BuildTasksGraph(TaskGraph& graph)
{
    EngineServer::BuildTasks(graph);
    GraphicServer::BuildTasks(graph);
}


