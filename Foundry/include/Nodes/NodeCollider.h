#ifndef FOUNDRY_NODECOLLIDER__H_
#define FOUNDRY_NODECOLLIDER__H_

#include "Node.h"
#include "NodeRigidBody.h"

class NodeCollider : public Node
{
public:
    ////////////////////////////////////////////////////////////
    // Engine

    class Proxy;

    NodeCollider(std::string const& name);
    ~NodeCollider() override;

    void init();

    void AttachToRigidBody(rp3d::RigidBody* rigidBody);
    void Detach();
    bool IsAttached() const { return m_pCollider != nullptr; }
    
    //rp3d::Collider* GetCollider() const { return m_pCollider; }

    // Engine
    ////////////////////////////////////////////////////////////



    // =========== Shapes ===========

    void SetBoxShape(const glm::vec3& halfExtents);
    void SetSphereShape(float radius);
    void SetCapsuleShape(float radius, float height);

    // =========== Local transform (offset from RigidBody) ===========

    void SetLocalPosition(const glm::vec3& pos);
    void SetLocalRotation(const glm::quat& rot);
    glm::vec3 const& GetLocalPosition() const { return m_localPosition; }
    glm::quat const& GetLocalRotation() const { return m_localRotation; }

    // =========== Material ===========

    void  SetBounciness(float bounciness);
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


private:
    void              DestroyShape();
    rp3d::Transform   GetLocalRp3dTransform() const;

    rp3d::Collider* m_pCollider = nullptr;
    rp3d::CollisionShape* m_pShape = nullptr;   // A GARDER PLUTOT DANS LE SERV POUR UNE SUELE INSTANCE DE CHAQUE SHAPE ?
    rp3d::RigidBody* m_pRigidBody = nullptr;

    glm::vec3 m_localPosition{ 0.0f, 0.0f, 0.0f };
    glm::quat m_localRotation{ 1.0f, 0.0f, 0.0f, 0.0f };

	friend class PhysicsServer;
};


REGISTER_ISERIALIZABLE(NodeCollider, NodeCollider::CreateInstance);

#include "Scripting/Proxies/NodeColliderProxy.inl"

#endif // !NODECOLLIDER__H_