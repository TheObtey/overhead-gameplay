#include "Nodes/NodeCollider.h"
#include "Serialization/SerializeObject.hpp"

#include "Servers/PhysicsServer.h"

NodeCollider::NodeCollider(std::string const &name) : Node3D(name)
{
	// OnSceneEnter.Subscribe([this](Node& self)
	//	{
	//
	//
	//	});
	OnParentChange.Subscribe([this](Node &self)
							 {
			if (auto* rigidBody = dynamic_cast<NodeRigidBody*>(m_pOwner))
				AttachToRigidBody(rigidBody); });

	// OnSceneLeave.Subscribe([this](Node& self)
	//	{
	//
	//	});
}

NodeCollider::~NodeCollider()
{
	// Detach();
	// DestroyShape();
}
//
// void NodeCollider::DestroyShape()
//{
//	PhysicsServer::DestroyShape(*this);
//}

rp3d::Transform NodeCollider::GetLocalRp3dTransform() const
{
	return rp3d::Transform(
		{m_localPosition.x, m_localPosition.y, m_localPosition.z},
		{m_localRotation.w, m_localRotation.x, m_localRotation.y, m_localRotation.z});
}

void NodeCollider::SetLocalPosition(const glm::vec3 &pos)
{
	if (m_pCollider)
		PhysicsServer::SetLocalPosition(pos, *this);
}

void NodeCollider::SetLocalRotation(const glm::quat &rot)
{
	if (m_pCollider)
		PhysicsServer::SetLocalRotation(rot, *this);
}

void NodeCollider::AttachToRigidBody(NodeRigidBody *rigidBody)
{
	PhysicsServer::AttachToRigidBody(rigidBody, *this);
}

void NodeCollider::Detach()
{
	PhysicsServer::Detach(*this);
}

void NodeCollider::SetBounciness(float v)
{
	if (v > 1.0f)
		v = v / 100;
	if (v > 1.0f)
		v = 1.0f;
	if (v < 0.0f)
		v = 0.0f;

	if (m_pCollider)
		PhysicsServer::SetBounciness(v, *this);
}
float NodeCollider::GetBounciness() const
{
	return m_pCollider ? m_pCollider->getMaterial().getBounciness() : 0.0f;
}
void NodeCollider::SetFrictionCoefficient(float v)
{
	// if (v > 1.0f) v = v / 100;
	// if (v > 1.0f) v = 1.0f;
	// if (v < 0.0f) v = 0.0f;
	if (m_pCollider)
		PhysicsServer::SetFrictionCoefficient(v, *this);
}
float NodeCollider::GetFrictionCoefficient() const
{
	return m_pCollider ? m_pCollider->getMaterial().getFrictionCoefficient() : 0.0f;
}
void NodeCollider::SetMassDensity(float v)
{
	if (m_pCollider)
		PhysicsServer::SetMassDensity(v, *this);
}
float NodeCollider::GetMassDensity() const
{
	return m_pCollider ? m_pCollider->getMaterial().getMassDensity() : 1.0f;
}

void NodeCollider::SetIsTrigger(bool v)
{
	if (m_pCollider)
		PhysicsServer::SetIsTrigger(v, *this);
}
bool NodeCollider::IsTrigger() const
{
	return m_pCollider ? m_pCollider->getIsTrigger() : false;
}
void NodeCollider::SetIsSimulationCollider(bool v)
{
	if (m_pCollider)
		PhysicsServer::SetIsSimulationCollider(v, *this);
}
bool NodeCollider::IsSimulationCollider() const
{
	return m_pCollider ? m_pCollider->getIsSimulationCollider() : false;
}
void NodeCollider::SetIsWorldQueryCollider(bool v)
{
	if (m_pCollider)
		PhysicsServer::SetIsWorldQueryCollider(v, *this);
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
	if (m_pCollider)
		PhysicsServer::SetCollideWithMaskBits(v, *this);
}
uint16_t NodeCollider::GetCollisionBitsMask() const
{
	return m_pCollider ? m_pCollider->getCollideWithMaskBits() : 0xFFFF;
}

