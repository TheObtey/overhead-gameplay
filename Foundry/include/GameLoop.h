#ifndef FOUNDRY_GAMELOOP__H_
#define FOUNDRY_GAMELOOP__H_

#include "SceneTree.h"

//TODO Move in a config file
#define PHYSICS_DT 1.0/60.0

class TaskGraph;
//Base class for the engine main loop
class GameLoop
{
public:
    virtual void StartGame(SceneTree& defaultTree);
    virtual void LoopGame();
    virtual void EndGame();

    virtual ~GameLoop() = default;

    std::function<Node&(Node& root)> LoadScene;

protected:
    virtual void InitServers();
    virtual void UpdateServers();
    virtual void BuildTasksGraph(TaskGraph& graph);

protected:
    SceneTree* m_pDefaultTree = nullptr;
    double m_accumulator = 0.0f;
};

#endif