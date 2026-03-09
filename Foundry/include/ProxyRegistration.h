#ifndef FOUNDRY_PROXY_REG__H_
#define FOUNDRY_PROXY_REG__H_

#include "Node.h"
#include "Scripting/Binder.h"

#define BIND_PROXY(B, X) X::Proxy::ProxyBinding __##X##__; __##X##__.Bind(B)

static void REGISTER_PROXY(Binder& binder)
{
	

	//BIND_PROXY(binder, Node);
}

#endif // !FOUNDRY_PROXY_REG__H_
