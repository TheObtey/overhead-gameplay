#include "Nodes/NodeCollider.h"

#include "Servers/PhysicsServer.h"

NodeCollider::NodeCollider(std::string const& name) : Node(name)
{
	OnSceneEnter.Subscribe([this](Node& self)
		{
			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
				AttachToRigidBody(rigidBody->GetRigidBody());
		});
	OnParentChange.Subscribe([this](Node& self)
		{
			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
				AttachToRigidBody(rigidBody->GetRigidBody());
		});

	//OnSceneLeave.Subscribe([this](Node& self)
	//	{
	//		Detach();
	//	});

}
void NodeCollider::init()
{
	//if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
	//	AttachToRigidBody(rigidBody->GetRigidBody());
}

NodeCollider::~NodeCollider()
{
	//Detach();
	//DestroyShape();
}

void NodeCollider::DestroyShape()
{
	//if (!m_pShape) return;

	//auto& pc = PhysicsServer::GetPhysicsCommon(); // A REFAIRE POUR SUIVRE LOGIQUE SERVEUR

	//if (auto* s = dynamic_cast<rp3d::BoxShape*>(m_pShape))
	//	pc.destroyBoxShape(s);
	//else if (auto* s = dynamic_cast<rp3d::SphereShape*>(m_pShape))
	//	pc.destroySphereShape(s);
	//else if (auto* s = dynamic_cast<rp3d::CapsuleShape*>(m_pShape))
	//	pc.destroyCapsuleShape(s);

	//m_pShape = nullptr;
	PhysicsServer::DestroyShape(*this);
}

void NodeCollider::SetBoxShape(const glm::vec3& halfExtents)
{
	PhysicsServer::SetBoxShape(halfExtents, *this);
}

void NodeCollider::SetSphereShape(float radius)
{
	PhysicsServer::SetSphereShape(radius, *this);
}

void NodeCollider::SetCapsuleShape(float radius, float height)
{
	PhysicsServer::SetCapsuleShape(radius, height, *this);
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
	if (m_pCollider)
		PhysicsServer::SetLocalPosition(pos, *this);
}

void NodeCollider::SetLocalRotation(const glm::quat& rot)
{
	if (m_pCollider)
		PhysicsServer::SetLocalRotation(rot, *this);
}


void NodeCollider::AttachToRigidBody(rp3d::RigidBody* rigidBody)
{
	PhysicsServer::AttachToRigidBody(rigidBody, *this);


	//Command<PhysicsServer> cmd;
	//cmd.Type = CommandTyp::ATTACH_TO_RIGID_BODY;
	////cmd.To = &c;
	//cmd.rigidBody = rigidBody; 
	//PhysicsServer::PushCommand(Command<PhysicsServer>{
	//	.Type = CmdType::APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS,
	//		.rigidBody = &rb,
	//		.force = force,
	//		.To = target
	//}, this);

	//PhysicsServer::Instance().m_commands.push(cmd);
	//PhysicsServer::Instance().m_commands.push({}, *this);
}

void NodeCollider::Detach()
{
	PhysicsServer::Detach(*this);
	//if (m_pCollider && m_pRigidBody)
	//	m_pRigidBody->removeCollider(m_pCollider);
	//m_pCollider = nullptr;
}

void  NodeCollider::SetBounciness(float v)
{
	if (m_pCollider) PhysicsServer::SetBounciness(v, *this);
}
float NodeCollider::GetBounciness() const
{
	return m_pCollider ? m_pCollider->getMaterial().getBounciness() : 0.0f;
}
void  NodeCollider::SetFrictionCoefficient(float v)
{
	if (m_pCollider) PhysicsServer::SetFrictionCoefficient(v, *this);
}
float NodeCollider::GetFrictionCoefficient() const
{
	return m_pCollider ? m_pCollider->getMaterial().getFrictionCoefficient() : 0.0f;
}
void  NodeCollider::SetMassDensity(float v)
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
	if (m_pCollider) PhysicsServer::SetCollisionCategoryBits(v, *this);
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