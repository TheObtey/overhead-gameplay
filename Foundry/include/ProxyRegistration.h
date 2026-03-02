#ifndef FOUNDRY_PROXY_REG__H_
#define FOUNDRY_PROXY_REG__H_

#include "Node.h"
#include "Scripting/Binder.h"

static inline void REGISTER_PROXY()
{

	//NodeProxy
	Binder::BindFunction("CreateNode", &Node::Proxy::CreateNodeProxy);

	Binder::BindClass<Node::Proxy>("Node", 
			"GetName", &Node::Proxy::GetName);
}

#endif // !FOUNDRY_PROXY_REG__H_
