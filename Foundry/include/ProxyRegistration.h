#ifndef FOUNDRY_PROXY_REG__H_
#define FOUNDRY_PROXY_REG__H_

#include "Node.h"
#include "Scripting/Binder.h"

void Test() { std::cout << "Hello world" << std::endl; }

static void REGISTER_PROXY()
{
	//NodeProxy
	Binder::BindFunction("CreateNode", &Node::Proxy::CreateNodeProxy);
	//Binder::BindFunction("Test", &Test);

	Binder::BindClass<Node::Proxy>("Node", 
			"GetName", &Node::Proxy::GetName);
}

#endif // !FOUNDRY_PROXY_REG__H_
