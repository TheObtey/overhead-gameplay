#include "Nodes/Node2D.h"

#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


Node2D::Node2D(
	float _x, float _y,
	float _scaleX, float _scaleY,
	bool _statism) : 
	Node("Node2D"),
	m_transform(
		_x, _y,
		_scaleX, _scaleY,
		_statism),
	m_worldScale(),
	m_worldPosition(),
	m_worldRotation(),
	m_worldTransform(),
	m_isParentNode2D(false)
{}
Node2D::Node2D(Transform2D _transform) : 
	Node("Node2D"),
	m_transform(_transform), 
	m_worldScale(),
	m_worldPosition(),
	m_worldRotation(),
	m_worldTransform(),
	m_isParentNode2D(false)
{}

Node2D::~Node2D() {}


Node2D Node2D::operator*(Node2D const& other) const
{
	return Node2D(m_transform * other.m_transform);
}
Node2D& Node2D::operator*=(Node2D const& other)
{
	m_transform *= other.m_transform;
	return *this;
}
		
Node2D Node2D::operator+(Node2D const& other) const
{
	return m_transform + other.m_transform;
}
Node2D& Node2D::operator+=(Node2D const& other)
{
	m_transform += other.m_transform;
	return *this;
}

Node2D Node2D::operator-(Node2D const& other) const
{
	return m_transform - other.m_transform;
}
Node2D& Node2D::operator-=(Node2D const& other)
{
	m_transform -= other.m_transform;
	return *this;
}

Node2D Node2D::operator/(Node2D const& other) const
{
	return m_transform / other.m_transform;
}
Node2D& Node2D::operator/=(Node2D const& other)
{
	m_transform /= other.m_transform;
	return *this;
}


void Node2D::SetScale(glm::vec2 _scale)
{
	m_transform.SetScale(_scale);
	/*for (int i = 0; i < GetChildren().size(); i++)
	{
		reinterpret_cast<Node2D&>(GetChild(i)).SetScale(_scale);
	}*/
}

void Node2D::SetScale(float  _width, float _height)
{
	m_transform.SetScale(_width, _height);
	/*for (int i = 0; i < GetChildren().size(); i++)
	{
		reinterpret_cast<Node2D&>(GetChild(i)).SetScale(_width, _height);
	}*/
}

glm::uvec2 Node2D::GetScale() const
{
	return m_transform.GetScale();
}


void Node2D::SetRotation(float _x, float _y)
{
	m_transform.SetRotation(_x, _y);
	/*for (int i = 0; i < GetChildren().size(); i++)
	{
		reinterpret_cast<Node2D&>(GetChild(i)).SetRotation(_x, _y);
	}*/
}
glm::vec2 Node2D::GetRotation() const
{
	return m_transform.GetRotation();
}

void Node2D::SetPosition(glm::vec2 _pos)
{
	m_transform.SetPosition(_pos);
	/*for (int i = 0; i < GetChildren().size(); i++)
	{
		reinterpret_cast<Node2D&>(GetChild(i)).SetPosition(_pos);
	}*/
}
void Node2D::SetPosition(float _x, float _y)
{
	m_transform.SetPosition(_x, _y);
	/*for (int i = 0; i < GetChildren().size(); i++)
	{
		reinterpret_cast<Node2D&>(GetChild(i)).SetPosition(_x, _y);
	}*/
}
glm::vec2 Node2D::GetPosition() const
{
	return m_transform.GetPosition();
}

glm::mat3& Node2D::GetTransformationMatrix()
{
	return m_worldTransform;
}

void Node2D::SetStatism(bool _statism)
{
	m_transform.SetStatism(_statism);
	/*for (int i = 0; i < GetChildren().size(); i++)
	{
		reinterpret_cast<Node2D&>(GetChild(i)).SetStatism(_statism);
	}*/
}
bool Node2D::IsStatic() const
{
	return m_transform.IsStatic();
}

void Node2D::UpdateLocal()
{
	if (GetParent() == nullptr)
		return;

	Node2D* pParent = static_cast<Node2D*>(GetParent());

	glm::vec3 T = m_worldPosition - pParent->m_worldPosition;

	glm::mat3 rotMatrix = glm::toMat3(glm::quat({ 
		pParent->m_worldRotation.x, 
		pParent->m_worldRotation.y, 
		pParent->m_worldRotation.z, 
		1.0f 
	}));
	T = T * rotMatrix;
	T /= pParent->m_worldScale;
	T.z = 1.0f;
	m_transform.SetPosition(T);

	glm::vec3 S = m_worldScale / pParent->m_worldScale;
	m_transform.SetScale(S);
	
	glm::mat3 newWorldRot = glm::toMat3(glm::quat({
		pParent->m_worldRotation.x,
		pParent->m_worldRotation.y,
		pParent->m_worldRotation.z,
		1.0f
	}));
	glm::vec3 R = glm::inverse(newWorldRot) * m_worldRotation;
	m_transform.SetRotation(R.x, R.y);
}

void Node2D::Reparent(Node& newParent, bool keepGlobalTransform)
{
	Node::Reparent(newParent, keepGlobalTransform);
}

void Node2D::CheckParentTransform()
{
	if (GetParent() == nullptr)
		return;
	m_isParentNode2D = dynamic_cast<Node2D*>(GetParent());
}

void Node2D::UpdateWorld()
{
	if (m_isParentNode2D == false)
	{
		m_worldPosition = { m_transform.GetPosition(), 1.0f };
		m_worldRotation = { m_transform.GetRotation(), 1.0f };
		m_worldScale = { m_transform.GetScale(), 1.0f};
		m_worldTransform = m_transform.GetTransformationMatrix();
		return;
	}

	Node2D* pParent = static_cast<Node2D*>(GetParent());
	m_worldScale = glm::vec3({ m_transform.GetScale(), 1.0f }) * pParent->m_worldScale;
	m_worldRotation = pParent->m_worldRotation * glm::vec3(m_transform.GetRotation(), 1.0f);
	m_worldPosition = glm::vec3({ m_transform.GetPosition(), 1.0f }) * pParent->m_worldScale;

	glm::mat3 newWorldRot = glm::toMat3(glm::quat({
		pParent->m_worldRotation.x,
		pParent->m_worldRotation.y,
		pParent->m_worldRotation.z,
		1.0f
		}));
	m_worldPosition = m_worldPosition * glm::transpose(newWorldRot);
	
	m_worldPosition += pParent->m_worldPosition;

	glm::mat3 S = glm::mat3(
		m_transform.GetScale().x, 0, 0,
		0, m_transform.GetScale().y, 0,
		0, 0, 1
	);

	glm::mat3 T = glm::mat3(
		1, 0, 0,
		0, 1, 0,
		m_transform.GetPosition().x, m_transform.GetPosition().y, 1
	);
	m_worldTransform = T * glm::toMat3(glm::quat(m_worldRotation)) * S;
}

void Node2D::SetWorldPosition(glm::vec3& _worldPos)
{
	m_worldPosition = _worldPos;

	UpdateLocal();
}

void Node2D::OnUpdate(float _delta)
{
	Node::OnUpdate(_delta);
	UpdateLocal();
	UpdateWorld();
}

