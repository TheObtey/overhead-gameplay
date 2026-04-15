#ifndef FOUNDRY_REGISTERPROXIES__H_
#define FOUNDRY_REGISTERPROXIES__H_

#include "Scripting/Proxies/EventManagerProxy.h"
#include "Scripting/Proxies/ActionMapProxy.h"
#include "Scripting/Proxies/PhysicsProxy.hpp"
#include "Scripting/Proxies/MathsProxy.h"
#include "Node.h"
#include "Nodes/Node2D.h"
#include "Nodes/Node3D.h"
#include "Nodes/NodeCamera.h"
#include "Nodes/NodeMesh.h"
#include "Nodes/NodeCollider.h"
#include "Nodes/NodeRigidBody.h"
#include "Scripting/Proxies/SceneTreeProxy.h"

#include "Registries/AutomaticRegisterProxy.hpp"


static void RegisterProxies()
{
	AutomaticRegisterProxy::Register<ActionMapProxyBinding>();
	AutomaticRegisterProxy::Register<EventManagerProxyBinding>();
	AutomaticRegisterProxy::Register<MathsProxyBinding>();
	AutomaticRegisterProxy::Register<PhysicsProxyBinding>();
	AutomaticRegisterProxy::Register<SceneTreeProxyBinding>();
	AutomaticRegisterProxy::Register<Node::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<Node2D::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<Node3D::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeCamera::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeWindow::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeMesh::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeCollider::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeBoxCollider::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeSphereCollider::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeCapsuleCollider::Proxy::ProxyBinding>();
	AutomaticRegisterProxy::Register<NodeRigidBody::Proxy::ProxyBinding>();
};


#endif //FOUNDRY_PROXYREGISTER__H_