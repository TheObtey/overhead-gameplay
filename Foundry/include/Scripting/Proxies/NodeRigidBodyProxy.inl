
class NodeRigidBody::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node) {}

	// =========== Forces and Torques ===========

	void ApplyLocalForceAtCenterOfMass(const glm::vec3& force)								{ m_pNode->ApplyLocalForceAtCenterOfMass(force); }
	void ApplyLocalForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyLocalForceAtLocalPosition(force, point); }
	void ApplyLocalForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyLocalForceAtWorldPosition(force, point); }

	void ApplyWorldForceAtCenterOfMass(const glm::vec3& force)								{ m_pNode->ApplyWorldForceAtCenterOfMass(force); }
	void ApplyWorldForceAtLocalPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyWorldForceAtLocalPosition(force, point); }
	void ApplyWorldForceAtWorldPosition(const glm::vec3& force, const glm::vec3& point)		{ m_pNode->ApplyWorldForceAtWorldPosition(force, point); }

	void ApplyLocalTorque(const glm::vec3& torque)			{ m_pNode->ApplyLocalTorque(torque); }
	void ApplyWorldTorque(const glm::vec3& torque)			{ m_pNode->ApplyWorldTorque(torque); }

	glm::vec3 const GetLinearVelocity() const				{ return m_pNode->GetLinearVelocity(); }
	glm::vec3 const GetAngularVelocity() const				{ return m_pNode->GetAngularVelocity(); }
	/// Return the linear decelerating factor				
	float const		GetLinearDamping() const				{ return m_pNode->GetLinearDamping(); }
	/// Return the angular velocity damping factor			
	float const		GetAngularDamping() const				{ return m_pNode->GetAngularDamping(); }
	glm::vec3 const GetTotalForce() const					{ return m_pNode->GetTotalForce(); }


	void SetLinearVelocity(const glm::vec3& velocity)		{ m_pNode->SetLinearVelocity(velocity); }
	void SetAngularVelocity(const glm::vec3& velocity)		{ m_pNode->SetAngularVelocity(velocity); }
	/// Set the linear decelerating factor					
	void SetLinearDamping(float linearDamping)				{ m_pNode->SetLinearDamping(linearDamping); }
	/// Set the angular decelerating factor					
	void SetAngularDamping(float angularDamping)			{ m_pNode->SetAngularDamping(angularDamping); }

	void LockLinearAxis(bool x, bool y, bool z)				{ m_pNode->LockLinearAxis(x, y, z); }
	void LockAngularAxis(bool x, bool y, bool z)			{ m_pNode->LockAngularAxis(x, y, z); }

	void ResetForces()										{ m_pNode->ResetForces(); }
	void ResetTorque()										{ m_pNode->ResetTorque(); }

	// =========== Mass and Body Type ===========

	float GetMass() const									{ return m_pNode->GetMass(); }
	void  SetMass(float mass)								{ m_pNode->SetMass(mass); }

	RigidBodyType GetBodyType() const						{ return m_pNode->GetBodyType(); }
	void          SetBodyType(RigidBodyType type)			{ m_pNode->SetBodyType(type); }


	// =========== Sleeping and Gravity ===========

	bool IsSleeping() const									{ return m_pNode->IsSleeping(); }
	bool IsAllowedToSleep() const							{ return m_pNode->IsAllowedToSleep(); }
	bool IsGravityEnabled()									{ return m_pNode->IsGravityEnabled(); }

	void SetSleepingEnabled(bool enabled)					{ m_pNode->SetSleepingEnabled(enabled); }
	void SetSleepingState(bool isSleeping)					{ m_pNode->SetSleepingState(isSleeping); }
	void SetIsGravityEnabled(bool enabled)					{ m_pNode->SetIsGravityEnabled(enabled); }

	//glm::vec3 Getposition() const							{ m_pNode->Getposition(); }

private:
	NodeRigidBody* m_pNode;
};

BindProxy(NodeRigidBody::Proxy,
	return binder.BindClass<NodeRigidBody::Proxy>("noderigidbody",
		"ApplyLocalForceAtCenterOfMass", BIND(ApplyLocalForceAtCenterOfMass),
	"ApplyLocalForceAtLocalPosition", BIND(ApplyLocalForceAtLocalPosition),
	"ApplyLocalForceAtWorldPosition", BIND(ApplyLocalForceAtWorldPosition),
	"ApplyWorldForceAtCenterOfMass", BIND(ApplyWorldForceAtCenterOfMass),
	"ApplyWorldForceAtLocalPosition", BIND(ApplyWorldForceAtLocalPosition),
	"ApplyWorldForceAtWorldPosition", BIND(ApplyWorldForceAtWorldPosition),
	"ApplyLocalTorque", BIND(ApplyLocalTorque),
	"ApplyWorldTorque", BIND(ApplyWorldTorque),
	"GetLinearVelocity", BIND(GetLinearVelocity),
	"GetAngularVelocity", BIND(GetAngularVelocity),
	"GetLinearDamping", BIND(GetLinearDamping),
	"GetAngularDamping", BIND(GetAngularDamping),
	"GetTotalForce", BIND(GetTotalForce),
	"SetLinearVelocity", BIND(SetLinearVelocity),
	"SetAngularVelocity", BIND(SetAngularVelocity),
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
	"IsSleeping", BIND(IsSleeping),
	"IsAllowedToSleep", BIND(IsAllowedToSleep),
	"IsGravityEnabled", BIND(IsGravityEnabled),
	"SetSleepingEnabled", BIND(SetSleepingEnabled),
	"SetSleepingState", BIND(SetSleepingState),
	"SetIsGravityEnabled", BIND(SetIsGravityEnabled)

		);
)

REGISTER_PROXY(NodeRigidBody::Proxy::ProxyBinding, NodeRigidBodyProxy);