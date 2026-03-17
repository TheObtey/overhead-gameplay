#ifndef PHYSICS_SERVER__H_
#define PHYSICS_SERVER__H_

#include "Server.hpp"
#include "Node.h"
#include "Nodes/NodeRigidBody.h"
//class NodeRigidBody;
class NodeCollider;

#include <iostream>
#include <memory>

#include <reactphysics3d/reactphysics3d.h>
#include <glm/ext/vector_float3.hpp>

template <>
struct Command<class PhysicsServer>
{

	Command()
	{
		transform = nullptr;
		position = { 0.0f, 0.0f, 0.0f };
		force = { 0.0f, 0.0f, 0.0f };		
		bodyType = RigidBodyType::NONE;
		mass = 0.0f;
		enabled = false;
		lockAxis[3] = { false };
	};
	~Command()
	{
		if(transform)
		{
			delete transform;
			transform = nullptr;
		}
	}


	enum class CmdType : uint16
	{
		// ========= Rigid Body commands ===========

		CREATE_RIGID_BODY, 
		DESTROY_RIGID_BODY,

		APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS,
		APPLY_LOCAL_FORCE_AT_LOCAL_POSITION,
		APPLY_LOCAL_FORCE_AT_WORLD_POSITION,

		APPLY_WORLD_FORCE_AT_CENTER_OF_MASS,
		APPLY_WORLD_FORCE_AT_LOCAL_POSITION,
		APPLY_WORLD_FORCE_AT_WORLD_POSITION,

		APPLY_LOCAL_TORQUE,
		APPLY_WORLD_TORQUE,

		SET_LINEAR_VELOCITY,
		SET_ANGULAR_VELOCITY,
		SET_LINEAR_DAMPING,
		SET_ANGULAR_DAMPING,

		LOCK_LINEAR_AXIS,
		LOCK_ANGULAR_AXIS,

		RESET_FORCES,
		RESET_TORQUE,

		SET_MASS,
		SET_BODY_TYPE,

		SET_SLEEPING_ENABLED,
		SET_SLEEPING_STATE,
		SET_IS_GRAVITY_ENABLED,


		// ========= Collider commands ===========

		ATTACH_TO_RIGID_BODY,
		DETACH,
		DESTROY_SHAPE,

		SET_BOX_SHAPE,
		SET_SPHERE_SHAPE,
		SET_CAPSULE_SHAPE,

		SET_LOCAL_POSITION,
		SET_LOCAL_ROTATION,

		SET_BOUNCINESS,
		SET_FRICTION_COEFFICIENT,
		SET_MASS_DENSITY,

		SET_IS_TRIGGER,
		SET_IS_SIMULATION_COLLIDER,
		SET_IS_WORLD_QUERY_COLLIDER,

		SET_COLLISION_CATEGORY_BITS,
		SET_COLLIDE_WITH_MASK_BITS

	} Type;



	Node* const To = nullptr;
	
	// Rigid body command parameters

	rp3d::Transform* transform;
	glm::vec3 position;
	union
	{
		glm::vec3 gravity;
		glm::vec3 velocity;
		glm::vec3 force;
		glm::vec3 torque;
	};
	RigidBodyType bodyType;
	union
	{
		float linearDamping;
		float angularDamping;
		float mass;
	};
	union
	{
		bool enabled;
		bool isSleeping;
	};
	bool lockAxis[3];

	// Collider command parameters

};

using CommandTyp = Command<PhysicsServer>::CmdType;

class PhysicsServer : public Server<PhysicsServer>
{
public:

	PhysicsServer();
	~PhysicsServer();



	static void Initialize();

	static rp3d::RigidBody* CreateRigidBody(const rp3d::Transform& transform, Node* const To);
	static void             DestroyRigidBody(NodeRigidBody& rigidBody);

	//static void Update(double dt)                       { Instance().m_pPhysicsWorld->update(dt); }

	//static rp3d::PhysicsCommon& GetPhysicsCommon()      { return Instance().m_physicsCommon; }
	//static rp3d::PhysicsWorld& GetPhysicsWorld()        { return *Instance().m_pPhysicsWorld; }
	static rp3d::PhysicsCommon& GetPhysicsCommon() { return m_physicsCommon; }
	static rp3d::PhysicsWorld& GetPhysicsWorld() { return *m_pPhysicsWorld; }
	static void UpdatePhysicsWorld(double dt) { m_pPhysicsWorld->update(dt); }

	//static void SetGravity(const glm::vec3& gravity)    { Instance().m_pPhysicsWorld->setGravity(glmToRp3d(gravity)); }
	//static glm::vec3 GetGravity()                       { return rp3dToGlm(Instance().m_pPhysicsWorld->getGravity()); }
	static void SetGravity(const glm::vec3& gravity)    { m_pPhysicsWorld->setGravity(glmToRp3d(gravity)); }
	static glm::vec3 GetGravity()                       { return rp3dToGlm(m_pPhysicsWorld->getGravity()); }

	//static void EnableSleeping(bool enabled)            { Instance().m_pPhysicsWorld->enableSleeping(enabled); }


