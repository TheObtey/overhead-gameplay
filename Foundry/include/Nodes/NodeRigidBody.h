#ifndef FOUNDRY_NODERIGIDBODY__H_
#define FOUNDRY_NODERIGIDBODY__H_

#include "Node.h"
#include "Nodes/Node3D.h"

#include <reactphysics3d/reactphysics3d.h>

namespace rp = reactphysics3d;

enum class CollisionShape { SPHERE, CAPSULE, CONVEX_POLYHEDRON, CONCAVE_SHAPE };
enum class CollisionShapeName { TRIANGLE, SPHERE, CAPSULE, BOX, CONVEX_MESH, TRIANGLE_MESH, HEIGHTFIELD };

struct Vec3 : public rp::Vector3
{
	Vec3(float x, float y, float z) : rp::Vector3(x,y,z)
	{ }
	operator glm::vec3()
	{
		return glm::vec3(x, y, z);
	}
};

class NodeRigidBody : public Node
{
public:
	NodeRigidBody(std::string const& name);
	NodeRigidBody(std::string const& name, Node3D* owner);
	~NodeRigidBody() override = default;

	void Init(Node3D* owner);
	virtual void OnUpdate(double delta) override;
	void AddBoxCollider(Vec3 halfSizes = { 0.f,0.f,0.f });
	void AddSphereCollider(float radius = 0.0f);
	void AddCapsuleCollider(float radius, float height);
	//void AddConcaveCollider(CollisionShape shape);
	//void AddConvexCollider(CollisionShape shape);

	rp::RigidBody& GetRigidBody() { return *m_pRigidBody; }

protected:

private:

	void AddCollider(rp::CollisionShape* collisionShape);

	Node3D* m_pOwner;
	rp::RigidBody* m_pRigidBody;
};

#endif // -FOUNDRY_NODERIGIDBODY__H_