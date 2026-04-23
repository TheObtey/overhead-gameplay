class NodeRigidBody::Proxy : public Node3D::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node3D::Proxy(node), m_pNode(static_cast<NodeRigidBody*>(&node)) {}

	static Proxy* CreateNodeRigidBodyProxy(std::string const& name);

	// =========== Forces and Torques ===========

	void ApplyLocalForceAtCenterOfMass(const glm::vec3& force)								{ m_pNode->ApplyLocalForceAtCenterOfMass(force); }
	void ApplyLocalForceAtCenterOfMass(float forceX, float forceY, float forceZ){ m_pNode->ApplyLocalForceAtCenterOfMass({forceX,forceY,forceZ}); }

	void ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point) { m_pNode->ApplyLocalForceAtLocalPosition(force, point); }
	void ApplyLocalForceAtLocalPosition(float forceX, float forceY, float forceZ, float pointX, float pointY, float pointZ) { m_pNode->ApplyLocalForceAtLocalPosition({ forceX, forceY, forceZ }, { pointX, pointY, pointZ }); }

	void ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyLocalForceAtWorldPosition(force, point); }
	void ApplyLocalForceAtWorldPosition(float forceX, float forceY, float forceZ, float pointX, float pointY, float pointZ) { m_pNode->ApplyLocalForceAtWorldPosition({ forceX, forceY, forceZ }, { pointX, pointY, pointZ }); }

	void ApplyWorldForceAtCenterOfMass(const glm::vec3& force)								{ m_pNode->ApplyWorldForceAtCenterOfMass(force); }
	void ApplyWorldForceAtCenterOfMass(float forceX, float forceY, float forceZ) { m_pNode->ApplyWorldForceAtCenterOfMass({ forceX,forceY,forceZ }); }

	void ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyWorldForceAtLocalPosition(force, point); }
	void ApplyWorldForceAtLocalPosition(float forceX, float forceY, float forceZ, float pointX, float pointY, float pointZ) { m_pNode->ApplyWorldForceAtLocalPosition({ forceX, forceY, forceZ }, { pointX, pointY, pointZ }); }

	void ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyWorldForceAtWorldPosition(force, point); }
	void ApplyWorldForceAtWorldPosition(float forceX, float forceY, float forceZ, float pointX, float pointY, float pointZ) { m_pNode->ApplyWorldForceAtWorldPosition({ forceX, forceY, forceZ }, { pointX, pointY, pointZ }); }

	void ApplyLocalTorque(const glm::vec3& torque)											{ m_pNode->ApplyLocalTorque(torque); }
	void ApplyLocalTorque(float torqueX, float torqueY, float torqueZ)						{ m_pNode->ApplyLocalTorque({ torqueX, torqueY, torqueZ }); }
	void ApplyWorldTorque(const glm::vec3& torque)											{ m_pNode->ApplyWorldTorque(torque); }
	void ApplyWorldTorque(float torqueX, float torqueY, float torqueZ)						{ m_pNode->ApplyWorldTorque({ torqueX, torqueY, torqueZ }); }

	glm::vec3 const GetLinearVelocity() const												{ return m_pNode->GetLinearVelocity(); }
	glm::vec3 const GetAngularVelocity() const												{ return m_pNode->GetAngularVelocity(); }
	/// Return the linear decelerating factor												
	float const		GetLinearDamping() const												{ return m_pNode->GetLinearDamping(); }
	/// Return the angular velocity damping factor											
	float const		GetAngularDamping() const												{ return m_pNode->GetAngularDamping(); }
	glm::vec3 const GetTotalForce() const													{ return m_pNode->GetTotalForce(); }


	void SetLinearVelocity(const glm::vec3& velocity)										{ m_pNode->SetLinearVelocity(velocity); }
	void SetLinearVelocity(float velocityX, float velocityY, float velocityZ)				{ m_pNode->SetLinearVelocity({ velocityX, velocityY, velocityZ }); }
	void SetAngularVelocity(const glm::vec3& velocity)										{ m_pNode->SetAngularVelocity(velocity); }
	void SetAngularVelocity(float velocityX, float velocityY, float velocityZ)				{ m_pNode->SetAngularVelocity({ velocityX, velocityY, velocityZ }); }
	/// Set the linear decelerating factor													
	void SetLinearDamping(float linearDamping)												{ m_pNode->SetLinearDamping(linearDamping); }
	/// Set the angular decelerating factor													
	void SetAngularDamping(float angularDamping)											{ m_pNode->SetAngularDamping(angularDamping); }

	void LockLinearAxis(bool x, bool y, bool z)												{ m_pNode->LockLinearAxis(x, y, z); }
	void LockAngularAxis(bool x, bool y, bool z)											{ m_pNode->LockAngularAxis(x, y, z); }

	void ResetForces()																		{ m_pNode->ResetForces(); }
	void ResetTorque()																		{ m_pNode->ResetTorque(); }

	// =========== Mass and Body Type ===========

	float GetMass() const																	{ return m_pNode->GetMass(); }
	void  SetMass(float mass)																{ m_pNode->SetMass(mass); }

	RigidBodyType GetBodyType() const														{ return m_pNode->GetBodyType(); }
	void          SetBodyType(RigidBodyType type)											{ m_pNode->SetBodyType(type); }

	// =========== Material ===========

	// global
	void  SetBounciness(float bounciness)													{ m_pNode->SetBounciness(bounciness); }
	float GetBounciness() const																{ return m_pNode->GetBounciness(); }
	void  SetFrictionCoefficient(float friction)											{ m_pNode->SetFrictionCoefficient(friction); }
	float GetFrictionCoefficient() const													{ return m_pNode->GetFrictionCoefficient(); }
	void  SetMassDensity(float density)														{ m_pNode->SetMassDensity(density); }
	float GetMassDensity() const															{ return m_pNode->GetMassDensity(); }

	// specific
	void  SetBounciness(int colliderIndex, float bounciness)								{ m_pNode->SetBounciness(colliderIndex, bounciness); }
	float GetBounciness(int colliderIndex) const											{ return m_pNode->GetBounciness(colliderIndex); }
	void  SetFrictionCoefficient(int colliderIndex, float friction)							{ m_pNode->SetFrictionCoefficient(colliderIndex, friction); }
	float GetFrictionCoefficient(int colliderIndex) const									{ return m_pNode->GetFrictionCoefficient(colliderIndex); }
	void  SetMassDensity(int colliderIndex, float density)									{ m_pNode->SetMassDensity(colliderIndex, density); }
	float GetMassDensity(int colliderIndex) const											{ return m_pNode->GetMassDensity(colliderIndex); }

	// =========== Sleeping and Gravity ===========

	bool IsAllowedToSleep() const															{ return m_pNode->IsAllowedToSleep(); }
	bool IsSleeping() const																	{ return m_pNode->IsSleeping(); }
	bool IsGravityEnabled()																	{ return m_pNode->IsGravityEnabled(); }

	void SetSleepingEnabled(bool enabled)													{ m_pNode->SetSleepingEnabled(enabled); }
	void SetSleepingState(bool isSleeping)													{ m_pNode->SetSleepingState(isSleeping); }
	void SetIsGravityEnabled(bool enabled)													{ m_pNode->SetIsGravityEnabled(enabled); }

	Node3D::Proxy* GetNode3DParent()														{ return static_cast<Node3D::Proxy*>(&m_pNode->GetNode3DParent()->GetNodeProxy()); };

