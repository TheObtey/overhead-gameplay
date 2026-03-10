//#include "Nodes/Node2D.h"
//
//class Node2D::Proxy : public Node::Proxy
//{
//public:
//	struct ProxyBinding;
//
//	void SetScale(glm::vec2 _scale) 
//	{ 
//		m_pNode2D->SetScale(_scale);
//	}
//	void SetScale(float _width, float _height) 
//	{
//		m_pNode2D->SetScale(_width, _height); 
//	}
//	glm::uvec2 GetScale()
//	{ 
//		return m_pNode2D->GetScale(); 
//	}
//
//	void SetRotation(float _theta) 
//	{ 
//		m_pNode2D->SetRotation(_theta); 
//	}
//	glm::mat2 GetRotationMatrix() const 
//	{ 
//		return m_pNode2D->GetRotation();
//	}
//
//	void SetPosition(glm::vec2 _pos) 
//	{ 
//		m_pNode2D->SetPosition(_pos);
//	}
//	void SetPosition(float _x, float _y) 
//	{ 
//		m_pNode2D->SetPosition(_x, _y); 
//	}
//	glm::vec2	GetPosition() const 
//	{ 
//		return m_pNode2D->GetPosition(); 
//	}
//
//	glm::mat3	GetTransformationMatrix() const 
//	{ 
//		return m_pNode2D->GetTransformationMatrix(); 
//	}
//
//	void SetStatism(bool _statism) 
//	{ 
//		m_pNode2D->SetStatism(_statism); 
//	}
//	bool IsStatic() const 
//	{ 
//		return m_pNode2D->IsStatic(); 
//	}
//
//	void UpdateLocal()
//	{
//		return m_pNode2D->UpdateLocal();
//	}
//	void UpdateWorld()
//	{
//		return m_pNode2D->UpdateWorld();
//	}
//
//private:
//	Node2D* m_pNode2D;
//};
//
//BindProxy(Node2D::Proxy,
//	binder.BindClass<Node2D::Proxy>("Node2D",
//		"SetScale", OVERLOAD(Node2D::Proxy, void, glm::vec2)(BIND(SetScale)),
//		"SetScale", OVERLOAD(Node2D::Proxy, void, float, float)(BIND(SetScale)),
//		"GetScale", BIND(GetScale),
//
//		"SetRotation", BIND(SetRotation),
//		"GetRotationMatrix", BIND(GetRotationMatrix),
//
//		"SetPosition", OVERLOAD(Node2D::Proxy, void, glm::vec2)(BIND(SetPosition)),
//		"SetPosition", OVERLOAD(Node2D::Proxy, void, float, float)(BIND(SetPosition)),
//		"GetPosition", BIND(GetPosition),
//
//		"GetTransformationMatrix", BIND(GetTransformationMatrix),
//
//		"SetStatism", BIND(SetStatism),
//		"IsStatic", BIND(IsStatic),
//
//		"UpdateLocal", BIND(UpdateLocal),
//		"UpdateWorld", BIND(UpdateWorld),
//
//		"SetWorldScale", BIND(SetWorldScale)
//		"SetWorldRotation", BIND(SetWorldRotation)
//		"SetWorldPosition", BIND(SetWorldPosition),
//
//		"CheckParentTransform", BIND(CheckParentTransform))
//	;)