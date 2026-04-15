#include "Nodes/NodeRigidBody.h"
#include "Serialization/SerializeObject.hpp"

#include "Servers/PhysicsServer.h"
#include "Nodes/NodeCollider.h"
#include <Debug.h>

NodeRigidBody::~NodeRigidBody()
{
	m_colliders.clear();
}

NodeRigidBody::NodeRigidBody(std::string const& name) : Node3D(name)
{
	//OnSceneEnter.Subscribe([this](Node& self)
	//	{
	//	});
	OnParentChange.Subscribe([this](Node& self)
		{
			if (dynamic_cast<Node3D*>(m_pOwner))
				m_pNode3DParent = static_cast<Node3D*>(m_pOwner);
			else
				m_pNode3DParent = this;
		});

	//OnSceneLeave.Subscribe([this](Node& self)
	//	{
	//	});
	//m_pOwner->Update(0.016);
	//PhysicsServer::CreateRigidBody(*this);
}

NodeRigidBody::NodeRigidBody(std::string const& name, Node3D* owner) : Node3D(name)
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
	m_pNode3DParent->Update(0.016);

	PhysicsServer::CreateRigidBody(*this);
}
void NodeRigidBody::OnUpdate(double delta)
{
	Node3D::OnUpdate(delta);
	if (m_rigidBodyCreated == false) return;

	Node3D* parent = static_cast<Node3D*>(m_pNode3DParent);

	if (m_pRigidBodyRP3D->getType() == rp3d::BodyType::DYNAMIC)
	{
		auto& pos = m_pRigidBodyRP3D->getTransform().getPosition();
		auto& rot = m_pRigidBodyRP3D->getTransform().getOrientation();
		parent->SetWorldPosition({ pos.x, pos.y, pos.z });
		parent->SetWorldRotationQuaternion({ rot.w, rot.x, rot.y, rot.z });
	}
	else
	{
		auto pos = parent->GetWorldPosition();
		auto& rot = parent->GetWorldRotationQuaternion();
		rp3d::Transform t;
		t.setPosition({ pos.x,pos.y,pos.z });
		t.setOrientation({ rot.x, rot.y, rot.z, rot.w });
		m_pRigidBodyRP3D->setTransform(t);
		return;
	}
}


void NodeRigidBody::Serialize(SerializedObject& datas) const
{
	Node3D::Serialize(datas);
	datas.SetType("NodeRigidBody");

	int bodyType = static_cast<int>(RigidBodyType::NONE);
	if (m_pRigidBodyRP3D != nullptr)
	{
		auto type = m_pRigidBodyRP3D->getType();
		if (type == rp3d::BodyType::DYNAMIC)
			bodyType = static_cast<int>(RigidBodyType::DYNAMIC);
		else if (type == rp3d::BodyType::KINEMATIC)
			bodyType = static_cast<int>(RigidBodyType::KINEMATIC);
		else
			bodyType = static_cast<int>(RigidBodyType::STATIC);
	}
	datas.AddPublicElement("BodyType", &bodyType);

	float mass = (m_pRigidBodyRP3D != nullptr) ? m_pRigidBodyRP3D->getMass() : 0.0f;
	datas.AddPublicElement("Mass", &mass);

	glm::vec3 linearVelocity{ 0.0f };
	glm::vec3 angularVelocity{ 0.0f };
	float linearDamping = 0.0f;
	float angularDamping = 0.0f;
	bool gravityEnabled = false;
	bool sleepingEnabled = false;
	bool isSleeping = false;
	bool lockLinearX = false, lockLinearY = false, lockLinearZ = false;
	bool lockAngularX = false, lockAngularY = false, lockAngularZ = false;

	if (m_pRigidBodyRP3D)
	{
		linearVelocity = rp3dToGlm(m_pRigidBodyRP3D->getLinearVelocity());
		angularVelocity = rp3dToGlm(m_pRigidBodyRP3D->getAngularVelocity());
		linearDamping = m_pRigidBodyRP3D->getLinearDamping();
		angularDamping = m_pRigidBodyRP3D->getAngularDamping();
		gravityEnabled = m_pRigidBodyRP3D->isGravityEnabled();
		sleepingEnabled = m_pRigidBodyRP3D->isAllowedToSleep();
		isSleeping = m_pRigidBodyRP3D->isSleeping();

		auto linearFactor = m_pRigidBodyRP3D->getLinearLockAxisFactor();
		lockLinearX = linearFactor.x == 0.0f;
		lockLinearY = linearFactor.y == 0.0f;
		lockLinearZ = linearFactor.z == 0.0f;

		auto angularFactor = m_pRigidBodyRP3D->getAngularLockAxisFactor();
		lockAngularX = angularFactor.x == 0.0f;
		lockAngularY = angularFactor.y == 0.0f;
		lockAngularZ = angularFactor.z == 0.0f;
	}
	datas.AddPublicElement("LinearVelocity", &linearVelocity);
	datas.AddPublicElement("AngularVelocity", &angularVelocity);
	datas.AddPublicElement("LinearDamping", &linearDamping);
	datas.AddPublicElement("AngularDamping", &angularDamping);
	datas.AddPublicElement("IsGravityEnabled", &gravityEnabled);
	datas.AddPublicElement("IsAllowedToSleep", &sleepingEnabled);
	datas.AddPublicElement("IsSleeping", &isSleeping);
	datas.AddPublicElement("LockLinearX", &lockLinearX);
	datas.AddPublicElement("LockLinearY", &lockLinearY);
	datas.AddPublicElement("LockLinearZ", &lockLinearZ);
	datas.AddPublicElement("LockAngularX", &lockAngularX);
	datas.AddPublicElement("LockAngularY", &lockAngularY);
	datas.AddPublicElement("LockAngularZ", &lockAngularZ);
}

