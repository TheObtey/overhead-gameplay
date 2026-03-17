#include "Servers/PhysicsServer.h"
//#include "Nodes/NodeRigidBody.h"
#include "Nodes/NodeCollider.h"
#include "Debug.h"

reactphysics3d::PhysicsWorld* PhysicsServer::m_pPhysicsWorld = nullptr;
reactphysics3d::PhysicsCommon PhysicsServer::m_physicsCommon;


PhysicsServer::PhysicsServer() : Server()
{
}

PhysicsServer::~PhysicsServer()
{
    PhysicsServer::GetPhysicsCommon().destroyPhysicsWorld(m_pPhysicsWorld);
}

void PhysicsServer::OnInitialize()
{
    rp3d::PhysicsWorld::WorldSettings settings;
    settings.isSleepingEnabled = true;
    settings.gravity = rp3d::Vector3(0, -9.81, 0);
    m_pPhysicsWorld = m_physicsCommon.createPhysicsWorld(settings);
}

void PhysicsServer::OnUnInitialize()
{
    if (m_pPhysicsWorld == nullptr) return;
    
    PhysicsServer::GetPhysicsCommon().destroyPhysicsWorld(m_pPhysicsWorld);
	m_pPhysicsWorld = nullptr;
}

void PhysicsServer::Initialize()
{
    rp3d::PhysicsWorld::WorldSettings settings;
	settings.worldName = "MainWorld";
	settings.defaultVelocitySolverNbIterations = 6;         // Default is 6
	settings.defaultPositionSolverNbIterations = 3;         // Default is 3
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
	
    m_pPhysicsWorld = m_physicsCommon.createPhysicsWorld(settings);
    //m_pPhysicsWorld = m_physicsCommon.createPhysicsWorld();
}
rp3d::RigidBody* PhysicsServer::CreateRigidBody(const rp3d::Transform& transform, Node* const To)
{
    //Instance().m_commands.push({ CommandType::CREATERIGIDBODY, To });
	return m_pPhysicsWorld->createRigidBody(transform);
}
void PhysicsServer::DestroyRigidBody(NodeRigidBody& rigidBody)
{

}

void PhysicsServer::FlushCommandsImpl()
{
while (!m_commands.empty())
{
    Command<PhysicsServer>& command = m_commands.front();

	auto& node = static_cast<NodeRigidBody&>(*command.To);

    switch (command.Type)
    {
    case CommandTyp::CREATE_RIGID_BODY:
        CreateRigidBody(command.transform, command.To);
        break;

    case CommandTyp::DESTROY_RIGID_BODY:
		DestroyRigidBody(node);
        break;

    case CommandTyp::APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS:
        ApplyLocalForceAtCenterOfMass(command.force, node);
        break;

    case CommandTyp::APPLY_LOCAL_FORCE_AT_LOCAL_POSITION:
        ApplyLocalForceAtLocalPosition(command.force, command.position, node);
        break;

    case CommandTyp::APPLY_LOCAL_FORCE_AT_WORLD_POSITION:
        ApplyLocalForceAtWorldPosition(command.force, command.position, node);
        break;

    case CommandTyp::APPLY_WORLD_FORCE_AT_CENTER_OF_MASS:
        ApplyWorldForceAtCenterOfMass(command.force, node);
        break;

    case CommandTyp::APPLY_WORLD_FORCE_AT_LOCAL_POSITION:
        ApplyWorldForceAtLocalPosition(command.force, command.position, node);
        break;

    case CommandTyp::APPLY_WORLD_FORCE_AT_WORLD_POSITION:
        ApplyWorldForceAtWorldPosition(command.force, command.position, node);
        break;

    case CommandTyp::APPLY_LOCAL_TORQUE:
        ApplyLocalTorque(command.torque, node);
        break;

    case CommandTyp::APPLY_WORLD_TORQUE:
        ApplyWorldTorque(command.torque, node);
        break;

    case CommandTyp::SET_LINEAR_VELOCITY:
        SetLinearVelocity(command.velocity, node);
        break;

    case CommandTyp::SET_ANGULAR_VELOCITY:
        SetAngularVelocity(command.velocity, node);
        break;

    case CommandTyp::SET_LINEAR_DAMPING:
        SetLinearDamping(command.linearDamping, node);
        break;

    case CommandTyp::SET_ANGULAR_DAMPING:
        SetAngularDamping(command.angularDamping, node);
        break;

    case CommandTyp::LOCK_LINEAR_AXIS:
        LockLinearAxis(command.lockAxis, node);
        break;

    case CommandTyp::LOCK_ANGULAR_AXIS:
        LockAngularAxis(command.lockAxis, node);
        break;

    case CommandTyp::RESET_FORCES:
        ResetForces(node);
        break;

    case CommandTyp::RESET_TORQUE:
        ResetTorque(node);
        break;

    case CommandTyp::SET_MASS:
        SetMass(command.mass, node);
        break;

    case CommandTyp::SET_BODY_TYPE:
        SetBodyType(command.bodyType, node);
        break;

    case CommandTyp::SET_SLEEPING_ENABLED:
        SetSleepingEnabled(command.enabled, node);
        break;

    case CommandTyp::SET_SLEEPING_STATE:
        SetSleepingState(command.isSleeping, node);
        break;

    case CommandTyp::SET_IS_GRAVITY_ENABLED:
        SetIsGravityEnabled(command.enabled, node);
        break;

    default:
        break;
    }

    m_commands.pop();
}
}




