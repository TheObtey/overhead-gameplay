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
}

void Node2D::SetScale(float  _width, float _height)
{
	m_transform.SetScale(_width, _height);
}

glm::vec2 Node2D::GetScale() const
{
	return m_transform.GetScale();
}


void Node2D::SetRotation(float _x, float _y)
{
	m_transform.SetRotation(_x, _y);
}
glm::vec2 Node2D::GetRotation() const
{
	return m_transform.GetRotation();
}

void Node2D::SetPosition(glm::vec2 _pos)
{
	m_transform.SetPosition(_pos);
}
void Node2D::SetPosition(float _x, float _y)
{
	m_transform.SetPosition(_x, _y);
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

	glm::mat3 parentInv = glm::inverse(pParent->m_worldTransform);
	glm::mat3 localMatrix = parentInv * m_transform.GetTransformationMatrix();

	glm::vec2 pos = { localMatrix[2][0], localMatrix[2][1] };

	glm::vec2 scale = {
		glm::length(glm::vec2(localMatrix[0][0], localMatrix[0][1])),
		glm::length(glm::vec2(localMatrix[1][0], localMatrix[1][1]))
	};

	float rot = atan2(localMatrix[1][0], localMatrix[0][0]);

	m_transform.SetScale(scale);
	m_transform.SetRotation(rot, 0);
	m_transform.SetPosition(pos);
}

void Node2D::Reparent(Node& newParent, bool keepGlobalTransform)
{
	if (keepGlobalTransform)
		UpdateWorld();

	Node::Reparent(newParent, keepGlobalTransform);

	CheckParentTransform();

	if (keepGlobalTransform)
		UpdateLocal();
}

void Node2D::CheckParentTransform()
{
	if (GetParent() == nullptr)
	{
		m_isParentNode2D = false;
		return;
	}

	m_isParentNode2D = dynamic_cast<Node2D*>(GetParent()) != nullptr;
}

void Node2D::UpdateWorld()
{
	glm::vec2 localScale = m_transform.GetScale();
	glm::vec2 localPos = m_transform.GetPosition();
	
	float angle = m_transform.GetRotation().x;

	glm::mat3 S = glm::mat3(
		localScale.x, 0, 0,
		0, localScale.y, 0,
		0, 0, 1
	);

	glm::mat3 R = glm::mat3(
		cos(angle), sin(angle), 0,
		-sin(angle), cos(angle), 0,
		0, 0, 1
	);

	glm::mat3 T = glm::mat3(
		1, 0, 0,
		0, 1, 0,
		localPos.x, localPos.y, 1
	);

	glm::mat3 localMatrix = T * R * S;

	CheckParentTransform();

	if (!m_isParentNode2D)
	{
		m_worldTransform = localMatrix;
	}
	else 
	{
		Node2D* pParent = static_cast<Node2D*>(GetParent());
		m_worldTransform = pParent->m_worldTransform * localMatrix;
	}

	m_worldPosition = m_worldTransform * glm::vec3(0, 0, 1);

	m_worldScale = {
		glm::length(glm::vec2(m_worldTransform[0])),
		glm::length(glm::vec2(m_worldTransform[1])),
		1.0f
	};

	m_worldRotation = {
		atan2(m_worldTransform[1][0], m_worldTransform[0][0]),
		0,
		1
	};
}

void Node2D::SetWorldScale(glm::vec3& _worldScale)
{
	m_worldScale = _worldScale;

	UpdateLocal();
}

void Node2D::SetWorldRotation(glm::vec3& _worldRot)
{
	m_worldRotation = _worldRot;

	UpdateLocal();
}

void Node2D::SetWorldPosition(glm::vec3& _worldPos)
{
	m_worldPosition = _worldPos;

	UpdateLocal();
}

void Node2D::OnUpdate(double _delta)
{
	Node::OnUpdate(_delta);

	UpdateWorld();
}

