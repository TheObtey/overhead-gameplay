#ifndef FOUNDRY_REGISTERPROXIES__H_
#define FOUNDRY_REGISTERPROXIES__H_

#include "Scripting/Proxies/EventManagerProxy.h"
#include "Scripting/Proxies/ActionMapProxy.h"
#include "Scripting/Proxies/PhysicsProxy.hpp"
#include "Scripting/Proxies/MathsProxy.h"
#include "Scripting/Proxies/SceneTreeProxy.h"
#include "Node.h"
#include "Nodes/Node2D.h"
#include "Nodes/Node3D.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeWindow.h"
#include "Nodes/NodeCollider.h"
#include "Nodes/NodeRigidBody.h"

#include "Registries/AutomaticRegisterProxy.hpp"


static void RegisterProxies(ScriptEngine& engine)
{
	AutomaticRegisterProxy::Register<ActionMapProxyBinding>(engine);
	AutomaticRegisterProxy::Register<EventManagerProxyBinding>(engine);
	AutomaticRegisterProxy::Register<MathsProxyBinding>(engine);
	AutomaticRegisterProxy::Register<PhysicsProxyBinding>(engine);
	AutomaticRegisterProxy::Register<SceneTreeProxyBinding>(engine);
	AutomaticRegisterProxy::Register<Node::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<Node2D::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<Node3D::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeCamera::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeWindow::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeMesh::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeCollider::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeBoxCollider::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeSphereCollider::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeCapsuleCollider::Proxy::ProxyBinding>(engine);
	AutomaticRegisterProxy::Register<NodeRigidBody::Proxy::ProxyBinding>(engine);
};


#endif //FOUNDRY_PROXYREGISTER__H_