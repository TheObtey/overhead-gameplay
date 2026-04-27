class NodeCollider::Proxy : public Node3D::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node3D::Proxy(node), m_pNode(static_cast<NodeCollider*>(&node)) {}

    // =========== Local transform (offset from RigidBody) ===========

    void SetLocalPosition(const glm::vec3& pos)             { m_pNode->SetLocalPosition(pos); }
    //void SetLocalPosition(float posX,float posY, float posZ){ m_pNode->SetLocalPosition({ posX, posY, posZ }); }
    void SetLocalRotation(const glm::quat& rot)             { m_pNode->SetLocalRotation(rot); }
    glm::vec3 const& GetLocalPosition() const               { return m_pNode->GetLocalPosition(); }
    glm::quat const& GetLocalRotation() const               { return m_pNode->GetLocalRotation(); }

    // =========== Material ===========

    void  SetBounciness(float bounciness)                   { m_pNode->SetBounciness(bounciness); }
    float GetBounciness() const                             { return m_pNode->GetBounciness(); }
    void  SetFrictionCoefficient(float friction)            { m_pNode->SetFrictionCoefficient(friction); }
    float GetFrictionCoefficient() const                    { return m_pNode->GetFrictionCoefficient(); }
    void  SetMassDensity(float density)                     { m_pNode->SetMassDensity(density); }
    float GetMassDensity() const                            { return m_pNode->GetMassDensity(); }

    // =========== Collider behavior ===========

    void SetIsTrigger(bool trigger)                         { m_pNode->SetIsTrigger(trigger); }
    bool IsTrigger() const                                  { return m_pNode->IsTrigger(); }
    void SetIsSimulationCollider(bool enabled)              { m_pNode->SetIsSimulationCollider(enabled); }
    bool IsSimulationCollider() const                       { return m_pNode->IsSimulationCollider(); }
    void SetIsWorldQueryCollider(bool enabled)              { m_pNode->SetIsWorldQueryCollider(enabled); }
    bool IsWorldQueryCollider() const                       { return m_pNode->IsWorldQueryCollider(); }

    // =========== Collision filtering ===========

    void     SetCollisionCategoryBits(uint16_t category)    { m_pNode->SetCollisionCategoryBits(category); }
    uint16_t GetCollisionCategoryBits() const               { return m_pNode->GetCollisionCategoryBits(); }
    void     SetCollideWithMaskBits(uint16_t mask)          { m_pNode->SetCollideWithMaskBits(mask); }
    uint16_t GetCollisionBitsMask() const                   { return m_pNode->GetCollisionBitsMask(); }

private:
	NodeCollider* m_pNode;
};

struct NodeCollider::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindClass<NodeCollider::Proxy>("nodecollider",
			sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			//"SetLocalPosition", OVERLOAD(NodeCollider::Proxy, void, const glm::vec3&)		(BIND(SetLocalPosition)),
			"SetLocalPosition",	(BIND(SetLocalPosition)),
			"SetLocalRotation", BIND(SetLocalRotation),
			"GetLocalPosition", BIND(GetLocalPosition),
			"GetLocalRotation", BIND(GetLocalRotation),

			"SetBounciness", BIND(SetBounciness),
			"GetBounciness", BIND(GetBounciness),
			"SetFrictionCoefficient", BIND(SetFrictionCoefficient),
			"GetFrictionCoefficient", BIND(GetFrictionCoefficient),
			"SetMassDensity", BIND(SetMassDensity),
			"GetMassDensity", BIND(GetMassDensity),

			"SetIsTrigger", BIND(SetIsTrigger),
			"IsTrigger", BIND(IsTrigger),
			"SetIsSimulationCollider", BIND(SetIsSimulationCollider),
			"IsSimulationCollider", BIND(IsSimulationCollider),
			"SetIsWorldQueryCollider", BIND(SetIsWorldQueryCollider),
			"IsWorldQueryCollider", BIND(IsWorldQueryCollider),

			"SetCollisionCategoryBits", BIND(SetCollisionCategoryBits),
			"GetCollisionCategoryBits", BIND(GetCollisionCategoryBits),
			"SetCollideWithMaskBits", BIND(SetCollideWithMaskBits),
			"GetCollisionBitsMask", BIND(GetCollisionBitsMask));
	};
};


