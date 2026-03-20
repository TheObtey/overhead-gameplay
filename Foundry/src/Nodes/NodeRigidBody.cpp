#include "Nodes/NodeRigidBody.h"

#include "Servers/PhysicsServer.h"
#include <Debug.h>

NodeRigidBody::~NodeRigidBody()
{
	//PhysicsServer::GetPhysicsWorld().destroyRigidBody(m_pRigidBody);
}

NodeRigidBody::NodeRigidBody(std::string const& name) : Node(name)
{
	//OnSceneEnter.Subscribe([this](Node& self)
	//	{
	//		if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
	//			PhysicsServer::AttachToRigidBody(&rigidBody->GetRigidBody(), this);
	//	});
	//OnParentChange.Subscribe([this](Node& self)
	//	{
	//			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
	//				AttachToRigidBody(&rigidBody->GetRigidBody());
	//	});

	//OnSceneLeave.Subscribe([this](Node& self)
	//	{
	//		//PhysicsServer::DestroyRigidBody(*this);
	//		//PhysicsServer::GetPhysicsWorld().destroyRigidBody(m_pRigidBody);
	//		m_pRigidBody = nullptr;
	//		m_pNode3D = nullptr;
	//	});
}

NodeRigidBody::NodeRigidBody(std::string const& name, Node3D* owner) : Node(name)
{
	SetNode3DParent(owner);
}

void NodeRigidBody::CreateRigidBody()
{
	PhysicsServer::CreateRigidBody(*this);
}
void NodeRigidBody::DestroyRigidBody(NodeRigidBody& rigidBody)
{
	PhysicsServer::DestroyRigidBody(*this);
}

void NodeRigidBody::SetNode3DParent(Node3D* owner)
{
	m_pNode3D = owner;
	m_pNode3D->Update(0.016);
	//m_pRigidBody = PhysicsServer::CreateRigidBody(*owner, this);

	reactphysics3d::Transform reactTr;
	auto pos = owner->GetWorldPosition();
	auto rot = owner->GetWorldRotationQuaternion();
	reactTr.setPosition({ pos.x, pos.y, pos.z });
	reactTr.setOrientation({ rot.x, rot.y, rot.z, rot.w });
	//m_pRigidBody = PhysicsServer::CreateRigidBody(reactTr, this);
	PhysicsServer::CreateRigidBody(*this);

	//m_pRigidBody->setType(rp3d::BodyType::STATIC);
	//m_pRigidBody->enableGravity(true);
	//m_pRigidBody->setLinearDamping(0.5f);
	//m_pRigidBody->setAngularDamping(0.5f);
	//m_pRigidBody->setMass(1.0f);
	//m_pRigidBody->setIsAllowedToSleep(true);
}
void NodeRigidBody::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
	if (m_pNode3D == nullptr) return;

	auto type = m_pRigidBody->getType();
	if (type == rp3d::BodyType::DYNAMIC)
	{
		auto& pos = m_pRigidBody->getTransform().getPosition();
		auto& rot = m_pRigidBody->getTransform().getOrientation();
		m_pNode3D->SetWorldPosition({ pos.x, pos.y, pos.z });
		m_pNode3D->SetWorldRotationQuaternion({ rot.w, rot.x, rot.y, rot.z });
	}
	else
	{
		auto pos = m_pNode3D->GetWorldPosition();
		auto& rot = m_pNode3D->GetWorldRotationQuaternion();
		rp3d::Transform t;
		t.setPosition({ pos.x,pos.y,pos.z });
		t.setOrientation({ rot.w, rot.x, rot.y, rot.z });
		m_pRigidBody->setTransform(t);
		return;
	}
	//m_pOwner->Update(delta);
}

NodeRigidBody::operator reactphysics3d::Transform()
{
	rp3d::Transform reactTr;
	auto pos = m_pNode3D->GetWorldPosition();
	auto rot = m_pNode3D->GetWorldRotationQuaternion();
	reactTr.setPosition({ pos.x, pos.y, pos.z});
	reactTr.setOrientation({ rot.x, rot.y, rot.z, rot.w });

	return reactTr;
}

NodeRigidBody::operator reactphysics3d::Transform*()
{
	rp3d::Transform* reactTr = new rp3d::Transform();
	auto pos = m_pNode3D->GetWorldPosition();
	auto rot = m_pNode3D->GetWorldRotationQuaternion();
	reactTr->setPosition({ pos.x, pos.y, pos.z});
	reactTr->setOrientation({ rot.x, rot.y, rot.z, rot.w });

	return reactTr;
}


void NodeRigidBody::ApplyLocalForceAtCenterOfMass(const glm::vec3& force)
{
	PhysicsServer::ApplyLocalForceAtCenterOfMass(force, *this);
}
void NodeRigidBody::ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)
{
	PhysicsServer::ApplyLocalForceAtLocalPosition(force, point, *this);
}
void NodeRigidBody::ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)
{
	PhysicsServer::ApplyLocalForceAtWorldPosition(force, point, *this);
}

