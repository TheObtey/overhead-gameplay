
#include "EventManager.h"
#include "ActionMap.h"
#include "Action.h"

#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include "GameLoop.h"
#include "SceneTree.h"
#include <Nodes/NodeWindow.h>

int main(int argc, char** argv)
{
    uptr<Node> root = Node::CreateNode<NodeWindow>("Root");
    uptr<Node> scene = Node::CreateNode<Node>("Scene");    //Load this with the default .st file
    root->AddChild(scene);
    SceneTree defaultSceneTree(root);

    Event<void(IControl&)> Zevent;
    Zevent.Subscribe(
        std::function<void(IControl&)>([](IControl& iControl)
            { std::cout << "YEEEEEHHH" << std::endl; }
        )
    );
    Action* pAction1 = new Action(ControlType::BUTTON, Zevent, EventInput::KEY_Z);

    ActionMap actionMap = ActionMap();
    actionMap.Emplace("MOVE_UP", pAction1);

    GameLoop loop;
    loop.StartGame(defaultSceneTree);


	return 0;
}
