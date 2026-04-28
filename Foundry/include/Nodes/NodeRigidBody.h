#ifndef FOUNDRY_NODERIGIDBODY__H_
#define FOUNDRY_NODERIGIDBODY__H_

#include "Node.h"
#include "Nodes/Node3D.h"

#include <reactphysics3d/reactphysics3d.h>

static glm::vec3 rp3dToGlm(const rp3d::Vector3 &vec) { return glm::vec3(vec.x, vec.y, vec.z); }
static rp3d::Vector3 glmToRp3d(const glm::vec3 &vec) { return rp3d::Vector3(vec.x, vec.y, vec.z); }

enum class RigidBodyType
{
	STATIC,
	KINEMATIC,
	DYNAMIC,
	NONE
};

class NodeCollider;

class NodeRigidBody : public Node3D
{
public:
	////////////////////////////////////////////////////////////
	// Engine

	class Proxy;

	NodeRigidBody(std::string const &name);
	NodeRigidBody(std::string const &name, Node3D *owner);
	~NodeRigidBody() override;

	void CreateRigidBody();
	void DestroyRigidBody(NodeRigidBody &rigidBody);

	virtual void OnUpdate(double delta) override;

	void SetNode3DParent(Node3D *owner);
	rp3d::RigidBody *GetRigidBody() { return m_pRigidBodyRP3D; }
	void SetRigidBody(rp3d::RigidBody *rb) { m_pRigidBodyRP3D = rb; }

	operator rp3d::Transform();
	operator rp3d::Transform *();

	// Engine
	////////////////////////////////////////////////////////////

	virtual void Serialize(SerializedObject &datas) const override;
	virtual void Deserialize(SerializedObject const &datas) override;
	static ISerializable *CreateInstance();

	// =========== Material ===========

	void AddCollider(NodeCollider &collider);
	std::vector<NodeCollider *>& GetColliders() { return m_colliders; }

	void SetBounciness(float bounciness);
	float GetBounciness() const;
	void SetFrictionCoefficient(float friction);
	float GetFrictionCoefficient() const;
	void SetMassDensity(float density);
	float GetMassDensity() const;

	void SetBounciness(int colliderIndex, float bounciness);
	float GetBounciness(int colliderIndex) const;
	void SetFrictionCoefficient(int colliderIndex, float friction);
	float GetFrictionCoefficient(int colliderIndex) const;
	void SetMassDensity(int colliderIndex, float density);
	float GetMassDensity(int colliderIndex) const;

	// =========== Forces and Torques ===========

	void ApplyLocalForceAtCenterOfMass(const glm::vec3 &force);
	void ApplyLocalForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &point);
	void ApplyLocalForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &point);

	void ApplyWorldForceAtCenterOfMass(const glm::vec3 &force);
	void ApplyWorldForceAtLocalPosition(const glm::vec3 &force, const glm::vec3 &point);
	void ApplyWorldForceAtWorldPosition(const glm::vec3 &force, const glm::vec3 &point);

	void ApplyLocalTorque(const glm::vec3 &torque);
	void ApplyWorldTorque(const glm::vec3 &torque);

	glm::vec3 const GetLinearVelocity() const;
	glm::vec3 const GetAngularVelocity() const;
	/// Return the linear decelerating factor
	float const GetLinearDamping() const;
	/// Return the angular velocity damping factor
	float const GetAngularDamping() const;
	glm::vec3 const GetTotalForce() const;

	void SetLinearVelocity(const glm::vec3 &velocity);
	void SetAngularVelocity(const glm::vec3 &velocity);
	/// Set the linear decelerating factor
	void SetLinearDamping(float linearDamping);
	/// Set the angular decelerating factor
	void SetAngularDamping(float angularDamping);

	void LockLinearAxis(bool x, bool y, bool z);
	void LockAngularAxis(bool x, bool y, bool z);

	void ResetForces();
	void ResetTorque();

	// =========== Mass and Body Type ===========

	float GetMass() const;
	void SetMass(float mass);

	RigidBodyType GetBodyType() const;
	void SetBodyType(RigidBodyType type);

	// =========== Sleeping and Gravity ===========

	bool IsAllowedToSleep() const;
	bool IsSleeping() const;
	bool IsGravityEnabled();

	void SetSleepingEnabled(bool enabled);
	void SetSleepingState(bool isSleeping);
	void SetIsGravityEnabled(bool enabled);

	Node3D* GetNode3DParent()
	{
		return m_pNode3DParent;
	}

#ifdef DEBUG_BUILD
	glm::vec3 GetPosition() const
	{
		return rp3dToGlm(m_pRigidBodyRP3D->getTransform().getPosition());
	}
#endif
	uptr<Node> Clone() override;

protected:
	void AttachScriptDeserialize(uptr<LuaScriptInstance>& script) override;

private:
	Node3D* m_pNode3DParent = this;
	rp3d::RigidBody *m_pRigidBodyRP3D = nullptr;

	std::vector<NodeCollider *> m_colliders;
	bool m_rigidBodyCreated = false;
	friend class PhysicsServer;
};

REGISTER_ISERIALIZABLE(NodeRigidBody, NodeRigidBody::CreateInstance);

#include "Scripting/Proxies/NodeRigidBodyProxy.inl"

#endif // -FOUNDRY_NODERIGIDBODY__H_