	// =========== Rigid Body intermediate functions ===========

	void ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb);
	void ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);
	void ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);

	void ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb);
	void ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);
	void ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);

	void ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb);
	void ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb);

	void SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb);
	void SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb);
	/// Set the linear decelerating factor				
	void SetLinearDamping(float linearDamping, NodeRigidBody& rb);
	/// Set the angular decelerating factor				
	void SetAngularDamping(float angularDamping, NodeRigidBody& rb);

	void LockLinearAxis(bool lockAxis[], NodeRigidBody& rb);
	void LockAngularAxis(bool lockAxis[], NodeRigidBody& rb);

	void ResetForces(NodeRigidBody& rb);
	void ResetTorque(NodeRigidBody& rb);

	void SetMass(float mass, NodeRigidBody& rb);
	void SetBodyType(RigidBodyType type, NodeRigidBody& rb);

	void SetSleepingEnabled(bool enabled, NodeRigidBody& rb);
	void SetSleepingState(bool isSleeping, NodeRigidBody& rb);
	void SetIsGravityEnabled(bool enabled, NodeRigidBody& rb);


	// =========== Collider intermediate functions ===========

	void AttachToRigidBody(rp3d::RigidBody* rigidBody, NodeCollider& c);
	void Detach(NodeCollider& c);
	void DestroyShape(NodeCollider& c);

	void SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c);
	void SetSphereShape(float radius, NodeCollider& c);
	void SetCapsuleShape(float radius, float height, NodeCollider& c);

	void SetLocalPosition(const glm::vec3& pos, NodeCollider& c);
	void SetLocalRotation(const glm::quat& rot, NodeCollider& c);

	void SetBounciness(float bounciness, NodeCollider& c);
	void SetFrictionCoefficient(float friction, NodeCollider& c);
	void SetMassDensity(float density, NodeCollider& c);

	void SetIsTrigger(bool trigger, NodeCollider& c);
	void SetIsSimulationCollider(bool enabled, NodeCollider& c);
	void SetIsWorldQueryCollider(bool enabled, NodeCollider& c);

	void SetCollisionCategoryBits(uint16_t category, NodeCollider& c);
	void SetCollideWithMaskBits(uint16_t mask, NodeCollider& c);

private:
	void FlushCommandsImpl() override;
	void BuildTasksImpl(TaskGraph& graph) override {};
	void OnInitialize() override;
	void OnUnInitialize() override;


	// =========== Rigid Body functions ===========

	void S_ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb);
	void S_ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);
	void S_ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);

	void S_ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb);
	void S_ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);
	void S_ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);

	void S_ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb);
	void S_ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb);

	void S_SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb);
	void S_SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb);
	/// Set the linear decelerating factor				
	void S_SetLinearDamping(float linearDamping, NodeRigidBody& rb);
	/// Set the angular decelerating factor				
	void S_SetAngularDamping(float angularDamping, NodeRigidBody& rb);

	void S_LockLinearAxis(bool lockAxis[], NodeRigidBody& rb);
	void S_LockAngularAxis(bool lockAxis[], NodeRigidBody& rb);

	void S_ResetForces(NodeRigidBody& rb);
	void S_ResetTorque(NodeRigidBody& rb);

	void S_SetMass(float mass, NodeRigidBody& rb);
	void S_SetBodyType(RigidBodyType type, NodeRigidBody& rb);

	void S_SetSleepingEnabled(bool enabled, NodeRigidBody& rb);
	void S_SetSleepingState(bool isSleeping, NodeRigidBody& rb);
	void S_SetIsGravityEnabled(bool enabled, NodeRigidBody& rb);


	// =========== Collider functions ===========

	void S_AttachToRigidBody(rp3d::RigidBody* rigidBody, NodeCollider& c);
	void S_Detach(NodeCollider& c);
	void S_DestroyShape(NodeCollider& c);
		 
	void S_SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c);
	void S_SetSphereShape(float radius, NodeCollider& c);
	void S_SetCapsuleShape(float radius, float height, NodeCollider& c);
		 
	void S_SetLocalPosition(const glm::vec3& pos, NodeCollider& c);
	void S_SetLocalRotation(const glm::quat& rot, NodeCollider& c);
		 
	void S_SetBounciness(float bounciness, NodeCollider& c);
	void S_SetFrictionCoefficient(float friction, NodeCollider& c);
	void S_SetMassDensity(float density, NodeCollider& c);
		 
	void S_SetIsTrigger(bool trigger, NodeCollider& c);
	void S_SetIsSimulationCollider(bool enabled, NodeCollider& c);
	void S_SetIsWorldQueryCollider(bool enabled, NodeCollider& c);
		 
	void S_SetCollisionCategoryBits(uint16_t category, NodeCollider& c);
	void S_SetCollideWithMaskBits(uint16_t mask, NodeCollider& c);



private:
	static rp3d::PhysicsCommon m_physicsCommon;
	static rp3d::PhysicsWorld* m_pPhysicsWorld;
	//rp3d::PhysicsCommon m_physicsCommon;
	//rp3d::PhysicsWorld* m_pPhysicsWorld;

	friend Server<PhysicsServer>;
};

#endif