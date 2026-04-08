
class Node3D::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node), m_pNode(static_cast<Node3D*>(&node)) {}

	static Proxy* CreateNode3DProxy(std::string const& name);

	// =========== Getters ===========

	glm::vec3 GetPosition() const									{ return m_pNode->GetPosition(); }
	float GetX() const												{ return m_pNode->GetX(); }
	float GetY() const												{ return m_pNode->GetY(); }
	float GetZ() const												{ return m_pNode->GetZ(); }
	glm::mat4 GetMatrixRotation() const								{ return m_pNode->GetMatrixRotation(); }
	//const glm::mat4& GetInverseMatrixRotation()					{ return m_pNode->GetInverseMatrixRotation(); }

	//Euler angles
	EulerAngles GetLocalRotationRad() const							{ return m_pNode->GetLocalRotationRad(); }
	EulerAngles GetLocalRotationDeg() const							{ return m_pNode->GetLocalRotationDeg(); }

	// { w, x, y, z }
	const glm::quat& GetLocalRotationQuat() const					{ return m_pNode->GetLocalRotationQuat(); }
	// -- Angle around Y axis in Degrees
	float GetLocalYaw() const										{ return m_pNode->GetLocalYaw(); }
	// -- Angle around X axis in Degrees
	float GetLocalPitch() const										{ return m_pNode->GetLocalPitch(); }
	// -- Angle around Z axis in Degrees
	float GetLocalRoll() const										{ return m_pNode->GetLocalRoll(); }
	float GetMaxScale() const										{ return m_pNode->GetMaxScale(); }
	float GetMinScale() const										{ return m_pNode->GetMinScale(); }
	glm::vec3 GetScale() const										{ return m_pNode->GetScale(); }
	glm::vec3 GetLocalRight() const									{ return m_pNode->GetLocalRight(); }
	glm::vec3 GetLocalUp() const									{ return m_pNode->GetLocalUp(); }
	glm::vec3 GetLocalForward() const								{ return m_pNode->GetLocalForward(); }

	glm::mat4x4 const& GetWorldMatrix() const						{ return m_pNode->GetWorldMatrix(); }
	glm::vec3 GetWorldPosition() const								{ return m_pNode->GetWorldPosition(); }
	glm::vec3 GetWorldScale() const									{ return m_pNode->GetWorldScale(); }
	// Pitch, Yaw, Roll  / x, y, z
	// -- Angles in Degrees
	glm::vec3 GetWorldRotation() const								{ return m_pNode->GetWorldRotation(); }
	// { w, x, y, z }
	glm::quat const& GetWorldRotationQuaternion() const				{ return m_pNode->GetWorldRotationQuaternion(); }

	// =========== Setters ===========

	void SetLocalPosition(glm::vec3 const pos)						{ m_pNode->SetLocalPosition(pos); }
	void SetLocalX(float const x)									{ m_pNode->SetLocalX(x); }
	void SetLocalY(float const y)									{ m_pNode->SetLocalY(y); }
	void SetLocalZ(float const z)									{ m_pNode->SetLocalZ(z); }

	void SetLocalRotationDeg(glm::vec3 const& rotation)				{ m_pNode->SetLocalRotationDeg(rotation); }
	void SetLocalRotationRad(glm::vec3 const& rotation)				{ m_pNode->SetLocalRotationRad(rotation); }
	// { w, x, y, z }
	void SetLocalRotationQuat(glm::quat rot)						{ m_pNode->SetLocalRotationQuat(rot); }

	void SetScale(glm::vec3 const scale)							{ m_pNode->SetScale(scale); }

	void SetWorldPosition(glm::vec3 const& worldPos)				{ m_pNode->SetWorldPosition(worldPos); }
	void SetWorldScale(glm::vec3 const& worldScale)					{ m_pNode->SetWorldScale(worldScale); }
	// -- Angles in Degrees
	void SetWorldRotation(glm::vec3 const& worldRot)				{ m_pNode->SetWorldRotation(worldRot); }
	// { w, x, y, z }
	// -- Angles in Radians
	void SetWorldRotationQuaternion(glm::quat const& worldRotQuat)	{ m_pNode->SetWorldRotationQuaternion(worldRotQuat); }

	// =========== Adders ===========

	void AddScale(glm::vec3 const scale)							{ m_pNode->AddScale(scale); }
	void AddLocalPosition(glm::vec3 const pos)						{ m_pNode->AddLocalPosition(pos); }
	void AddLocalX(float const x)									{ m_pNode->AddLocalX(x); }
	void AddLocalY(float const y)									{ m_pNode->AddLocalY(y); }
	void AddLocalZ(float const z)									{ m_pNode->AddLocalZ(z); }
	// -- Angles in Radians
	void AddLocalRotation(glm::vec3 const rot)						{ m_pNode->AddLocalRotation(rot); }
	// -- Angles in Radians
	void AddLocalYaw(float const yaw)								{ m_pNode->AddLocalYaw(yaw); }
	// -- Angles in Radians
	void AddLocalPitch(float const pitch)							{ m_pNode->AddLocalPitch(pitch); }
	// -- Angles in Radians
	void AddLocalRoll(float const roll)								{ m_pNode->AddLocalRoll(roll); }