// =========== Rigid Body intermediate function definitions ===========

void PhysicsServer::ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	Command<PhysicsServer> cmd
	{
		CommandTyp::APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS,
		.To = &rb,
		.force = force
	}
	cmd.Type = ;
	cmd.To = &rb;
	cmd.force = force;
	Instance().m_commands.push({ CommandTyp::APPLY_LOCAL_FORCE_AT_CENTER_OF_MASS, &rb, .force = force });
}
void PhysicsServer::ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void PhysicsServer::ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void PhysicsServer::ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldForceAtCenterOfMass(glmToRp3d(force));
}
void PhysicsServer::ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void PhysicsServer::ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void PhysicsServer::ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalTorque(glmToRp3d(torque));
}
void PhysicsServer::ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldTorque(glmToRp3d(torque));
}

void PhysicsServer::SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setLinearVelocity(glmToRp3d(velocity));
}
void PhysicsServer::SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setAngularVelocity(glmToRp3d(velocity));
}
/// Set the linear decelerating factor
void  PhysicsServer::SetLinearDamping(float linearDamping, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setLinearDamping(linearDamping);
}
/// Set the angular decelerating factor
void  PhysicsServer::SetAngularDamping(float angularDamping, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setAngularDamping(angularDamping);
}

void PhysicsServer::ResetForces(NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->resetForce();
}
void PhysicsServer::ResetTorque(NodeRigidBody& rb)
{

	if (rb.m_pNode3D)
		rb.m_pRigidBody->resetTorque();
}

void  PhysicsServer::SetMass(float mass, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setMass(mass);
}
void PhysicsServer::SetBodyType(RigidBodyType type, NodeRigidBody& rb)
{
	if (rb.m_pNode3D == nullptr) return;
	switch (type)
	{
	case RigidBodyType::STATIC:
		rb.m_pRigidBody->setType(rp3d::BodyType::STATIC);
		break;
	case RigidBodyType::KINEMATIC:
		rb.m_pRigidBody->setType(rp3d::BodyType::KINEMATIC);
		break;
	case RigidBodyType::DYNAMIC:
		rb.m_pRigidBody->setType(rp3d::BodyType::DYNAMIC);
		break;
	default:
		DEBUG("Invalid RigidBodyType set in SetBodyType func\n");
		break;
	}
}

void PhysicsServer::LockLinearAxis(bool lockAxis[], NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setLinearLockAxisFactor(rp3d::Vector3{ lockAxis[0] == true ? 0.0f : 1.0f, lockAxis[1] == true ? 0.0f : 1.0f, lockAxis[2] == true ? 0.0f : 1.0f });
}
void PhysicsServer::LockAngularAxis(bool lockAxis[], NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setAngularLockAxisFactor(rp3d::Vector3{ lockAxis[0] == true ? 0.0f : 1.0f, lockAxis[1] == true ? 0.0f : 1.0f, lockAxis[2] == true ? 0.0f : 1.0f });
}
void PhysicsServer::SetSleepingEnabled(bool enabled, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setIsAllowedToSleep(enabled);
}
void PhysicsServer::SetSleepingState(bool isSleeping, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setIsSleeping(isSleeping);
}
void PhysicsServer::SetIsGravityEnabled(bool enabled, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->enableGravity(enabled);
}


// =========== Collider intermediate function definitions ===========

