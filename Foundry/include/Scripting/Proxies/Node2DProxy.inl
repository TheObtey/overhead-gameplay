
class Node2D::Proxy : public Node::Proxy
{
public:
	struct ProxyBinding;

	Proxy(Node& node) : Node::Proxy(node), m_pNode2D(static_cast<Node2D*>(&node)) {}

	void SetScale(glm::vec2 _scale) 
	{ 
		m_pNode2D->SetScale(_scale);
	}
	void SetScale(float _width, float _height) 
	{
		m_pNode2D->SetScale(_width, _height); 
	}
	glm::uvec2 GetScale()
	{ 
		return m_pNode2D->GetScale(); 
	}

	void SetRotation(glm::vec2 _rot) 
	{ 
		m_pNode2D->SetRotation(_rot.x, _rot.y);
	}
	glm::vec2 GetRotation() const 
	{ 
		return m_pNode2D->GetRotation();
	}

	void SetPosition(glm::vec2 _pos) 
	{ 
		m_pNode2D->SetPosition(_pos);
	}
	void SetPosition(float _x, float _y) 
	{ 
		m_pNode2D->SetPosition(_x, _y); 
	}
	glm::vec2	GetPosition() const 
	{ 
		return m_pNode2D->GetPosition(); 
	}

	glm::mat3	GetTransformationMatrix() const 
	{ 
		return m_pNode2D->GetTransformationMatrix(); 
	}

	void SetStatism(bool _statism) 
	{ 
		m_pNode2D->SetStatism(_statism); 
	}
	bool IsStatic() const 
	{ 
		return m_pNode2D->IsStatic(); 
	}

	void UpdateLocal()
	{
		m_pNode2D->UpdateLocal();
	}
	void UpdateWorld()
	{
		m_pNode2D->UpdateWorld();
	}

	void SetWorldScale(glm::vec3 _scale)
	{
		m_pNode2D->SetWorldScale(_scale);
	}
	void SetWorldRotation(glm::vec3 _rot)
	{
		m_pNode2D->SetWorldRotation(_rot);
	}
	void SetWorldPosition(glm::vec3 _pos)
	{
		m_pNode2D->SetWorldPosition(_pos);
	}

private:
	Node2D* m_pNode2D;
};

BindProxy(Node2D::Proxy,
	binder.BindClass<Node2D::Proxy>("Node2D",
		"SetScale", OVERLOAD(Node2D::Proxy, void, glm::vec2)(BIND(SetScale)),
		"SetScale", OVERLOAD(Node2D::Proxy, void, float, float)(BIND(SetScale)),
		"GetScale", BIND(GetScale),

		"SetRotation", BIND(SetRotation),
		"GetRotation", BIND(GetRotation),

		"SetPosition", OVERLOAD(Node2D::Proxy, void, glm::vec2)(BIND(SetPosition)),
		"SetPosition", OVERLOAD(Node2D::Proxy, void, float, float)(BIND(SetPosition)),
		"GetPosition", BIND(GetPosition),

		"GetTransformationMatrix", BIND(GetTransformationMatrix),

		"SetStatism", BIND(SetStatism),
		"IsStatic", BIND(IsStatic),

		"UpdateLocal", BIND(UpdateLocal),
		"UpdateWorld", BIND(UpdateWorld),

		"SetWorldScale", BIND(SetWorldScale),
		"SetWorldRotation", BIND(SetWorldRotation),
		"SetWorldPosition", BIND(SetWorldPosition));
);

inline static Node2D::Proxy::ProxyBinding Node2DBinding{};