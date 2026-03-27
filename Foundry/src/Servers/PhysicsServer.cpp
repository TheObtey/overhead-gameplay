#include "Servers/PhysicsServer.h"
//#include "Nodes/NodeRigidBody.h"
#include "Nodes/NodeCollider.h"
#include "Debug.h"

//reactphysics3d::PhysicsWorld* PhysicsServer::m_pPhysicsWorld = nullptr;
//reactphysics3d::PhysicsCommon PhysicsServer::m_physicsCommon;


PhysicsServer::PhysicsServer() : Server()
{
}

PhysicsServer::~PhysicsServer()
{
}

void PhysicsServer::OnInitialize()
{
}

void PhysicsServer::OnUnInitialize()
{
}

void PhysicsServer::Initialize() // ajouter tous les params ?
{
	rp3d::PhysicsWorld::WorldSettings settings;
	settings.worldName = "MainWorld";
	settings.defaultVelocitySolverNbIterations = 60;         // Default is 6
	settings.defaultPositionSolverNbIterations = 30;         // Default is 3
	settings.isSleepingEnabled = true;                      // Default is true
	settings.gravity = rp3d::Vector3(0, -9.81, 0);          // Default is(0, -9.81, 0)
	settings.defaultTimeBeforeSleep = 1.0f;                 // Default is 1.0f
	settings.cosAngleSimilarContactManifold = 0.95;         // Default is 0.95
	settings.defaultSleepLinearVelocity = 0.02;             // Default is 0.02
	settings.defaultSleepAngularVelocity = 3.0 * pi / 180;  // Default is 3.0 * (PI / 180.0)
	settings.restitutionVelocityThreshold = 0.5;            // Default is 0.5
	settings.defaultFrictionCoefficient = 0.3;              // Default is 0.3
	settings.defaultBounciness = 0.5f;                      // Default is 0.5f
	settings.persistentContactDistanceThreshold = 0.03;     // Default is 0.03

	Instance().m_pPhysicsWorld = Instance().m_physicsCommon.createPhysicsWorld(settings);
}
void PhysicsServer::CreateRigidBody(NodeRigidBody& rigidBody)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::CREATE_RIGID_BODY;
	cmd.To = &rigidBody;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::DestroyRigidBody(NodeRigidBody& rigidBody)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::DESTROY_RIGID_BODY;
	cmd.To = &rigidBody;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::S_CreateRigidBody(NodeRigidBody& rigidBody)
{
	rigidBody.m_pRigidBodyRP3D = Instance().m_pPhysicsWorld->createRigidBody(rigidBody);

	rigidBody.m_pRigidBodyRP3D->setType(rp3d::BodyType::STATIC);
	rigidBody.m_pRigidBodyRP3D->enableGravity(true);
	rigidBody.m_pRigidBodyRP3D->setLinearDamping(0.5f);
	rigidBody.m_pRigidBodyRP3D->setAngularDamping(0.5f);
	rigidBody.m_pRigidBodyRP3D->setMass(1.0f);
	rigidBody.m_pRigidBodyRP3D->setIsAllowedToSleep(true);
	rigidBody.m_pRigidBodyRP3D->setUserData(&rigidBody.m_pProxy);
	rigidBody.m_rigidBodyCreated = true;
}
void PhysicsServer::S_DestroyRigidBody(NodeRigidBody& rigidBody)
{
	//Instance().m_pPhysicsWorld->destroyRigidBody(rigidBody.m_pRigidBodyRP3D);
}