void NodeRigidBody::Deserialize(SerializedObject const& datas)
{
	Node3D::Deserialize(datas);

	int bodyTypeValue = static_cast<int>(RigidBodyType::STATIC);
	datas.GetPublicElement("BodyType", &bodyTypeValue);

	float mass = 1.0f;
	datas.GetPublicElement("Mass", &mass);

	glm::vec3 linearVelocity{ 0.0f };
	datas.GetPublicElement("LinearVelocity", &linearVelocity);

	glm::vec3 angularVelocity{ 0.0f };
	datas.GetPublicElement("AngularVelocity", &angularVelocity);

	float linearDamping = 0.0f;
	datas.GetPublicElement("LinearDamping", &linearDamping);

	float angularDamping = 0.0f;
	datas.GetPublicElement("AngularDamping", &angularDamping);

	bool gravityEnabled = true;
	datas.GetPublicElement("IsGravityEnabled", &gravityEnabled);

	bool sleepingEnabled = true;
	datas.GetPublicElement("IsAllowedToSleep", &sleepingEnabled);

	bool isSleeping = false;
	datas.GetPublicElement("IsSleeping", &isSleeping);

	bool lockLinearX = false, lockLinearY = false, lockLinearZ = false;
	datas.GetPublicElement("LockLinearX", &lockLinearX);
	datas.GetPublicElement("LockLinearY", &lockLinearY);
	datas.GetPublicElement("LockLinearZ", &lockLinearZ);

	bool lockAngularX = false, lockAngularY = false, lockAngularZ = false;
	datas.GetPublicElement("LockAngularX", &lockAngularX);
	datas.GetPublicElement("LockAngularY", &lockAngularY);
	datas.GetPublicElement("LockAngularZ", &lockAngularZ);
	
	if (m_rigidBodyCreated == false){
		CreateRigidBody();
		PhysicsServer::FlushCommands();
	}

	SetBodyType(static_cast<RigidBodyType>(bodyTypeValue));
	SetMass(mass);
	SetLinearDamping(linearDamping);
	SetAngularDamping(angularDamping);
	SetIsGravityEnabled(gravityEnabled);
	SetSleepingEnabled(sleepingEnabled);
	SetSleepingState(isSleeping);
	LockLinearAxis(lockLinearX, lockLinearY, lockLinearZ);
	LockAngularAxis(lockAngularX, lockAngularY, lockAngularZ);
	SetLinearVelocity(linearVelocity);
	SetAngularVelocity(angularVelocity);

}

void NodeRigidBody::AttachScriptDeserialize(uptr<LuaScriptInstance>& script)
{
	AttachScript<NodeRigidBody>(script, *this);
}

ISerializable* NodeRigidBody::CreateInstance()
{
	return CreateNode<NodeRigidBody>("NodeRigidBody").release();
}

NodeRigidBody::operator reactphysics3d::Transform()
{
	rp3d::Transform reactTr;
	Node3D* parent = static_cast<Node3D*>(m_pNode3DParent);
	auto pos = parent->GetWorldPosition();
	auto rot = parent->GetWorldRotationQuaternion();
	reactTr.setPosition({ pos.x, pos.y, pos.z });
	reactTr.setOrientation({ rot.x, rot.y, rot.z, rot.w });

	return reactTr;
}

NodeRigidBody::operator reactphysics3d::Transform* ()
{
	rp3d::Transform* reactTr = new rp3d::Transform();
	Node3D* parent = static_cast<Node3D*>(m_pNode3DParent);
	auto pos = parent->GetWorldPosition();
	auto rot = parent->GetWorldRotationQuaternion();
	reactTr->setPosition({ pos.x, pos.y, pos.z });
	reactTr->setOrientation({ rot.x, rot.y, rot.z, rot.w });

	return reactTr;
}