void PhysicsServer::AttachToRigidBody(rp3d::RigidBody* rigidBody, NodeCollider& c)
{
	if (!c.m_pShape) return;
	c.m_pRigidBody = rigidBody;
	c.m_pCollider = rigidBody->addCollider(c.m_pShape, c.GetLocalRp3dTransform());
}
void PhysicsServer::Detach(NodeCollider& c)
{
}
void PhysicsServer::DestroyShape(NodeCollider& c)
{
	if (!c.m_pShape) return;

	auto& pc = PhysicsServer::GetPhysicsCommon(); // A REFAIRE POUR SUIVRE LOGIQUE SERVEUR

	if (auto* s = dynamic_cast<rp3d::BoxShape*>(c.m_pShape))
		pc.destroyBoxShape(s);
	else if (auto* s = dynamic_cast<rp3d::SphereShape*>(c.m_pShape))
		pc.destroySphereShape(s);
	else if (auto* s = dynamic_cast<rp3d::CapsuleShape*>(c.m_pShape))
		pc.destroyCapsuleShape(s);

	c.m_pShape = nullptr;
}
void PhysicsServer::SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c)
{
	Detach(c);
	DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createBoxShape({ halfExtents.x, halfExtents.y, halfExtents.z });

	if (c.m_pRigidBody)
		AttachToRigidBody(c.m_pRigidBody, c);
}

void PhysicsServer::SetSphereShape(float radius, NodeCollider& c)
{
	Detach(c);
	DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createSphereShape(radius);
	if (c.m_pRigidBody) AttachToRigidBody(c.m_pRigidBody, c);
}

void PhysicsServer::SetCapsuleShape(float radius, float height, NodeCollider& c)
{
	Detach(c);
	DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createCapsuleShape(radius, height);
	if (c.m_pRigidBody) AttachToRigidBody(c.m_pRigidBody, c);
}

void PhysicsServer::SetLocalPosition(const glm::vec3& pos, NodeCollider& c)
{
	c.m_localPosition = pos;
	if (c.m_pCollider)
		c.m_pCollider->setLocalToBodyTransform(c.GetLocalRp3dTransform());
}

void PhysicsServer::SetLocalRotation(const glm::quat& rot, NodeCollider& c)
{
	c.m_localRotation = rot;
	if (c.m_pCollider)
		c.m_pCollider->setLocalToBodyTransform(c.GetLocalRp3dTransform());
}

void  PhysicsServer::SetBounciness(float v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->getMaterial().setBounciness(v);
}
void  PhysicsServer::SetFrictionCoefficient(float v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->getMaterial().setFrictionCoefficient(v);
}
void  PhysicsServer::SetMassDensity(float v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->getMaterial().setMassDensity(v);
}
void PhysicsServer::SetIsTrigger(bool v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setIsTrigger(v);
}
void PhysicsServer::SetIsSimulationCollider(bool v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setIsSimulationCollider(v);
}
void PhysicsServer::SetIsWorldQueryCollider(bool v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setIsWorldQueryCollider(v);
}
void PhysicsServer::SetCollisionCategoryBits(uint16_t v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setCollisionCategoryBits(v);
}

void PhysicsServer::SetCollideWithMaskBits(uint16_t v, NodeCollider& c) 
{
	if (c.m_pCollider) c.m_pCollider->setCollideWithMaskBits(v);
}






// =========== Rigid Body function definitions ===========

void PhysicsServer::S_ApplyLocalForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalForceAtCenterOfMass(glmToRp3d(force));
}
void PhysicsServer::S_ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void PhysicsServer::S_ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void PhysicsServer::S_ApplyWorldForceAtCenterOfMass(const glm::vec3& force, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldForceAtCenterOfMass(glmToRp3d(force));
}
void PhysicsServer::S_ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldForceAtLocalPosition(glmToRp3d(force), glmToRp3d(point));
}
void PhysicsServer::S_ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldForceAtWorldPosition(glmToRp3d(force), glmToRp3d(point));
}

void PhysicsServer::S_ApplyLocalTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyLocalTorque(glmToRp3d(torque));
}
void PhysicsServer::S_ApplyWorldTorque(const glm::vec3& torque, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->applyWorldTorque(glmToRp3d(torque));
}

void PhysicsServer::S_SetLinearVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setLinearVelocity(glmToRp3d(velocity));
}
void PhysicsServer::S_SetAngularVelocity(const glm::vec3& velocity, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setAngularVelocity(glmToRp3d(velocity));
}
/// Set the linear decelerating factor
void  PhysicsServer::S_SetLinearDamping(float linearDamping, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setLinearDamping(linearDamping);
}
/// Set the angular decelerating factor
void  PhysicsServer::S_SetAngularDamping(float angularDamping, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setAngularDamping(angularDamping);
}

