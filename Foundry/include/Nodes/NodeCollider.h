#ifndef FOUNDRY_NODECOLLIDER__H_
#define FOUNDRY_NODECOLLIDER__H_

#include "Node.h"
#include "NodeRigidBody.h"

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

	//rp3d::Collider* GetCollider() const { return m_pCollider; }

	// Engine
	////////////////////////////////////////////////////////////



	// =========== Local transform (offset from RigidBody) ===========

	void SetLocalPosition(const glm::vec3& pos);
	void SetLocalRotation(const glm::quat& rot);
	glm::vec3 const& GetLocalPosition() const { return m_localPosition; }
	glm::quat const& GetLocalRotation() const { return m_localRotation; }

	// =========== Material ===========

	// 0.0 < v < 1.0
	void  SetBounciness(float bounciness);
	// 0.0 < v < 1.0
	float GetBounciness() const;
	void  SetFrictionCoefficient(float friction);
	float GetFrictionCoefficient() const;
	void  SetMassDensity(float density);
	float GetMassDensity() const;

	// =========== Collider behavior ===========

	void SetIsTrigger(bool trigger);
	bool IsTrigger() const;
	void SetIsSimulationCollider(bool enabled);
	bool IsSimulationCollider() const;
	void SetIsWorldQueryCollider(bool enabled);
	bool IsWorldQueryCollider() const;

	// =========== Collision filtering ===========

	void     SetCollisionCategoryBits(uint16_t category);
	uint16_t GetCollisionCategoryBits() const;
	void     SetCollideWithMaskBits(uint16_t mask);
	uint16_t GetCollisionBitsMask() const;


protected:
	virtual void      DestroyShape() = 0;
	rp3d::Transform   GetLocalRp3dTransform() const;

	rp3d::Collider* m_pCollider		= nullptr;
	rp3d::CollisionShape* m_pShape	= nullptr;  
	rp3d::RigidBody* m_pRigidBody	= nullptr;

	NodeRigidBody* m_pAttachedRigidBody = nullptr;

	glm::vec3 m_localPosition{ 0.0f, 0.0f, 0.0f };
	glm::quat m_localRotation{ 1.0f, 0.0f, 0.0f, 0.0f };

	int m_indexInRigidBody = -1;

	friend class PhysicsServer;
};
class NodeBoxCollider : public NodeCollider
{
public:
	NodeBoxCollider(std::string const& name) : NodeCollider(name) {};
	~NodeBoxCollider() override {};
	void SetShape(const glm::vec3& halfExtents);
private:
	virtual void DestroyShape() override;
};
class NodeSphereCollider : public NodeCollider
{
public:
	NodeSphereCollider(std::string const& name) : NodeCollider(name) {};
	~NodeSphereCollider() override {};
	void SetShape(float radius);
private:
	virtual void DestroyShape() override;
};
class NodeCapsuleCollider : public NodeCollider
{
public:
	NodeCapsuleCollider(std::string const& name) : NodeCollider(name) {};
	~NodeCapsuleCollider() override {};
	void SetShape(float radius, float height);
private:
	virtual void DestroyShape() override;
};

REGISTER_ISERIALIZABLE(NodeCollider, NodeCollider::CreateInstance);

#include "Scripting/Proxies/NodeColliderProxy.inl"

#endif // !NODECOLLIDER__H_