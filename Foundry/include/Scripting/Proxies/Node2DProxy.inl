
class Node2D::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node), m_pNode2D(static_cast<Node2D*>(&node)) {}

	void SetScale(glm::vec2 const& _scale) { m_pNode2D->SetScale(_scale); }
	void SetScale(float const _width, float const _height) { m_pNode2D->SetScale(_width, _height); }
	glm::vec2 const& GetScale() { return m_pNode2D->GetScale(); }

	void SetRotation(float const _theta) { m_pNode2D->SetRotation(_theta); }
	float GetRotation() const { return m_pNode2D->GetRotation(); }

	void SetPosition(glm::vec2 const& _pos) { m_pNode2D->SetPosition(_pos);}
	void SetPosition(float const _x, float const _y) { m_pNode2D->SetPosition(_x, _y); }
	glm::vec2 const& GetPosition() const { return m_pNode2D->GetPosition(); }

	void SetShearing(glm::vec2 const& shearing) { return m_pNode2D->SetShearing(shearing); }
	void SetShearing(float const x, float const y) { return m_pNode2D->SetShearing({x, y}); }
	glm::vec2 const& GetShearing() const { return m_pNode2D->GetShearing(); }

	void SetMirroringOnAxis(Transform2D::Axis axis) { m_pNode2D->SetMirroringOnAxis(axis); }

	glm::mat3 const& GetTransformationMatrix() const { return m_pNode2D->GetTransformationMatrix(); }

	void SetStatism(bool const _statism) { m_pNode2D->SetStatism(_statism); }
	bool IsStatic() const { return m_pNode2D->IsStatic(); }

	void UpdateLocal() { m_pNode2D->UpdateLocal(); }
	void UpdateWorld() { m_pNode2D->UpdateWorld(); }

	void SetWorldScale(glm::vec3 const& _scale) { m_pNode2D->SetWorldScale(_scale); }
	void SetWorldRotationAngle(float const _rot) { m_pNode2D->SetWorldRotationAngle(_rot); }
	void SetWorldPosition(glm::vec3 const& _pos) { m_pNode2D->SetWorldPosition(_pos); }

private:
	Node2D* m_pNode2D;
};

BindProxy(Node2D::Proxy,
	binder.BindClass<Node2D::Proxy>("node2d",
		sol::base_classes, sol::bases<Node::Proxy>(),
		sol::meta_function::garbage_collect, BIND(GCNodeProxy),
		sol::meta_function::new_index, StoreUserData(),
		sol::meta_function::index, LoadUserData(),
		"SetScale", OVERLOAD(Proxy, void, glm::vec2 const&)(BIND(SetScale)),
		"SetScale", OVERLOAD(Proxy, void, float, float)(BIND(SetScale)),
		"GetScale", BIND(GetScale),

		"SetRotation", BIND(SetRotation),
		"GetRotation", BIND(GetRotation),

		"SetPosition", OVERLOAD(Proxy, void, glm::vec2 const&)(BIND(SetPosition)),
		"SetPosition", OVERLOAD(Proxy, void, float, float)(BIND(SetPosition)),
		"GetPosition", BIND(GetPosition),

		"SetShearing", OVERLOAD(Proxy, void, glm::vec2 const&)(BIND(SetShearing)),
		"SetShearing", OVERLOAD(Proxy, void, float x, float y)(BIND(SetShearing)),
		"GetShearing", BIND(GetScale),

		"SetMirroringOnAxis", BIND(SetMirroringOnAxis),

		"SetStatism", BIND(SetStatism),
		"IsStatic", BIND(IsStatic),

		"SetWorldScale", BIND(SetWorldScale),
		"SetWorldRotationAngle", BIND(SetWorldRotationAngle),
		"SetWorldPosition", BIND(SetWorldPosition));
);

REGISTER_PROXY(Node2D::Proxy::ProxyBinding, Node2DProxy);