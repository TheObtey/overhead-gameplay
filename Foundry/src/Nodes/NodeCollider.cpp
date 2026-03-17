#include "Nodes/NodeCollider.h"

#include "Servers/PhysicsServer.h"

NodeCollider::NodeCollider(std::string const& name) : Node(name)
{
	OnSceneEnter.Subscribe([this](Node& self)
		{
			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
				AttachToRigidBody(&rigidBody->GetRigidBody());
		});
	OnParentChange.Subscribe([this](Node& self)
		{
			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
				AttachToRigidBody(&rigidBody->GetRigidBody());
		});

	OnSceneLeave.Subscribe([this](Node& self)
		{
			Detach();
		});

}
void NodeCollider::init()
{
	if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
		AttachToRigidBody(&rigidBody->GetRigidBody());
}

NodeCollider::~NodeCollider()
{
	Detach();
	DestroyShape();
}

void NodeCollider::DestroyShape()
{
	if (!m_pShape) return;

	auto& pc = PhysicsServer::GetPhysicsCommon(); // A REFAIRE POUR SUIVRE LOGIQUE SERVEUR

	if (auto* s = dynamic_cast<rp3d::BoxShape*>(m_pShape))
		pc.destroyBoxShape(s);
	else if (auto* s = dynamic_cast<rp3d::SphereShape*>(m_pShape))
		pc.destroySphereShape(s);
	else if (auto* s = dynamic_cast<rp3d::CapsuleShape*>(m_pShape))
		pc.destroyCapsuleShape(s);

	m_pShape = nullptr;
}

void NodeCollider::SetBoxShape(const glm::vec3& halfExtents)
{
	Detach();
	DestroyShape();
	m_pShape = PhysicsServer::GetPhysicsCommon().createBoxShape({ halfExtents.x, halfExtents.y, halfExtents.z });

	if (m_pRigidBody)
		AttachToRigidBody(m_pRigidBody);
}

void NodeCollider::SetSphereShape(float radius)
{
	Detach();
	DestroyShape();
	m_pShape = PhysicsServer::GetPhysicsCommon().createSphereShape(radius);
	if (m_pRigidBody) AttachToRigidBody(m_pRigidBody);
}

void NodeCollider::SetCapsuleShape(float radius, float height)
{
	Detach();
	DestroyShape();
	m_pShape = PhysicsServer::GetPhysicsCommon().createCapsuleShape(radius, height);
	if (m_pRigidBody) AttachToRigidBody(m_pRigidBody);
}

rp3d::Transform NodeCollider::GetLocalRp3dTransform() const
{
	return rp3d::Transform(
		{ m_localPosition.x, m_localPosition.y, m_localPosition.z },
		{ m_localRotation.w, m_localRotation.x, m_localRotation.y, m_localRotation.z }
	);
}

void NodeCollider::SetLocalPosition(const glm::vec3& pos)
{
	m_localPosition = pos;
	if (m_pCollider)
		m_pCollider->setLocalToBodyTransform(GetLocalRp3dTransform());
}

void NodeCollider::SetLocalRotation(const glm::quat& rot)
{
	m_localRotation = rot;
	if (m_pCollider)
		m_pCollider->setLocalToBodyTransform(GetLocalRp3dTransform());
}


void NodeCollider::AttachToRigidBody(rp3d::RigidBody* rigidBody)
{
	if (!m_pShape) return;
	m_pRigidBody = rigidBody;
	m_pCollider = rigidBody->addCollider(m_pShape, GetLocalRp3dTransform());
}

void NodeCollider::Detach()
{
	//if (m_pCollider && m_pRigidBody)
	//	m_pRigidBody->removeCollider(m_pCollider);
	//m_pCollider = nullptr;
}

void  NodeCollider::SetBounciness(float v)
{
	if (m_pCollider) m_pCollider->getMaterial().setBounciness(v);
}
float NodeCollider::GetBounciness() const
{
	return m_pCollider ? m_pCollider->getMaterial().getBounciness() : 0.0f;
}
void  NodeCollider::SetFrictionCoefficient(float v)
{
	if (m_pCollider) m_pCollider->getMaterial().setFrictionCoefficient(v);
}
float NodeCollider::GetFrictionCoefficient() const
{
	return m_pCollider ? m_pCollider->getMaterial().getFrictionCoefficient() : 0.0f;
}
void  NodeCollider::SetMassDensity(float v)
{
	if (m_pCollider) m_pCollider->getMaterial().setMassDensity(v);
}
float NodeCollider::GetMassDensity() const
{
	return m_pCollider ? m_pCollider->getMaterial().getMassDensity() : 1.0f;
}

void NodeCollider::SetIsTrigger(bool v)
{
	if (m_pCollider) m_pCollider->setIsTrigger(v);
}
bool NodeCollider::IsTrigger() const
{
	return m_pCollider ? m_pCollider->getIsTrigger() : false;
}
void NodeCollider::SetIsSimulationCollider(bool v)
{
	if (m_pCollider) m_pCollider->setIsSimulationCollider(v);
}
bool NodeCollider::IsSimulationCollider() const
{
	return m_pCollider ? m_pCollider->getIsSimulationCollider() : false;
}
void NodeCollider::SetIsWorldQueryCollider(bool v)
{
	if (m_pCollider) m_pCollider->setIsWorldQueryCollider(v);
}
bool NodeCollider::IsWorldQueryCollider() const
{
	return m_pCollider ? m_pCollider->getIsWorldQueryCollider() : false;
}


void NodeCollider::SetCollisionCategoryBits(uint16_t v)
{
	if (m_pCollider) m_pCollider->setCollisionCategoryBits(v);
}
uint16_t NodeCollider::GetCollisionCategoryBits() const
{
	return m_pCollider ? m_pCollider->getCollisionCategoryBits() : 0x0001;
}
void NodeCollider::SetCollideWithMaskBits(uint16_t v) 
{
	if (m_pCollider) m_pCollider->setCollideWithMaskBits(v);
}
uint16_t NodeCollider::GetCollisionBitsMask() const
{
	return m_pCollider ? m_pCollider->getCollideWithMaskBits() : 0xFFFF;
}