#include "Nodes/NodeRigidBody.h"

#include "Servers/PhysicsServer.h"
#include <Debug.h>

NodeRigidBody::~NodeRigidBody()
{
	PhysicsServer::GetPhysicsWorld().destroyRigidBody(m_pRigidBody);
}

NodeRigidBody::NodeRigidBody(std::string const& name) : Node(name)
{
	OnParentChange.Subscribe([this](Node& self)
		{
		//	if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
		//		AttachToRigidBody(&rigidBody->GetRigidBody());
		});

	OnSceneLeave.Subscribe([this](Node& self)
		{
			PhysicsServer::GetPhysicsWorld().destroyRigidBody(m_pRigidBody);
			 m_pRigidBody = nullptr;
			 m_pNode3D = nullptr;
		});
}

NodeRigidBody::NodeRigidBody(std::string const& name, Node3D* owner) : Node(name)
{
	SetNode3DParent(owner);

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
	m_pRigidBody = PhysicsServer::CreateRigidBody(reactTr, this);

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
	if (type == rp3d::BodyType::KINEMATIC || type == rp3d::BodyType::STATIC)
	{
		auto pos = m_pNode3D->GetWorldPosition();
		auto& rot = m_pNode3D->GetWorldRotationQuaternion();
		rp3d::Transform t;
		t.setPosition({ pos.x,pos.y,pos.z });
		t.setOrientation({ rot.w, rot.x, rot.y, rot.z });
		m_pRigidBody->setTransform(t);
		return;
	}
	else
	{
		auto& pos = m_pRigidBody->getTransform().getPosition();
		auto& rot = m_pRigidBody->getTransform().getOrientation();
		m_pNode3D->SetWorldPosition({ pos.x, pos.y, pos.z });
		m_pNode3D->SetWorldRotationQuaternion({ rot.w, rot.x, rot.y, rot.z });
	}
	//m_pOwner->Update(delta);
}



void NodeRigidBody::ApplyLocalForceAtCenterOfMass(const glm::vec3& force)
{
	if (m_pNode3D)
		m_pRigidBody->applyLocalForceAtCenterOfMass(glmToRp3d(force));
}
void NodeRigidBody::ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)
{
	if (m_pNode3D)
		m_pRigidBody->applyLocalForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void NodeRigidBody::ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)
{
	if (m_pNode3D)
		m_pRigidBody->applyLocalForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void NodeRigidBody::ApplyWorldForceAtCenterOfMass(const glm::vec3& force)
{
	if (m_pNode3D)
		m_pRigidBody->applyWorldForceAtCenterOfMass(glmToRp3d(force));
}
void NodeRigidBody::ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)
{
	if (m_pNode3D)
		m_pRigidBody->applyWorldForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void NodeRigidBody::ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)
{
	if (m_pNode3D)
		m_pRigidBody->applyWorldForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void NodeRigidBody::ApplyLocalTorque(const glm::vec3& torque)
{
	if (m_pNode3D)
		m_pRigidBody->applyLocalTorque(glmToRp3d(torque));
}
void NodeRigidBody::ApplyWorldTorque(const glm::vec3& torque)
{
	if (m_pNode3D)
		m_pRigidBody->applyWorldTorque(glmToRp3d(torque));
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
	if (m_pNode3D)
		m_pRigidBody->setLinearVelocity(glmToRp3d(velocity));
}
void NodeRigidBody::SetAngularVelocity(const glm::vec3& velocity)
{
	if (m_pNode3D)
		m_pRigidBody->setAngularVelocity(glmToRp3d(velocity));
}
/// Set the linear decelerating factor
void  NodeRigidBody::SetLinearDamping(float linearDamping)
{
	if (m_pNode3D)
		m_pRigidBody->setLinearDamping(linearDamping);
}
/// Set the angular decelerating factor
void  NodeRigidBody::SetAngularDamping(float angularDamping)
{
	if (m_pNode3D)
		m_pRigidBody->setAngularDamping(angularDamping);
}
void NodeRigidBody::ResetForces()
{
	if (m_pNode3D)
		m_pRigidBody->resetForce();
}
void NodeRigidBody::ResetTorque()
{

	if (m_pNode3D)
		m_pRigidBody->resetTorque();
}

float NodeRigidBody::GetMass() const
{
	if (m_pNode3D == nullptr) return 0.0f;
	return m_pRigidBody->getMass();
}
void  NodeRigidBody::SetMass(float mass)
{
	if (m_pNode3D)
		m_pRigidBody->setMass(mass);
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
	if (m_pNode3D == nullptr) return;
	switch (type)
	{
	case RigidBodyType::STATIC:
		m_pRigidBody->setType(rp3d::BodyType::STATIC);
		break;
	case RigidBodyType::KINEMATIC:
		m_pRigidBody->setType(rp3d::BodyType::KINEMATIC);
		break;
	case RigidBodyType::DYNAMIC:
		m_pRigidBody->setType(rp3d::BodyType::DYNAMIC);
		break;
	default:
		DEBUG("Invalid RigidBodyType set in SetBodyType func\n");
		break;
	}
}

void NodeRigidBody::LockLinearAxis(bool x, bool y, bool z)
{
	if (m_pNode3D)
		m_pRigidBody->setLinearLockAxisFactor(rp3d::Vector3{ x == true ? 0.0f : 1.0f, y == true ? 0.0f : 1.0f, z == true ? 0.0f : 1.0f });
}
void NodeRigidBody::LockAngularAxis(bool x, bool y, bool z)
{
	if (m_pNode3D)
		m_pRigidBody->setAngularLockAxisFactor(rp3d::Vector3{ x == true ? 0.0f : 1.0f, y == true ? 0.0f : 1.0f, z == true ? 0.0f : 1.0f });
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
	if (m_pNode3D)
		m_pRigidBody->setIsAllowedToSleep(enabled);
}
void NodeRigidBody::SetSleepingState(bool isSleeping)
{
	if (m_pNode3D)
		m_pRigidBody->setIsSleeping(isSleeping);
}

bool NodeRigidBody::IsGravityEnabled()
{
	if (m_pNode3D)
		return m_pRigidBody->isGravityEnabled();
}
void NodeRigidBody::SetIsGravityEnabled(bool enabled)
{
	if (m_pNode3D)
		m_pRigidBody->enableGravity(enabled);
}