void NodeCollider::Serialize(SerializedObject &datas) const
{
	Node3D::Serialize(datas);
	datas.SetType("NodeCollider");

	glm::vec3 localPosition = m_localPosition;
	glm::vec4 localRotation{m_localRotation.x, m_localRotation.y, m_localRotation.z, m_localRotation.w};

	float bounciness = GetBounciness();
	float friction = GetFrictionCoefficient();
	float massDensity = GetMassDensity();
	bool isTrigger = IsTrigger();
	bool isSimulationCollider = IsSimulationCollider();
	bool isWorldQueryCollider = IsWorldQueryCollider();
	uint16_t collisionCategoryBits = GetCollisionCategoryBits();
	uint16_t collideWithMaskBits = GetCollisionBitsMask();

	datas.AddPublicElement("LocalPosition", &localPosition);
	datas.AddPublicElement("LocalRotation", &localRotation);
	datas.AddPublicElement("Bounciness", &bounciness);
	datas.AddPublicElement("FrictionCoefficient", &friction);
	datas.AddPublicElement("MassDensity", &massDensity);
	datas.AddPublicElement("IsTrigger", &isTrigger);
	datas.AddPublicElement("IsSimulationCollider", &isSimulationCollider);
	datas.AddPublicElement("IsWorldQueryCollider", &isWorldQueryCollider);
	datas.AddPublicElement("CollisionCategoryBits", &collisionCategoryBits);
	datas.AddPublicElement("CollideWithMaskBits", &collideWithMaskBits);
}

void NodeCollider::Deserialize(SerializedObject const &datas)
{
	Node3D::Deserialize(datas);

	glm::vec3 localPosition = m_localPosition;
	datas.GetPublicElement("LocalPosition", &localPosition);

	glm::vec4 localRotationVec{m_localRotation.x, m_localRotation.y, m_localRotation.z, m_localRotation.w};
	datas.GetPublicElement("LocalRotation", &localRotationVec);
	glm::quat localRotation(localRotationVec.w, localRotationVec.x, localRotationVec.y, localRotationVec.z);

	float bounciness = GetBounciness();
	datas.GetPublicElement("Bounciness", &bounciness);

	float friction = GetFrictionCoefficient();
	datas.GetPublicElement("FrictionCoefficient", &friction);

	float massDensity = GetMassDensity();
	datas.GetPublicElement("MassDensity", &massDensity);

	bool isTrigger = IsTrigger();
	datas.GetPublicElement("IsTrigger", &isTrigger);

	bool isSimulationCollider = IsSimulationCollider();
	datas.GetPublicElement("IsSimulationCollider", &isSimulationCollider);

	bool isWorldQueryCollider = IsWorldQueryCollider();
	datas.GetPublicElement("IsWorldQueryCollider", &isWorldQueryCollider);

	uint16_t collisionCategoryBits = GetCollisionCategoryBits();
	datas.GetPublicElement("CollisionCategoryBits", &collisionCategoryBits);

	uint16_t collideWithMaskBits = GetCollisionBitsMask();
	datas.GetPublicElement("CollideWithMaskBits", &collideWithMaskBits);

	m_localPosition = localPosition;
	m_localRotation = localRotation;

	if (m_pCollider)
	{
		PhysicsServer::SetLocalPosition(m_localPosition, *this);
		PhysicsServer::SetLocalRotation(m_localRotation, *this);
	}

	SetBounciness(bounciness);
	SetFrictionCoefficient(friction);
	SetMassDensity(massDensity);
	SetIsTrigger(isTrigger);
	SetIsSimulationCollider(isSimulationCollider);
	SetIsWorldQueryCollider(isWorldQueryCollider);
	SetCollisionCategoryBits(collisionCategoryBits);
	SetCollideWithMaskBits(collideWithMaskBits);
}