private:
	Node3D* m_pNode;
};

struct Node3D::Proxy::ProxyBinding
{
	static void Bind(Binder& binder)
	{
		binder.BindFunction("CreateNode3D", &Node3D::Proxy::CreateNode3DProxy);
		binder.BindClass<Node3D::Proxy>("node3d",
			sol::base_classes, sol::bases<Node::Proxy>(),
			sol::meta_function::garbage_collect, BIND(GCNodeProxy),
			sol::meta_function::new_index, StoreUserData(),
			sol::meta_function::index, LoadUserData(),
			"GetPosition", BIND(GetPosition),
			"GetX", BIND(GetX),
			"GetY", BIND(GetY),
			"GetZ", BIND(GetZ),
			"GetMatrixRotation", BIND(GetMatrixRotation),
			"GetLocalRotationRad", BIND(GetLocalRotationRad),
			"GetLocalRotationDeg", BIND(GetLocalRotationDeg),
			"GetLocalRotationQuat", BIND(GetLocalRotationQuat),
			"GetLocalYaw", BIND(GetLocalYaw),
			"GetLocalPitch", BIND(GetLocalPitch),
			"GetLocalRoll", BIND(GetLocalRoll),
			"GetMaxScale", BIND(GetMaxScale),
			"GetMinScale", BIND(GetMinScale),
			"GetScale", BIND(GetScale),
			"GetLocalRight", BIND(GetLocalRight),
			"GetLocalUp", BIND(GetLocalUp),
			"GetLocalForward", BIND(GetLocalForward),
			"GetWorldMatrix", BIND(GetWorldMatrix),
			"GetWorldPosition", BIND(GetWorldPosition),
			"GetWorldScale", BIND(GetWorldScale),
			"GetWorldRotation", BIND(GetWorldRotation),
			"GetWorldRotationQuaternion", BIND(GetWorldRotationQuaternion),
			"SetLocalPosition", BIND(SetLocalPosition),
			"SetLocalX", BIND(SetLocalX),
			"SetLocalY", BIND(SetLocalY),
			"SetLocalZ", BIND(SetLocalZ),
			"SetLocalRotationDeg", BIND(SetLocalRotationDeg),
			"SetLocalRotationRad", BIND(SetLocalRotationRad),
			"SetLocalRotationQuat", BIND(SetLocalRotationQuat),
			"SetScale", BIND(SetScale),
			"SetWorldPosition", BIND(SetWorldPosition),
			"SetWorldScale", BIND(SetWorldScale),
			"SetWorldRotation", BIND(SetWorldRotation),
			"SetWorldRotationQuaternion", BIND(SetWorldRotationQuaternion),

			"AddScale", BIND(AddScale),
			"AddLocalPosition", BIND(AddLocalPosition),
			"AddLocalX", BIND(AddLocalX),
			"AddLocalY", BIND(AddLocalY),
			"AddLocalZ", BIND(AddLocalZ),
			"AddLocalRotation", BIND(AddLocalRotation),
			"AddLocalYaw", BIND(AddLocalYaw),
			"AddLocalPitch", BIND(AddLocalPitch),
			"AddLocalRoll", BIND(AddLocalRoll)
		);
	};
};

REGISTER_PROXY(Node3D::Proxy::ProxyBinding, Node3DProxy);
