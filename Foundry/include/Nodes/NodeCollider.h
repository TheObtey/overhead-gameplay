#ifndef FOUNDRY_NODECOLLIDER__H_
#define FOUNDRY_NODECOLLIDER__H_

#include "Node.h"
#include "NodeRigidBody.h"

#include <functional>

class NodeCollider : public Node3D
{
public:
	////////////////////////////////////////////////////////////
	// Engine

	class Proxy;

	NodeCollider(std::string const& name);
	~NodeCollider() override;

	void AttachToRigidBody(NodeRigidBody* rigidBody);
	void Detach();
	bool IsAttached() const { return m_pCollider != nullptr; }

	// Engine
	////////////////////////////////////////////////////////////
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

	// =========== Local transform (offset from RigidBody) ===========

	void SetLocalPosition(const glm::vec3& pos);
	void SetLocalRotation(const glm::quat& rot);
	glm::vec3 const& GetLocalPosition() const { return m_localPosition; }
	glm::quat const& GetLocalRotation() const { return m_localRotation; }

	// =========== Material ===========

	// 0.0 < v < 1.0
	void SetBounciness(float bounciness);
	// 0.0 < v < 1.0
	float GetBounciness() const;
	void SetFrictionCoefficient(float friction);
	float GetFrictionCoefficient() const;
	void SetMassDensity(float density);
	float GetMassDensity() const;

	// =========== Collider behavior ===========

	void SetIsTrigger(bool trigger);
	bool IsTrigger() const;
	void SetIsSimulationCollider(bool enabled);
	bool IsSimulationCollider() const;
	void SetIsWorldQueryCollider(bool enabled);
	bool IsWorldQueryCollider() const;

	// =========== Collision filtering ===========

	void SetCollisionCategoryBits(uint16_t category);
	uint16_t GetCollisionCategoryBits() const;
	void SetCollideWithMaskBits(uint16_t mask);
	uint16_t GetCollisionBitsMask() const;

	// =========== RP3D Events ===========

	void ContactEvent(NodeCollider& other, int state);
	void TriggerEvent(NodeCollider& other, int state);

	Event<void(NodeCollider&, const NodeRigidBody& data, int state)> OnContact;
	Event<void(NodeCollider&, const NodeRigidBody& data, int state)> OnTrigger;

protected:
	virtual void DestroyShape() = 0;
	void AttachScriptDeserialize(uptr<LuaScriptInstance>& script) override;
	rp3d::Transform GetLocalRp3dTransform() const;

	rp3d::Collider* m_pCollider = nullptr;
	rp3d::CollisionShape* m_pShape = nullptr;
	rp3d::RigidBody* m_pRigidBodyRP3D = nullptr;

	NodeRigidBody* m_pNodeRigidBody = nullptr;

	glm::vec3 m_localPosition{ 0.0f, 0.0f, 0.0f };
	glm::quat m_localRotation{ 1.0f, 0.0f, 0.0f, 0.0f };

	int m_indexInRigidBody = -1;

	friend class PhysicsServer;
};

class NodeBoxCollider : public NodeCollider
{
public:
	class Proxy;
	NodeBoxCollider(std::string const& name) : NodeCollider(name) {};
	~NodeBoxCollider() override {};
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	static ISerializable* CreateInstance();

	void SetShape(const glm::vec3& halfExtents);
	uptr<Node> Clone() override;

private:
	virtual void DestroyShape() override;
};

class NodeSphereCollider : public NodeCollider
{
public:
	class Proxy;
	NodeSphereCollider(std::string const& name) : NodeCollider(name) {};
	~NodeSphereCollider() override {};
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	static ISerializable* CreateInstance();

	void SetShape(float radius);
	uptr<Node> Clone() override;

private:
	virtual void DestroyShape() override;
};

class NodeCapsuleCollider : public NodeCollider
{
public:
	class Proxy;
	NodeCapsuleCollider(std::string const& name) : NodeCollider(name) {};
	~NodeCapsuleCollider() override {};
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	static ISerializable* CreateInstance();

	void SetShape(float radius, float height);
	uptr<Node> Clone() override;

private:
	virtual void DestroyShape() override;
};

REGISTER_ISERIALIZABLE(NodeCollider, NodeCollider::CreateInstance);
REGISTER_ISERIALIZABLE(NodeBoxCollider, NodeBoxCollider::CreateInstance);
REGISTER_ISERIALIZABLE(NodeSphereCollider, NodeSphereCollider::CreateInstance);
REGISTER_ISERIALIZABLE(NodeCapsuleCollider, NodeCapsuleCollider::CreateInstance);

#include "Scripting/Proxies/NodeColliderProxy.inl"
#include "Scripting/Proxies/NodeSpeColliderProxy.inl"

#endif // !NODECOLLIDER__H_