void PhysicsServer::S_ResetForces(NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->resetForce();
}
void PhysicsServer::S_ResetTorque(NodeRigidBody& rb)
{

	if (rb.m_pNode3D)
		rb.m_pRigidBody->resetTorque();
}

void  PhysicsServer::S_SetMass(float mass, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setMass(mass);
}
void PhysicsServer::S_SetBodyType(RigidBodyType type, NodeRigidBody& rb)
{
	if (rb.m_pNode3D == nullptr) return;
	switch (type)
	{
	case RigidBodyType::STATIC:
		rb.m_pRigidBody->setType(rp3d::BodyType::STATIC);
		break;
	case RigidBodyType::KINEMATIC:
		rb.m_pRigidBody->setType(rp3d::BodyType::KINEMATIC);
		break;
	case RigidBodyType::DYNAMIC:
		rb.m_pRigidBody->setType(rp3d::BodyType::DYNAMIC);
		break;
	default:
		DEBUG("Invalid RigidBodyType set in SetBodyType func\n");
		break;
	}
}

void PhysicsServer::S_LockLinearAxis(bool lockAxis[], NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setLinearLockAxisFactor(rp3d::Vector3{ lockAxis[0] == true ? 0.0f : 1.0f, lockAxis[1] == true ? 0.0f : 1.0f, lockAxis[2] == true ? 0.0f : 1.0f });
}
void PhysicsServer::S_LockAngularAxis(bool lockAxis[], NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setAngularLockAxisFactor(rp3d::Vector3{ lockAxis[0] == true ? 0.0f : 1.0f, lockAxis[1] == true ? 0.0f : 1.0f, lockAxis[2] == true ? 0.0f : 1.0f });
}
void PhysicsServer::S_SetSleepingEnabled(bool enabled, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setIsAllowedToSleep(enabled);
}
void PhysicsServer::S_SetSleepingState(bool isSleeping, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->setIsSleeping(isSleeping);
}
void PhysicsServer::S_SetIsGravityEnabled(bool enabled, NodeRigidBody& rb)
{
	if (rb.m_pNode3D)
		rb.m_pRigidBody->enableGravity(enabled);
}




// =========== Collider function definitions ===========

void PhysicsServer::S_AttachToRigidBody(rp3d::RigidBody* rigidBody, NodeCollider& c)
{
	if (!c.m_pShape) return;
	c.m_pRigidBody = rigidBody;
	c.m_pCollider = rigidBody->addCollider(c.m_pShape, c.GetLocalRp3dTransform());
}
void PhysicsServer::S_Detach(NodeCollider& c)
{
}
void PhysicsServer::S_DestroyShape(NodeCollider& c)
{
	if (!c.m_pShape) return;

	auto& pc = PhysicsServer::GetPhysicsCommon(); // A REFAIRE POUR SUIVRE LOGIQUE SERVEUR

	if (auto* s = dynamic_cast<rp3d::BoxShape*>(c.m_pShape))
		pc.destroyBoxShape(s);
	else if (auto* s = dynamic_cast<rp3d::SphereShape*>(c.m_pShape))
		pc.destroySphereShape(s);
	else if (auto* s = dynamic_cast<rp3d::CapsuleShape*>(c.m_pShape))
		pc.destroyCapsuleShape(s);

	c.m_pShape = nullptr;
}
void PhysicsServer::S_SetBoxShape(const glm::vec3& halfExtents, NodeCollider& c)
{
	Detach(c);
	DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createBoxShape({ halfExtents.x, halfExtents.y, halfExtents.z });

	if (c.m_pRigidBody)
		AttachToRigidBody(c.m_pRigidBody, c);
}

void PhysicsServer::S_SetSphereShape(float radius, NodeCollider& c)
{
	Detach(c);
	DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createSphereShape(radius);
	if (c.m_pRigidBody) AttachToRigidBody(c.m_pRigidBody, c);
}

void PhysicsServer::S_SetCapsuleShape(float radius, float height, NodeCollider& c)
{
	Detach(c);
	DestroyShape(c);
	c.m_pShape = PhysicsServer::GetPhysicsCommon().createCapsuleShape(radius, height);
	if (c.m_pRigidBody) AttachToRigidBody(c.m_pRigidBody, c);
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
	if (c.m_pCollider) c.m_pCollider->setIsTrigger(v);
}
void PhysicsServer::S_SetIsSimulationCollider(bool v, NodeCollider& c)
{
	if (c.m_pCollider) c.m_pCollider->setIsSimulationCollider(v);
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