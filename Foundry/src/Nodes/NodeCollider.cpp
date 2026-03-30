#include "Nodes/NodeCollider.h"

#include "Servers/PhysicsServer.h"

NodeCollider::NodeCollider(std::string const& name) : Node3D(name)
{
	//OnSceneEnter.Subscribe([this](Node& self)
	//	{
	//		
	//		
	//	});
	OnParentChange.Subscribe([this](Node& self)
		{
			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
				AttachToRigidBody(rigidBody);
		});

	//OnSceneLeave.Subscribe([this](Node& self)
	//	{
	//		
	//	});

}

NodeCollider::~NodeCollider()
{
	//Detach();
	//DestroyShape();
}
//
//void NodeCollider::DestroyShape()
//{
//	PhysicsServer::DestroyShape(*this);
//}

rp3d::Transform NodeCollider::GetLocalRp3dTransform() const
{
	return rp3d::Transform(
		{ m_localPosition.x, m_localPosition.y, m_localPosition.z },
		{ m_localRotation.w, m_localRotation.x, m_localRotation.y, m_localRotation.z }
	);
}

void NodeCollider::SetLocalPosition(const glm::vec3& pos)
{
	if (m_pCollider)
		PhysicsServer::SetLocalPosition(pos, *this);
}

void NodeCollider::SetLocalRotation(const glm::quat& rot)
{
	if (m_pCollider)
		PhysicsServer::SetLocalRotation(rot, *this);
}


void NodeCollider::AttachToRigidBody(NodeRigidBody* rigidBody)
{
	PhysicsServer::AttachToRigidBody(rigidBody, *this);
}

void NodeCollider::Detach()
{
	PhysicsServer::Detach(*this);
}

void NodeCollider::SetBounciness(float v)
{
	if (v > 1.0f) v = v/100;
	if (v > 1.0f) v = 1.0f;
	if (v < 0.0f) v = 0.0f;

	if (m_pCollider) PhysicsServer::SetBounciness(v, *this);
}
float NodeCollider::GetBounciness() const
{
	return m_pCollider ? m_pCollider->getMaterial().getBounciness() : 0.0f;
}
void NodeCollider::SetFrictionCoefficient(float v)
{
	//if (v > 1.0f) v = v / 100;
	//if (v > 1.0f) v = 1.0f;
	//if (v < 0.0f) v = 0.0f;
	if (m_pCollider) PhysicsServer::SetFrictionCoefficient(v, *this);
}
float NodeCollider::GetFrictionCoefficient() const
{
	return m_pCollider ? m_pCollider->getMaterial().getFrictionCoefficient() : 0.0f;
}
void NodeCollider::SetMassDensity(float v)
{
	if (m_pCollider) PhysicsServer::SetMassDensity(v, *this);
}
float NodeCollider::GetMassDensity() const
{
	return m_pCollider ? m_pCollider->getMaterial().getMassDensity() : 1.0f;
}

void NodeCollider::SetIsTrigger(bool v)
{
	if (m_pCollider) PhysicsServer::SetIsTrigger(v, *this);
}
bool NodeCollider::IsTrigger() const
{
	return m_pCollider ? m_pCollider->getIsTrigger() : false;
}
void NodeCollider::SetIsSimulationCollider(bool v)
{
	if (m_pCollider) PhysicsServer::SetIsSimulationCollider(v, *this);
}
bool NodeCollider::IsSimulationCollider() const
{
	return m_pCollider ? m_pCollider->getIsSimulationCollider() : false;
}
void NodeCollider::SetIsWorldQueryCollider(bool v)
{
	if (m_pCollider)PhysicsServer::SetIsWorldQueryCollider(v, *this);
}
bool NodeCollider::IsWorldQueryCollider() const
{
	return m_pCollider ? m_pCollider->getIsWorldQueryCollider() : false;
}

void NodeCollider::SetCollisionCategoryBits(uint16_t v)
{
	if (m_pCollider) 
		PhysicsServer::SetCollisionCategoryBits(v, *this);
}
uint16_t NodeCollider::GetCollisionCategoryBits() const
{
	return m_pCollider ? m_pCollider->getCollisionCategoryBits() : 0x0001;
}
void NodeCollider::SetCollideWithMaskBits(uint16_t v)
{
	if (m_pCollider) PhysicsServer::SetCollideWithMaskBits(v, *this);
}
uint16_t NodeCollider::GetCollisionBitsMask() const
{
	return m_pCollider ? m_pCollider->getCollideWithMaskBits() : 0xFFFF;
}



void NodeBoxCollider::SetShape(const glm::vec3& halfExtents)
{
	PhysicsServer::SetBoxShape(halfExtents, *this);
}
void NodeBoxCollider::DestroyShape()
{
	PhysicsServer::GetPhysicsCommon().destroyBoxShape(static_cast<rp3d::BoxShape*>(m_pShape));
}

void NodeSphereCollider::SetShape(float radius)
{
	PhysicsServer::SetSphereShape(radius, *this);
}
void NodeSphereCollider::DestroyShape()
{
	PhysicsServer::GetPhysicsCommon().destroySphereShape(static_cast<rp3d::SphereShape*>(m_pShape));
}

void NodeCapsuleCollider::SetShape(float radius, float height)
{
	PhysicsServer::SetCapsuleShape(radius, height, *this);
}
void NodeCapsuleCollider::DestroyShape()
{
	PhysicsServer::GetPhysicsCommon().destroyCapsuleShape(static_cast<rp3d::CapsuleShape*>(m_pShape));
}