void NodeRigidBody::AddCollider(NodeCollider& collider)
{
	PhysicsServer::AddCollider(collider, *this);
}

void NodeRigidBody::SetBounciness(float v)
{
	if (m_colliders.empty()) return;
	for (auto c : m_colliders)
		c->SetBounciness(v);
}
float NodeRigidBody::GetBounciness() const
{
	return m_colliders.empty() ? -1.0f : m_colliders[0]->GetBounciness();
}
void NodeRigidBody::SetFrictionCoefficient(float v)
{
	if (m_colliders.empty()) return;
	for (auto c : m_colliders)
		c->SetFrictionCoefficient(v);
}
float NodeRigidBody::GetFrictionCoefficient() const
{
	return m_colliders.empty() ? -1.0f : m_colliders[0]->GetFrictionCoefficient();
}
void NodeRigidBody::SetMassDensity(float v)
{
	if (m_colliders.empty()) return;
	for (auto c : m_colliders)
		c->SetMassDensity(v);
}
float NodeRigidBody::GetMassDensity() const
{
	return m_colliders.empty() ? -1.0f : m_colliders[0]->GetMassDensity();
}


void NodeRigidBody::SetBounciness(int colliderIndex, float v)
{
	if (m_colliders.empty() || colliderIndex > m_colliders.size()) return;

	m_colliders[colliderIndex]->SetBounciness(v);
}
float NodeRigidBody::GetBounciness(int colliderIndex) const
{
	return m_colliders.empty() ? -1.0f : m_colliders[colliderIndex]->GetBounciness();
}
void NodeRigidBody::SetFrictionCoefficient(int colliderIndex, float v)
{
	if (m_colliders.empty() || colliderIndex > m_colliders.size()) return;

	m_colliders[colliderIndex]->SetFrictionCoefficient(v);
}
float NodeRigidBody::GetFrictionCoefficient(int colliderIndex) const
{
	return m_colliders.empty() ? -1.0f : m_colliders[colliderIndex]->GetFrictionCoefficient();
}
void NodeRigidBody::SetMassDensity(int colliderIndex, float v)
{
	if (m_colliders.empty() || colliderIndex > m_colliders.size()) return;

	m_colliders[colliderIndex]->SetMassDensity(v);
}
float NodeRigidBody::GetMassDensity(int colliderIndex) const
{
	return m_colliders.empty() ? -1.0f : m_colliders[colliderIndex]->GetMassDensity();
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
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return { 0,0,0 };
	return rp3dToGlm(m_pRigidBodyRP3D->getLinearVelocity());
}
glm::vec3 const NodeRigidBody::GetAngularVelocity() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return { 0,0,0 };
	return rp3dToGlm(m_pRigidBodyRP3D->getAngularVelocity());
}
/// Return the linear decelerating factor
float const NodeRigidBody::GetLinearDamping() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return 0.0f;
	return m_pRigidBodyRP3D->getLinearDamping();
}
/// Return the angular velocity damping factor
float const NodeRigidBody::GetAngularDamping() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return 0.0f;
	return m_pRigidBodyRP3D->getAngularDamping();
}

glm::vec3 const NodeRigidBody::GetTotalForce() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return { 0,0,0 };
	return rp3dToGlm(m_pRigidBodyRP3D->getForce());
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
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return 0.0f;
	return m_pRigidBodyRP3D->getMass();
}
void NodeRigidBody::SetMass(float mass)
{
	PhysicsServer::SetMass(mass, *this);
}

RigidBodyType NodeRigidBody::GetBodyType() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return RigidBodyType::NONE;
	auto type = m_pRigidBodyRP3D->getType();

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

bool NodeRigidBody::IsAllowedToSleep() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return false;
	return m_pRigidBodyRP3D->isAllowedToSleep();
}
bool NodeRigidBody::IsSleeping() const
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return false;
	return m_pRigidBodyRP3D->isSleeping();
}
bool NodeRigidBody::IsGravityEnabled()
{
	if (m_pNode3DParent == nullptr || m_pRigidBodyRP3D == nullptr) return false;
	return m_pRigidBodyRP3D->isGravityEnabled();
}

void NodeRigidBody::SetSleepingEnabled(bool enabled)
{
	PhysicsServer::SetSleepingEnabled(enabled, *this);
}
void NodeRigidBody::SetSleepingState(bool isSleeping)
{
	PhysicsServer::SetSleepingState(isSleeping, *this);
}
void NodeRigidBody::SetIsGravityEnabled(bool enabled)
{
	PhysicsServer::SetIsGravityEnabled(enabled, *this);
}

uptr<Node> NodeRigidBody::Clone()
{
	uptr<NodeRigidBody> clone = Node::CreateNode<NodeRigidBody>(GetName());

	SerializedObject datas;
	Serialize(datas);
	clone->Deserialize(datas);

	return clone;
}