void NodeCollider::ContactEvent(NodeCollider &other)
{
	OnContact(*this, *other.m_pNodeRigidBody);
}
void NodeCollider::TriggerEvent(NodeCollider &other)
{
	OnTrigger(*this, *other.m_pNodeRigidBody);
}

void NodeBoxCollider::SetShape(const glm::vec3 &halfExtents)
{
	PhysicsServer::SetBoxShape(halfExtents, *this);
}
void NodeBoxCollider::DestroyShape()
{
	PhysicsServer::GetPhysicsCommon().destroyBoxShape(static_cast<rp3d::BoxShape *>(m_pShape));
}

void NodeBoxCollider::Serialize(SerializedObject &datas) const
{
	NodeCollider::Serialize(datas);
	datas.SetType("NodeBoxCollider");

	glm::vec3 halfExtents{0.5f, 0.5f, 0.5f};
	if (m_pShape)
	{
		auto *boxShape = static_cast<rp3d::BoxShape *>(m_pShape);
		auto he = boxShape->getHalfExtents();
		halfExtents = {he.x, he.y, he.z};
	}

	datas.AddPublicElement("HalfExtents", &halfExtents);
}

void NodeBoxCollider::Deserialize(SerializedObject const &datas)
{
	glm::vec3 halfExtents{0.5f, 0.5f, 0.5f};
	datas.GetPublicElement("HalfExtents", &halfExtents);
	SetShape(halfExtents);

	NodeCollider::Deserialize(datas);
}

ISerializable *NodeBoxCollider::CreateInstance()
{
	return CreateNode<NodeBoxCollider>("NodeBoxCollider").release();
}

void NodeSphereCollider::SetShape(float radius)
{
	PhysicsServer::SetSphereShape(radius, *this);
}

void NodeSphereCollider::DestroyShape()
{
	PhysicsServer::GetPhysicsCommon().destroySphereShape(static_cast<rp3d::SphereShape *>(m_pShape));
}

void NodeSphereCollider::Serialize(SerializedObject &datas) const
{
	NodeCollider::Serialize(datas);
	datas.SetType("NodeSphereCollider");

	float radius = 0.5f;
	if (m_pShape)
		radius = static_cast<rp3d::SphereShape *>(m_pShape)->getRadius();

	datas.AddPublicElement("Radius", &radius);
}

void NodeSphereCollider::Deserialize(SerializedObject const &datas)
{
	float radius = 0.5f;
	datas.GetPublicElement("Radius", &radius);
	SetShape(radius);

	NodeCollider::Deserialize(datas);
}

ISerializable *NodeSphereCollider::CreateInstance()
{
	return CreateNode<NodeSphereCollider>("NodeSphereCollider").release();
}

void NodeCapsuleCollider::SetShape(float radius, float height)
{
	PhysicsServer::SetCapsuleShape(radius, height, *this);
}

void NodeCapsuleCollider::DestroyShape()
{
	PhysicsServer::GetPhysicsCommon().destroyCapsuleShape(static_cast<rp3d::CapsuleShape *>(m_pShape));
}

void NodeCapsuleCollider::Serialize(SerializedObject &datas) const
{
	NodeCollider::Serialize(datas);
	datas.SetType("NodeCapsuleCollider");

	float radius = 0.5f;
	float height = 1.0f;
	if (m_pShape)
	{
		auto *capsule = static_cast<rp3d::CapsuleShape *>(m_pShape);
		radius = capsule->getRadius();
		height = capsule->getHeight();
	}

	datas.AddPublicElement("Radius", &radius);
	datas.AddPublicElement("Height", &height);
}

void NodeCapsuleCollider::Deserialize(SerializedObject const &datas)
{
	float radius = 0.5f;
	float height = 1.0f;
	datas.GetPublicElement("Radius", &radius);
	datas.GetPublicElement("Height", &height);
	SetShape(radius, height);

	NodeCollider::Deserialize(datas);
}

ISerializable *NodeCapsuleCollider::CreateInstance()
{
	return CreateNode<NodeCapsuleCollider>("NodeCapsuleCollider").release();
}