void PhysicsServer::FlushCommandsImpl()
{
	while (!m_commands.empty())
	{
		Command<PhysicsServer>& command = m_commands.front();

		//auto& node = static_cast<NodeRigidBody&>(*command.To);

		switch (command.Type)
		{
			// =========== Rigid Body ===========

		case CommandTyp::CREATE_RIGID_BODY:
			//S_CreateRigidBody(*command.transform, command.To);
			S_CreateRigidBody(static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::DESTROY_RIGID_BODY:
			S_DestroyRigidBody(static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::ADD_COLLIDER:
			S_AddCollider(static_cast<NodeCollider&>(*command.collider), static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS:
			S_ApplyLocalForceAtCenterOfMass(command.force, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_LOCAL_FORCE_AT_LOCAL_POSITION:
			S_ApplyLocalForceAtLocalPosition(command.force, command.position, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_LOCAL_FORCE_AT_WORLD_POSITION:
			S_ApplyLocalForceAtWorldPosition(command.force, command.position, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_WORLD_FORCE_AT_CENTER_OF_MASS:
			S_ApplyWorldForceAtCenterOfMass(command.force, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_WORLD_FORCE_AT_LOCAL_POSITION:
			S_ApplyWorldForceAtLocalPosition(command.force, command.position, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_WORLD_FORCE_AT_WORLD_POSITION:
			S_ApplyWorldForceAtWorldPosition(command.force, command.position, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_LOCAL_TORQUE:
			S_ApplyLocalTorque(command.torque, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::APPLY_WORLD_TORQUE:
			S_ApplyWorldTorque(command.torque, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_LINEAR_VELOCITY:
			S_SetLinearVelocity(command.velocity, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_ANGULAR_VELOCITY:
			S_SetAngularVelocity(command.velocity, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_LINEAR_DAMPING:
			S_SetLinearDamping(command.linearDamping, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_ANGULAR_DAMPING:
			S_SetAngularDamping(command.angularDamping, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::LOCK_LINEAR_AXIS:
			S_LockLinearAxis(command.lockAxis, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::LOCK_ANGULAR_AXIS:
			S_LockAngularAxis(command.lockAxis, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::RESET_FORCES:
			S_ResetForces(static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::RESET_TORQUE:
			S_ResetTorque(static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_MASS:
			S_SetMass(command.mass, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_BODY_TYPE:
			S_SetBodyType(command.bodyType, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_SLEEPING_ENABLED:
			S_SetSleepingEnabled(command.sleepingEnabled, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_SLEEPING_STATE:
			S_SetSleepingState(command.isSleeping, static_cast<NodeRigidBody&>(*command.To));
			break;
		case CommandTyp::SET_IS_GRAVITY_ENABLED:
			S_SetIsGravityEnabled(command.gravityEnabled, static_cast<NodeRigidBody&>(*command.To));
			break;


			// ========= Collider ===========

		case CommandTyp::ATTACH_TO_RIGID_BODY:
			S_AttachToRigidBody(command.rigidBody, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::DETACH:
			S_Detach(static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::DESTROY_SHAPE:
			S_DestroyShape(static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_BOX_SHAPE:
			S_SetBoxShape(command.halfExtents, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_SPHERE_SHAPE:
			S_SetSphereShape(command.radius, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_CAPSULE_SHAPE:
			S_SetCapsuleShape(command.radius, command.height, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_LOCAL_POSITION:
			S_SetLocalPosition(command.position, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_LOCAL_ROTATION:
			S_SetLocalRotation(command.rotation, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_BOUNCINESS:
			S_SetBounciness(command.bounciness, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_FRICTION_COEFFICIENT:
			S_SetFrictionCoefficient(command.friction, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_MASS_DENSITY:
			S_SetMassDensity(command.density, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_IS_TRIGGER:
			S_SetIsTrigger(command.trigger, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_IS_SIMULATION_COLLIDER:
			S_SetIsSimulationCollider(command.isSimulationCollider, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_IS_WORLD_QUERY_COLLIDER:
			S_SetIsWorldQueryCollider(command.isWorldQueryCollider, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_COLLISION_CATEGORY_BITS:
			S_SetCollisionCategoryBits(command.category, static_cast<NodeCollider&>(*command.To));
			break;
		case CommandTyp::SET_COLLIDE_WITH_MASK_BITS:
			S_SetCollideWithMaskBits(command.mask, static_cast<NodeCollider&>(*command.To));
			break;
		default:
			break;
		}

		m_commands.pop();
	}
}




// =========== Rigid Body intermediate function definitions ===========

void PhysicsServer::AddCollider(NodeCollider& collider, NodeRigidBody& rigidBody)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::ADD_COLLIDER;
	cmd.To = &rigidBody;
	cmd.collider = &collider;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS;
	cmd.To = &rb;
	cmd.force = force;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_LOCAL_FORCE_AT_LOCAL_POSITION;
	cmd.To = &rb;
	cmd.force = force;
	cmd.position = point;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_LOCAL_FORCE_AT_WORLD_POSITION;
	cmd.To = &rb;
	cmd.force = force;
	cmd.position = point;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_WORLD_FORCE_AT_CENTER_OF_MASS;
	cmd.To = &rb;
	cmd.force = force;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_WORLD_FORCE_AT_LOCAL_POSITION;
	cmd.To = &rb;
	cmd.force = force;
	cmd.position = point;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_WORLD_FORCE_AT_WORLD_POSITION;
	cmd.To = &rb;
	cmd.force = force;
	cmd.position = point;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_LOCAL_TORQUE;
	cmd.To = &rb;
	cmd.torque = torque;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::APPLY_WORLD_TORQUE;
	cmd.To = &rb;
	cmd.torque = torque;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_LINEAR_VELOCITY;
	cmd.To = &rb;
	cmd.velocity = velocity;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_ANGULAR_VELOCITY;
	cmd.To = &rb;
	cmd.velocity = velocity;
	Instance().m_commands.push(cmd);
}
/// Set the linear decelerating factor
void  PhysicsServer::SetLinearDamping(float linearDamping, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_LINEAR_DAMPING;
	cmd.To = &rb;
	cmd.linearDamping = linearDamping;
	Instance().m_commands.push(cmd);
}
/// Set the angular decelerating factor
void  PhysicsServer::SetAngularDamping(float angularDamping, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_ANGULAR_DAMPING;
	cmd.To = &rb;
	cmd.angularDamping = angularDamping;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::ResetForces(NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::RESET_FORCES;
	cmd.To = &rb;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::ResetTorque(NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::RESET_TORQUE;
	cmd.To = &rb;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetMass(float mass, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_MASS;
	cmd.To = &rb;
	cmd.mass = mass;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetBodyType(RigidBodyType type, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_BODY_TYPE;
	cmd.To = &rb;
	cmd.bodyType = type;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::LockLinearAxis(bool lockAxis[], NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::LOCK_LINEAR_AXIS;
	cmd.To = &rb;
	cmd.lockAxis[0] = lockAxis[0];
	cmd.lockAxis[1] = lockAxis[1];
	cmd.lockAxis[2] = lockAxis[2];
	Instance().m_commands.push(cmd);
}
void PhysicsServer::LockAngularAxis(bool lockAxis[], NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::LOCK_ANGULAR_AXIS;
	cmd.To = &rb;
	cmd.lockAxis[0] = lockAxis[0];
	cmd.lockAxis[1] = lockAxis[1];
	cmd.lockAxis[2] = lockAxis[2];
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetSleepingEnabled(bool enabled, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_SLEEPING_ENABLED;
	cmd.To = &rb;
	cmd.sleepingEnabled = enabled;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetSleepingState(bool isSleeping, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_SLEEPING_STATE;
	cmd.To = &rb;
	cmd.isSleeping = isSleeping;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetIsGravityEnabled(bool enabled, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_IS_GRAVITY_ENABLED;
	cmd.To = &rb;
	cmd.gravityEnabled = enabled;
	Instance().m_commands.push(cmd);
}


// =========== Collider intermediate function definitions ===========

void PhysicsServer::AttachToRigidBody(NodeRigidBody* _rigidBody, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::ATTACH_TO_RIGID_BODY;
	cmd.To = &c;
	cmd.rigidBody = _rigidBody;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::Detach(NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::DETACH;
	cmd.To = &c;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::DestroyShape(NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::DESTROY_SHAPE;
	cmd.To = &c;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_BOX_SHAPE;
	cmd.To = &c;
	cmd.halfExtents = halfExtents;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetSphereShape(float radius, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_SPHERE_SHAPE;
	cmd.To = &c;
	cmd.radius = radius;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetCapsuleShape(float radius, float height, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_CAPSULE_SHAPE;
	cmd.To = &c;
	cmd.radius = radius;
	cmd.height = height;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetLocalPosition(const glm::vec3& pos, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_LOCAL_POSITION;
	cmd.To = &c;
	cmd.pos = pos;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetLocalRotation(const glm::quat& rot, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_LOCAL_ROTATION;
	cmd.To = &c;
	cmd.rotation = rot;
	Instance().m_commands.push(cmd);
}

void  PhysicsServer::SetBounciness(float bounciness, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_BOUNCINESS;
	cmd.To = &c;
	cmd.bounciness = bounciness;
	Instance().m_commands.push(cmd);
}
void  PhysicsServer::SetFrictionCoefficient(float coef, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_FRICTION_COEFFICIENT;
	cmd.To = &c;
	cmd.friction = coef;
	Instance().m_commands.push(cmd);
}
void  PhysicsServer::SetMassDensity(float mass, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_MASS_DENSITY;
	cmd.To = &c;
	cmd.friction = mass;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetIsTrigger(bool value, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_IS_TRIGGER;
	cmd.To = &c;
	cmd.trigger = value;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetIsSimulationCollider(bool value, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_IS_SIMULATION_COLLIDER;
	cmd.To = &c;
	cmd.isSimulationCollider = value;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetIsWorldQueryCollider(bool value, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_IS_WORLD_QUERY_COLLIDER;
	cmd.To = &c;
	cmd.isWorldQueryCollider = value;
	Instance().m_commands.push(cmd);
}
void PhysicsServer::SetCollisionCategoryBits(uint16_t value, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_COLLISION_CATEGORY_BITS;
	cmd.To = &c;
	cmd.category = value;
	Instance().m_commands.push(cmd);
}

void PhysicsServer::SetCollideWithMaskBits(uint16_t mask, NodeCollider& c)
{
	Command<PhysicsServer> cmd;
	cmd.Type = CommandTyp::SET_COLLIDE_WITH_MASK_BITS;
	cmd.To = &c;
	cmd.mask = mask;
	Instance().m_commands.push(cmd);
}






// =========== Rigid Body function definitions ===========

void PhysicsServer::S_AddCollider(NodeCollider& collider, NodeRigidBody& rigidBody)
{
	if (!collider.m_pShape) return;
	collider.m_pRigidBodyRP3D = rigidBody.m_pRigidBodyRP3D;
	collider.m_pNodeRigidBody = &rigidBody;
	collider.m_indexInRigidBody = rigidBody.m_colliders.size();
	collider.m_pCollider = rigidBody.m_pRigidBodyRP3D->addCollider(collider.m_pShape, collider.GetLocalRp3dTransform());
	rigidBody.m_colliders.push_back(&collider);
}

void PhysicsServer::S_ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyLocalForceAtCenterOfMass(glmToRp3d(force));
}
void PhysicsServer::S_ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyLocalForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void PhysicsServer::S_ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyLocalForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void PhysicsServer::S_ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyWorldForceAtCenterOfMass(glmToRp3d(force));
}
void PhysicsServer::S_ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyWorldForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void PhysicsServer::S_ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyWorldForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void PhysicsServer::S_ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyLocalTorque(glmToRp3d(torque));
}
void PhysicsServer::S_ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->applyWorldTorque(glmToRp3d(torque));
}

void PhysicsServer::S_SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setLinearVelocity(glmToRp3d(velocity));
}
void PhysicsServer::S_SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setAngularVelocity(glmToRp3d(velocity));
}
/// Set the linear decelerating factor
void  PhysicsServer::S_SetLinearDamping(float linearDamping, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setLinearDamping(linearDamping);
}
/// Set the angular decelerating factor
void  PhysicsServer::S_SetAngularDamping(float angularDamping, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setAngularDamping(angularDamping);
}

void PhysicsServer::S_ResetForces(NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->resetForce();
}
void PhysicsServer::S_ResetTorque(NodeRigidBody& rb)
{

	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->resetTorque();
}

void  PhysicsServer::S_SetMass(float mass, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setMass(mass);
}
void PhysicsServer::S_SetBodyType(RigidBodyType type, NodeRigidBody& rb)
{
	if (rb.m_pOwner == nullptr) return;
	switch (type)
	{
	case RigidBodyType::STATIC:
		rb.m_pRigidBodyRP3D->setType(rp3d::BodyType::STATIC);
		break;
	case RigidBodyType::KINEMATIC:
		rb.m_pRigidBodyRP3D->setType(rp3d::BodyType::KINEMATIC);
		break;
	case RigidBodyType::DYNAMIC:
		rb.m_pRigidBodyRP3D->setType(rp3d::BodyType::DYNAMIC);
		break;
	default:
		DEBUG("Invalid RigidBodyType set in SetBodyType func\n");
		break;
	}
}

void PhysicsServer::S_LockLinearAxis(bool lockAxis[], NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setLinearLockAxisFactor(rp3d::Vector3{ lockAxis[0] == true ? 0.0f : 1.0f, lockAxis[1] == true ? 0.0f : 1.0f, lockAxis[2] == true ? 0.0f : 1.0f });
}
void PhysicsServer::S_LockAngularAxis(bool lockAxis[], NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setAngularLockAxisFactor(rp3d::Vector3{ lockAxis[0] == true ? 0.0f : 1.0f, lockAxis[1] == true ? 0.0f : 1.0f, lockAxis[2] == true ? 0.0f : 1.0f });
}
void PhysicsServer::S_SetSleepingEnabled(bool enabled, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setIsAllowedToSleep(enabled);
}
void PhysicsServer::S_SetSleepingState(bool isSleeping, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->setIsSleeping(isSleeping);
}
void PhysicsServer::S_SetIsGravityEnabled(bool enabled, NodeRigidBody& rb)
{
	if (rb.m_pOwner)
		rb.m_pRigidBodyRP3D->enableGravity(enabled);
}




// =========== Collider function definitions ===========

void PhysicsServer::S_AttachToRigidBody(NodeRigidBody* rigidBody, NodeCollider& c)
{
	if (!c.m_pShape) return;

	auto rb = rigidBody->GetRigidBody();
	c.m_pCollider = rb->addCollider(c.m_pShape, c.GetLocalRp3dTransform());
	c.m_pRigidBodyRP3D = rb;
	c.m_pNodeRigidBody = rigidBody;
	c.m_indexInRigidBody = rigidBody->m_colliders.size();
	rigidBody->m_colliders.push_back(&c);

}
void PhysicsServer::S_Detach(NodeCollider& c)
{
	if (c.m_pNodeRigidBody == nullptr) return;

	auto& colliders = c.m_pNodeRigidBody->m_colliders;
	colliders[c.m_indexInRigidBody] = nullptr;	
	colliders.erase(colliders.begin() + c.m_indexInRigidBody);

	c.m_pNodeRigidBody->m_pRigidBodyRP3D->removeCollider(c.m_pCollider);
	c.m_indexInRigidBody = -1;


	for (int i = 0; i < colliders.size(); i++)
	{
		colliders[i]->m_indexInRigidBody = i;
	}
}

void PhysicsServer::S_DestroyShape(NodeCollider& c)
{
	if (!c.m_pShape) return;

	c.DestroyShape();

	c.m_pShape = nullptr;
}
void PhysicsServer::S_SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c)
{
	S_Detach(c);
	S_DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createBoxShape({ halfExtents.x, halfExtents.y, halfExtents.z });

	if (c.m_pRigidBodyRP3D)
		S_AttachToRigidBody(c.m_pNodeRigidBody, c);
}

void PhysicsServer::S_SetSphereShape(float radius, NodeCollider& c)
{
	S_Detach(c);
	S_DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createSphereShape(radius);
	if (c.m_pRigidBodyRP3D)
		S_AttachToRigidBody(c.m_pNodeRigidBody, c);
}

void PhysicsServer::S_SetCapsuleShape(float radius, float height, NodeCollider& c)
{
	S_Detach(c);
	S_DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createCapsuleShape(radius, height);
	if (c.m_pRigidBodyRP3D)
		S_AttachToRigidBody(c.m_pNodeRigidBody, c);
}

void PhysicsServer::S_SetLocalPosition(const glm::vec3& pos, NodeCollider& c)
{
	c.m_localPosition = pos;
	if (c.m_pCollider)
		c.m_pCollider->setLocalToBodyTransform(c.GetLocalRp3dTransform());
}

void PhysicsServer::S_SetLocalRotation(const glm::quat& rot, NodeCollider& c)
{
	c.m_localRotation = rot;
	if (c.m_pCollider)
		c.m_pCollider->setLocalToBodyTransform(c.GetLocalRp3dTransform());
}

void  PhysicsServer::S_SetBounciness(float v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->getMaterial().setBounciness(v);
}
void  PhysicsServer::S_SetFrictionCoefficient(float v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->getMaterial().setFrictionCoefficient(v);
}
void  PhysicsServer::S_SetMassDensity(float v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->getMaterial().setMassDensity(v);
}
void PhysicsServer::S_SetIsTrigger(bool v, NodeCollider& c)
{
	if (!c.m_pCollider) return;
	
	c.m_pCollider->setIsTrigger(v); c.m_pCollider->setIsSimulationCollider(!v);
}
void PhysicsServer::S_SetIsSimulationCollider(bool v, NodeCollider& c)
{
	if (!c.m_pCollider) return;
	c.m_pCollider->setIsSimulationCollider(v); c.m_pCollider->setIsTrigger(!v);
}
void PhysicsServer::S_SetIsWorldQueryCollider(bool v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setIsWorldQueryCollider(v);
}
void PhysicsServer::S_SetCollisionCategoryBits(uint16_t v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setCollisionCategoryBits(v);
}

void PhysicsServer::S_SetCollideWithMaskBits(uint16_t v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setCollideWithMaskBits(v);
}