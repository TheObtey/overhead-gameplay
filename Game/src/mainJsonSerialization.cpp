////////////////////////////////////////////////////
// CMake Generator by Alexandre Glatz
// https://github.com/AlexandreGlatz/cmake-generator
////////////////////////////////////////////////////

#include <SerializeObject.h>
#include <ISerializable.h>
#include <Servers/EngineServer.h>
#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

int main() {
    //std::cout << "MAIN EDITOR" << std::endl;
    //uptr<Node> node = Node::CreateNode<Node>("grand parent");
    //uptr<Node> node2 = Node::CreateNode<Node>("Enfant1");
    //uptr<Node> node4 = Node::CreateNode<Node>("Enfant2");
    //uptr<Node> node3 = Node::CreateNode<Node>("enfant1 d'enfant1");
    //node2.get()->AddChild(node3);
    //node.get()->AddChild(node2);
    //node.get()->AddChild(node4);


    SerializeObject json;
    uptr<Node> rootNode = Node::CreateNode<Node>("Root");
    SerializeObject::Save("testJson.json", rootNode);
    EngineServer::FlushCommands();

}