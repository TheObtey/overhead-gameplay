#include "GameLoop.h"

#include "Clock.hpp"
#include "Node.h"
#include "Multithreading/TaskGraph.h"
#include "Servers/EngineServer.h"

void GameLoop::StartGame(SceneTree& defaultTree)
{
    m_pDefaultTree = &defaultTree;
    m_pDefaultTree->OnGameStarted();
    m_pDefaultTree->OnSceneChanged();
    LoopGame();
}

void GameLoop::LoopGame()
{
    Clock<> clock;
    //while (m_defaultTree->GetRoot().isOpen());
    while (true)
    {
        TaskGraph graph;
        double const dt = clock.Reset();
        Node& root = m_pDefaultTree->GetRoot();

        m_accumulator += dt;
        do
        {
            m_accumulator -= dt;
            root.PhysicsUpdate(PHYSICS_DT);
        }
        while (m_accumulator > dt);

        root.Update(dt);
        UpdateServers();
        BuildTasksGraph(graph);

        graph.Execute();
    }

    EndGame();
}

void GameLoop::EndGame()
{
    m_pDefaultTree->OnGameEnded();
}

void GameLoop::UpdateServers()
{
    EngineServer::FlushCommands();
}

void GameLoop::BuildTasksGraph(TaskGraph& graph)
{
    EngineServer::BuildTasks(graph);
}