private:
	NodeRigidBody* m_pNode;
};


struct NodeRigidBody::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNodeRigidBody", &NodeRigidBody::Proxy::CreateNodeRigidBodyProxy);
		binder.BindClass<NodeRigidBody::Proxy>("noderigidbody",
			sol::base_classes, sol::bases<Node::Proxy, Node3D::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"ApplyLocalForceAtCenterOfMass", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&)		(BIND(ApplyLocalForceAtCenterOfMass)),
			"ApplyLocalForceAtCenterOfMass", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float)		(BIND(ApplyLocalForceAtCenterOfMass)),
			"ApplyLocalForceAtLocalPosition", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&, const glm::vec3&)		(BIND(ApplyLocalForceAtLocalPosition)),
			"ApplyLocalForceAtLocalPosition", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float, float, float, float)		(BIND(ApplyLocalForceAtLocalPosition)),
			"ApplyLocalForceAtWorldPosition", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&, const glm::vec3&)		(BIND(ApplyLocalForceAtWorldPosition)),
			"ApplyLocalForceAtWorldPosition", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float, float, float, float)		(BIND(ApplyLocalForceAtWorldPosition)),
			"ApplyWorldForceAtCenterOfMass", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&)		(BIND(ApplyWorldForceAtCenterOfMass)),
			"ApplyWorldForceAtCenterOfMass", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float)		(BIND(ApplyWorldForceAtCenterOfMass)),
			"ApplyWorldForceAtLocalPosition", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&, const glm::vec3&)		(BIND(ApplyWorldForceAtLocalPosition)),
			"ApplyWorldForceAtLocalPosition", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float, float, float, float)		(BIND(ApplyWorldForceAtLocalPosition)),
			"ApplyWorldForceAtWorldPosition", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&, const glm::vec3&)		(BIND(ApplyWorldForceAtWorldPosition)),
			"ApplyWorldForceAtWorldPosition", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float, float, float, float)		(BIND(ApplyWorldForceAtWorldPosition)),
			"ApplyLocalTorque", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&)		(BIND(ApplyLocalTorque)),
			"ApplyLocalTorque", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float)		(BIND(ApplyLocalTorque)),
			"ApplyWorldTorque", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&)		(BIND(ApplyWorldTorque)),
			"ApplyWorldTorque", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float)		(BIND(ApplyWorldTorque)),

			"GetLinearVelocity", BIND(GetLinearVelocity),
			"GetAngularVelocity", BIND(GetAngularVelocity),
			"GetLinearDamping", BIND(GetLinearDamping),
			"GetAngularDamping", BIND(GetAngularDamping),
			"GetTotalForce", BIND(GetTotalForce),

			"SetLinearVelocity", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&)		(BIND(SetLinearVelocity)),
			"SetLinearVelocity", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float)		(BIND(SetLinearVelocity)),
			"SetAngularVelocity", OVERLOAD(NodeRigidBody::Proxy, void, const glm::vec3&)		(BIND(SetAngularVelocity)),
			"SetAngularVelocity", OVERLOAD(NodeRigidBody::Proxy, void, float, float, float)		(BIND(SetAngularVelocity)),
			"SetLinearDamping", BIND(SetLinearDamping),
			"SetAngularDamping", BIND(SetAngularDamping),
			"LockLinearAxis", BIND(LockLinearAxis),
			"LockAngularAxis", BIND(LockAngularAxis),
			"ResetForces", BIND(ResetForces),
			"ResetTorque", BIND(ResetTorque),

			"GetMass", BIND(GetMass),
			"SetMass", BIND(SetMass),
			"GetBodyType", BIND(GetBodyType),
			"SetBodyType", BIND(SetBodyType),

			"SetBounciness", OVERLOAD(NodeRigidBody::Proxy, void, float)		(BIND(SetBounciness)),
			"GetBounciness", OVERLOAD_CONST(NodeRigidBody::Proxy, float)			(BIND(GetBounciness)),
			"SetFrictionCoefficient", OVERLOAD(NodeRigidBody::Proxy, void, float)		(BIND(SetFrictionCoefficient)),
			"GetFrictionCoefficient", OVERLOAD_CONST(NodeRigidBody::Proxy, float)			(BIND(GetFrictionCoefficient)),
			"SetMassDensity", OVERLOAD(NodeRigidBody::Proxy, void, float)		(BIND(SetMassDensity)),
			"GetMassDensity", OVERLOAD_CONST(NodeRigidBody::Proxy, float)			(BIND(GetMassDensity)),

			"SetBounciness", OVERLOAD(NodeRigidBody::Proxy, void, int, float)	(BIND(SetBounciness)),
			"GetBounciness", OVERLOAD_CONST(NodeRigidBody::Proxy, float, int)	(BIND(GetBounciness)),
			"SetFrictionCoefficient", OVERLOAD(NodeRigidBody::Proxy, void, int, float)	(BIND(SetFrictionCoefficient)),
			"GetFrictionCoefficient", OVERLOAD_CONST(NodeRigidBody::Proxy, float, int)	(BIND(GetFrictionCoefficient)),
			"SetMassDensity", OVERLOAD(NodeRigidBody::Proxy, void, int, float)	(BIND(SetMassDensity)),
			"GetMassDensity", OVERLOAD_CONST(NodeRigidBody::Proxy, float, int)	(BIND(GetMassDensity)),

			"IsSleeping", BIND(IsSleeping),
			"IsAllowedToSleep", BIND(IsAllowedToSleep),
			"IsGravityEnabled", BIND(IsGravityEnabled),
			"SetSleepingEnabled", BIND(SetSleepingEnabled),
			"SetSleepingState", BIND(SetSleepingState),
			"SetIsGravityEnabled", BIND(SetIsGravityEnabled),

			"GetNode3DParent", BIND(GetNode3DParent)
		);
	};
};