void NodeRigidBody::ApplyWorldForceAtCenterOfMass(const glm::vec3& force)
{
	PhysicsServer::ApplyWorldForceAtCenterOfMass(force, *this);
}
void NodeRigidBody::ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)
{
	PhysicsServer::ApplyWorldForceAtLocalPosition(force, point, *this);
}
void NodeRigidBody::ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)
{
	PhysicsServer::ApplyWorldForceAtWorldPosition(force, point, *this);
}

void NodeRigidBody::ApplyLocalTorque(const glm::vec3& torque)
{
	PhysicsServer::ApplyLocalTorque(torque, *this);
}
void NodeRigidBody::ApplyWorldTorque(const glm::vec3& torque)
{
	PhysicsServer::ApplyWorldTorque(torque, *this);
}

glm::vec3 const NodeRigidBody::GetLinearVelocity() const
{
	if (m_pNode3D == nullptr) return { 0,0,0 };
	return rp3dToGlm(m_pRigidBody->getLinearVelocity());
}
glm::vec3 const NodeRigidBody::GetAngularVelocity() const
{
	if (m_pNode3D == nullptr) return { 0,0,0 };
	return rp3dToGlm(m_pRigidBody->getAngularVelocity());
}
/// Return the linear decelerating factor
float const NodeRigidBody::GetLinearDamping() const
{
	if (m_pNode3D == nullptr) return 0.0f;
	return m_pRigidBody->getLinearDamping();
}
/// Return the angular velocity damping factor
float const NodeRigidBody::GetAngularDamping() const
{
	if (m_pNode3D == nullptr) return 0.0f;
	return m_pRigidBody->getAngularDamping();
}

glm::vec3 const NodeRigidBody::GetTotalForce() const
{
	if (m_pNode3D == nullptr) return { 0,0,0 };
	return rp3dToGlm(m_pRigidBody->getForce());
}


void NodeRigidBody::SetLinearVelocity(const glm::vec3& velocity)
{
	PhysicsServer::SetLinearVelocity(velocity, *this);
}
void NodeRigidBody::SetAngularVelocity(const glm::vec3& velocity)
{
	PhysicsServer::SetAngularVelocity(velocity, *this);
}
/// Set the linear decelerating factor
void  NodeRigidBody::SetLinearDamping(float linearDamping)
{
	PhysicsServer::SetLinearDamping(linearDamping, *this);
}
/// Set the angular decelerating factor
void  NodeRigidBody::SetAngularDamping(float angularDamping)
{
	PhysicsServer::SetAngularDamping(angularDamping, *this);
}
void NodeRigidBody::ResetForces()
{
	PhysicsServer::ResetForces(*this);
}
void NodeRigidBody::ResetTorque()
{
	PhysicsServer::ResetTorque(*this);
}

float NodeRigidBody::GetMass() const
{
	if (m_pNode3D == nullptr) return 0.0f;
	return m_pRigidBody->getMass();
}
void NodeRigidBody::SetMass(float mass)
{
	PhysicsServer::SetMass(mass, *this);
}

RigidBodyType NodeRigidBody::GetBodyType() const
{
	if (m_pNode3D == nullptr) return RigidBodyType::NONE;
	auto type = m_pRigidBody->getType();

	if (type == rp3d::BodyType::DYNAMIC)
		return RigidBodyType::DYNAMIC;
	else if (type == rp3d::BodyType::KINEMATIC)
		return RigidBodyType::KINEMATIC;
	else
		return RigidBodyType::STATIC;
}
void NodeRigidBody::SetBodyType(RigidBodyType type)
{
	PhysicsServer::SetBodyType(type, *this);
}

void NodeRigidBody::LockLinearAxis(bool x, bool y, bool z)
{
	PhysicsServer::LockLinearAxis(new bool[3] { x, y, z }, *this);
}
void NodeRigidBody::LockAngularAxis(bool x, bool y, bool z)
{
	PhysicsServer::LockAngularAxis(new bool[3] { x, y, z }, *this);
}

bool NodeRigidBody::IsSleeping() const
{
	if (m_pNode3D == nullptr) return false;
	return m_pRigidBody->isSleeping();
}
bool NodeRigidBody::IsAllowedToSleep() const
{
	if (m_pNode3D == nullptr) return false;
	return m_pRigidBody->isAllowedToSleep();
}
void NodeRigidBody::SetSleepingEnabled(bool enabled)
{
	PhysicsServer::SetSleepingEnabled(enabled, *this);
}
void NodeRigidBody::SetSleepingState(bool isSleeping)
{
	PhysicsServer::SetSleepingState(isSleeping, *this);
}

bool NodeRigidBody::IsGravityEnabled()
{
	if (m_pNode3D)
		return m_pRigidBody->isGravityEnabled();
}
void NodeRigidBody::SetIsGravityEnabled(bool enabled)
{
	PhysicsServer::SetIsGravityEnabled(enabled, *this);
}



