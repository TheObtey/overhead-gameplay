#include "Nodes/NodeRigidBody.h"

#include "Servers/PhysicsServer.h"

NodeRigidBody::NodeRigidBody(std::string const& name) : Node(name)
{

}

NodeRigidBody::NodeRigidBody(std::string const& name, Node3D* owner) : Node(name)
{
	Init(owner);
}
void NodeRigidBody::Init(Node3D* owner)
{
	m_pOwner = owner;
	m_pOwner->Update(0.016);
	m_pRigidBody = PhysicsServer::CreateRigidBody(*owner, this);
}
void NodeRigidBody::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
	auto& pos = m_pRigidBody->getTransform().getPosition();
	auto& rot = m_pRigidBody->getTransform().getOrientation();
	m_pOwner->SetPosition({ pos.x, pos.y, pos.z });
	m_pOwner->SetRotation({ rot.x, rot.y, rot.z });
	m_pOwner->Update(delta);
	//GetRigidBody().setLinearVelocity({ 0,0,0 });
}

void NodeRigidBody::AddBoxCollider(Vec3 halfSizes)
{
	if (halfSizes == Vec3{0.0f, 0.0f, 0.0f})
	{
		halfSizes.x = halfSizes.y =	halfSizes.z = m_pOwner->GetMaxScale();
	}

	rp::BoxShape* box = PhysicsServer::GetPhysicsCommon().createBoxShape(halfSizes);
	AddCollider(box);
}
void NodeRigidBody::AddSphereCollider(float radius)
{
	if (radius == 0.0f)
	{
		radius = m_pOwner->GetMaxScale();
	}
	rp::SphereShape* sphere = PhysicsServer::GetPhysicsCommon().createSphereShape(radius);
	AddCollider(sphere);

}
void NodeRigidBody::AddCapsuleCollider(float radius, float height)
{
	rp::CapsuleShape* capsule = PhysicsServer::GetPhysicsCommon().createCapsuleShape(radius, height);
	AddCollider(capsule);
}
//void NodeRigidBody::AddConcaveCollider(CollisionShape shape)
//{
//
//}
//void NodeRigidBody::AddConvexCollider(CollisionShape shape)
//{
// use : reactphysics3d::PolygonVertexArray for convex mesh shape
//
//}

void NodeRigidBody::AddCollider(rp::CollisionShape* collisionShape)
{
	//m_pRigidBody->addCollider(collisionShape, *m_pOwner);
	rp::Transform t;
	t.setPosition(Vec3(0.0, 0.0, 0.0));
	t.setOrientation({ m_pOwner->GetWorldRotationQuaternion().x,m_pOwner->GetWorldRotationQuaternion().y,m_pOwner->GetWorldRotationQuaternion().z,m_pOwner->GetWorldRotationQuaternion().w });

	m_pRigidBody->addCollider(collisionShape, t);
}

