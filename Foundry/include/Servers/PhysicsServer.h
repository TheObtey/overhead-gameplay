#ifndef PHYSICS_SERVER__H_
#define PHYSICS_SERVER__H_

#include "Server.hpp"
#include "Nodes/NodeRigidBody.h"

#include <iostream>
#include <memory>
#include <reactphysics3d/reactphysics3d.h>
#include <glm/ext/vector_float3.hpp>

class NodeCollider;

template <>
struct Command<class PhysicsServer>
{

	Command() : 
		Type(CmdType::NONE), 
		To(nullptr), 
		collider(nullptr),
		transform(nullptr), 
		position({ 0.0f, 0.0f, 0.0f }), 
		force({ 0.0f, 0.0f, 0.0f }),	
		bodyType(RigidBodyType::NONE),
		mass(0.0f),
		sleepingEnabled(false),
		//lockAxis({}),
		rotation({ 0.0f, 0.0f, 0.0f, 1.0f }),
		radius(0.0f),
		mask(0)
	{
		
	};

	enum class CmdType : uint16
	{
		NONE,
		// ========= Rigid Body commands ===========

		CREATE_RIGID_BODY,
		DESTROY_RIGID_BODY,

		ADD_COLLIDER,
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

	Node* To = nullptr;

	// Rigid body command parameters
	glm::vec3 position;
	RigidBodyType bodyType;
	rp3d::Transform* transform;
	bool lockAxis[3];

	// Collider command parameters
	glm::quat rotation;
	union { uint16_t category, mask; };
	float radius;

	// Both
	union {
		// RigidBody
		bool gravityEnabled, sleepingEnabled, isSleeping;
		// Collider
		bool trigger, isSimulationCollider, isWorldQueryCollider;
	};
	union {
		// RigidBody
		float linearDamping, angularDamping, mass;
		// Collider
		float height, bounciness, friction, density;
	};
	union {
		// RigidBody
		glm::vec3 gravity, velocity, force, torque;
		// Collider
		glm::vec3 halfExtents, pos;
	};
	union {
		// RigidBody
		NodeCollider* collider;
		// Collider
		NodeRigidBody* rigidBody;
	};
};

enum class EventType
{
	Start,
	Stay,
	Exit
};

class PhysicsEvents : public rp3d::EventListener
{
public:
	PhysicsEvents() = default;
	~PhysicsEvents() = default;

	virtual void onContact(const rp3d::CollisionCallback::CallbackData& data) override;

	virtual void onTrigger(const rp3d::OverlapCallback::CallbackData& data) override;
};


using CommandTyp = Command<PhysicsServer>::CmdType;

class PhysicsServer : public Server<PhysicsServer>
{
public:

	PhysicsServer();
	~PhysicsServer();

	static void Initialize();

	static void UpdatePhysicsWorld(double dt)           { Instance().m_pPhysicsWorld->update(dt); }

	static rp3d::PhysicsCommon& GetPhysicsCommon()      { return Instance().m_physicsCommon; }
	static rp3d::PhysicsWorld& GetPhysicsWorld()        { return *Instance().m_pPhysicsWorld; }

	static void SetGravity(const glm::vec3& gravity)    { Instance().m_pPhysicsWorld->setGravity(glmToRp3d(gravity)); }
	static glm::vec3 GetGravity()                       { return rp3dToGlm(Instance().m_pPhysicsWorld->getGravity()); }


	// =========== Rigid Body intermediate functions ===========

	static void CreateRigidBody(NodeRigidBody& rigidBody);
	static void DestroyRigidBody(NodeRigidBody& rigidBody);

	static void AddCollider(NodeCollider& collider, NodeRigidBody& rigidBody);

	static void ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb);
	static void ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);
	static void ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);

	static void ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb);
	static void ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);
	static void ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb);

	static void ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb);
	static void ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb);

	static void SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb);
	static void SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb);
	/// Set the linear decelerating factor				
	static void SetLinearDamping(float linearDamping, NodeRigidBody& rb);
	/// Set the angular decelerating factor				
	static void SetAngularDamping(float angularDamping, NodeRigidBody& rb);

	static void LockLinearAxis(bool lockAxis[], NodeRigidBody& rb);
	static void LockAngularAxis(bool lockAxis[], NodeRigidBody& rb);

	static void ResetForces(NodeRigidBody& rb);
	static void ResetTorque(NodeRigidBody& rb);

	static void SetMass(float mass, NodeRigidBody& rb);
	static void SetBodyType(RigidBodyType type, NodeRigidBody& rb);

	static void SetSleepingEnabled(bool enabled, NodeRigidBody& rb);
	static void SetSleepingState(bool isSleeping, NodeRigidBody& rb);
	static void SetIsGravityEnabled(bool enabled, NodeRigidBody& rb);


	// =========== Collider intermediate functions ===========

	static void AttachToRigidBody(NodeRigidBody* rigidBody, NodeCollider& c);
	static void Detach(NodeCollider& c);
	static void DestroyShape(NodeCollider& c);

	static void SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c);
	static void SetSphereShape(float radius, NodeCollider& c);
	static void SetCapsuleShape(float radius, float height, NodeCollider& c);

	static void SetLocalPosition(const glm::vec3& pos, NodeCollider& c);
	static void SetLocalRotation(const glm::quat& rot, NodeCollider& c);

	static void SetBounciness(float bounciness, NodeCollider& c);
	static void SetFrictionCoefficient(float friction, NodeCollider& c);
	static void SetMassDensity(float density, NodeCollider& c);

	static void SetIsTrigger(bool trigger, NodeCollider& c);
	static void SetIsSimulationCollider(bool enabled, NodeCollider& c);
	static void SetIsWorldQueryCollider(bool enabled, NodeCollider& c);

	static void SetCollisionCategoryBits(uint16_t category, NodeCollider& c);
	static void SetCollideWithMaskBits(uint16_t mask, NodeCollider& c);

private:
	void FlushCommandsImpl() override;
	void BuildTasksImpl(TaskGraph& graph) override {};
	void OnInitialize() override;
	void OnUnInitialize() override;


	// =========== Rigid Body functions ===========

	static void S_CreateRigidBody(NodeRigidBody& rigidBody);
	static void S_DestroyRigidBody(NodeRigidBody& rigidBody);

	static void S_AddCollider(NodeCollider& collider, NodeRigidBody& rigidBody);

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

	void S_AttachToRigidBody(NodeRigidBody* rigidBody, NodeCollider& c);
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

	rp3d::PhysicsCommon m_physicsCommon;
	rp3d::PhysicsWorld* m_pPhysicsWorld;
	PhysicsEvents* m_pPhysicsEvents;

	friend Server<PhysicsServer>;
};

#endif