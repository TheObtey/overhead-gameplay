#ifndef FOUNDRY_NODERIGIDBODY__H_
#define FOUNDRY_NODERIGIDBODY__H_

#include "Node.h"
#include "Nodes/Node3D.h"

#include <reactphysics3d/reactphysics3d.h>

static glm::vec3 rp3dToGlm(const rp3d::Vector3& vec) { return glm::vec3(vec.x, vec.y, vec.z); }
static rp3d::Vector3 glmToRp3d(const glm::vec3& vec) { return rp3d::Vector3(vec.x, vec.y, vec.z); }

enum class CollisionShape { SPHERE, CAPSULE, CONVEX_POLYHEDRON, CONCAVE_SHAPE };
enum class CollisionShapeName { TRIANGLE, SPHERE, CAPSULE, BOX, CONVEX_MESH, TRIANGLE_MESH, HEIGHTFIELD };


enum class RigidBodyType { STATIC, KINEMATIC, DYNAMIC, NONE };

class NodeRigidBody : public Node
{
public:

	////////////////////////////////////////////////////////////
	// Engine
	
	class Proxy;

	NodeRigidBody(std::string const& name);
	NodeRigidBody(std::string const& name, Node3D* owner);
	~NodeRigidBody() override;

	virtual void OnUpdate(double delta) override;

	void SetNode3DParent(Node3D* owner);
	rp3d::RigidBody& GetRigidBody() { return *m_pRigidBody; }

	// Engine
	////////////////////////////////////////////////////////////



	// =========== Forces and Torques ===========

	void ApplyLocalForceAtCenterOfMass(const glm::vec3& force);
	void ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point);
	void ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point);

	void ApplyWorldForceAtCenterOfMass(const glm::vec3& force);
	void ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point);
	void ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point);

	void ApplyLocalTorque(const glm::vec3& torque);
	void ApplyWorldTorque(const glm::vec3& torque);

	glm::vec3 const GetLinearVelocity() const;
	glm::vec3 const GetAngularVelocity() const;
	/// Return the linear decelerating factor			
	float const		GetLinearDamping() const;
	/// Return the angular velocity damping factor		
	float const		GetAngularDamping() const;
	glm::vec3 const GetTotalForce() const;


	void SetLinearVelocity(const glm::vec3& velocity);
	void SetAngularVelocity(const glm::vec3& velocity);
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
	void  SetMass(float mass);

	RigidBodyType GetBodyType() const;
	void          SetBodyType(RigidBodyType type);


	// =========== Sleeping and Gravity ===========

	bool IsSleeping() const;
	bool IsAllowedToSleep() const;
	bool IsGravityEnabled();

	void SetSleepingEnabled(bool enabled);
	void SetSleepingState(bool isSleeping);
	void SetIsGravityEnabled(bool enabled);


	// fonction temporaire debug
	glm::vec3 Getposition() const 
	{ 
		return rp3dToGlm(m_pRigidBody->getTransform().getPosition());
	}


protected:

private:

	Node3D* m_pNode3D;
	rp3d::RigidBody* m_pRigidBody;

	friend class PhysicsServer;
};

REGISTER_ISERIALIZABLE(NodeRigidBody, NodeRigidBody::CreateInstance);

#include "Scripting/Proxies/NodeRigidBodyProxy.inl"

#endif // -FOUNDRY_NODERIGIDBODY__H_