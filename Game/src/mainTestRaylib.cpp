////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

//#include <EditorSerializer.h>
#include <Servers/EngineServer.h>

//#include "Scripting/Proxies/NodeProxy.inl"

#include "Node.h"
#include "Scripting/Proxies/MathsProxy.hpp"

int main() {
    //std::cout << "MAIN EDITOR" << std::endl;
    //uptr<Node> node = Node::CreateNode<Node>("grand parent");
    //uptr<Node> node2 = Node::CreateNode<Node>("Enfant1");
    //uptr<Node> node4 = Node::CreateNode<Node>("Enfant2");
    //uptr<Node> node3 = Node::CreateNode<Node>("enfant1 d'enfant1");
    //node2.get()->AddChild(node3);
    //node.get()->AddChild(node2);
    //node.get()->AddChild(node4);


    std::string t;
    uptr<Node> rootNode = Node::CreateNode<Node>("Root");
    uptr<Node> rootNode1 = Node::CreateNode<Node>("Root1");
    uptr<Node> rootNode2 = Node::CreateNode<Node>("Root2");
    uptr<Node> rootNode3 = Node::CreateNode<Node>("Root3");

    rootNode1.get()->AddChild(rootNode3);
    rootNode.get()->AddChild(rootNode1);
    rootNode.get()->AddChild(rootNode2);
    EngineServer::FlushCommands();

    //EditorSerializer::Save("testJson.json", rootNode);

    //uptr<Node> outRoot = EditorSerializer::LoadFromJson("testJson.json");
    EngineServer::FlushCommands();

}