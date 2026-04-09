#include "Nodes/Node2D.h"
#include "Serialization/SerializeObject.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "glm/glm.hpp"


Node2D::Node2D(std::string const &name) : Node(name)
{
	OnParentChange += [&](Node& node)
	{
		CheckParentTransform(node);
		m_localDirty = true;
		UpdateWorld();
	};
}

Node2D::~Node2D() {}

void Node2D::Serialize(SerializedObject& datas) const
{
	Node::Serialize(datas);
	datas.SetType("Node2D");
	datas.AddPublicElement("Transform2D", static_cast<ISerializable const*>(&m_transform));
}


void Node2D::Deserialize(SerializedObject const& datas)
{
	Node::Deserialize(datas);
	datas.GetPublicElement("Transform2D", static_cast<ISerializable*>(&m_transform));
}

ISerializable* Node2D::CreateInstance()
{
	return CreateNode<Node2D>("Node2D").release();
}

void Node2D::SetScale(glm::vec2 const& _scale)
{
	m_transform.SetScale(_scale);
}

void Node2D::SetScale(float const _width, float const _height)
{
	m_transform.SetScale(_width, _height);
}

glm::vec2 const& Node2D::GetScale() const
{
	return m_transform.GetScale();
}

void Node2D::SetRotation(float const _theta)
{
	m_transform.SetRotation(_theta);
}
float Node2D::GetRotation() const
{
	return m_transform.GetRotation();
}

void Node2D::SetShearing(glm::vec2 const &shear)
{
	m_transform.SetShearing(shear);
}

glm::vec2 const & Node2D::GetShearing() const
{
	return m_transform.GetShearing();
}

void Node2D::SetPosition(glm::vec2 const& _pos)
{
	m_transform.SetPosition(_pos);
}
void Node2D::SetPosition(float const _x, float const _y)
{
	m_transform.SetPosition(_x, _y);
}
glm::vec2 const& Node2D::GetPosition() const
{
	return m_transform.GetPosition();
}

glm::mat3 const& Node2D::GetTransformationMatrix()
{
	return m_worldTransform;
}

void Node2D::SetMirroringOnAxis(Transform2D::Axis const _axis)
{
	m_transform.SetMirroringOnAxis(_axis);
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
	if (!m_worldDirty) return;

	if (m_isParentNode2D == false)
	{
		m_transform.SetPosition(m_worldPosition);
		m_transform.SetScale(m_worldScale);
		m_transform.SetRotation(m_worldRotationAngle);
		m_worldDirty = false;
		return;
	}

	Node2D* parent = static_cast<Node2D*>(GetParent());

	glm::vec3 newLocal = m_worldPosition - parent->m_worldPosition;
	newLocal.z = 1.0f;
	float theta = parent->GetWorldRotation();
	glm::mat3 rotMatrix = glm::mat3(
		cos(theta), -sin(theta), 0,
		sin(theta), cos(theta), 0,
		0, 0, 1
	);
	newLocal = newLocal * rotMatrix;
	newLocal /= parent->m_worldScale;
	newLocal.z = 1.0f;
	m_transform.SetPosition(newLocal);

	glm::vec3 newScale = m_worldScale / parent->m_worldScale;
	newScale.z = 1.0f;
	m_transform.SetScale(newScale);

	m_transform.SetRotation(parent->GetWorldRotation() + m_worldRotationAngle);
	m_worldDirty = false;
}

void Node2D::Reparent(Node& newParent, bool keepGlobalTransform)
{
	Node::Reparent(newParent, keepGlobalTransform);
	if (keepGlobalTransform) m_worldDirty = true;
}

void Node2D::CheckParentTransform(Node& node)
{
	if (m_pOwner == nullptr) return;
	m_isParentNode2D = dynamic_cast<Node2D*>(m_pOwner);
}

void Node2D::UpdateWorld()
{
	if (m_localDirty == false) return;

	if (m_isParentNode2D == false)
	{
		m_worldPosition		 = glm::vec3(m_transform.GetPosition(), 1.0f);
		m_worldRotationAngle = m_transform.GetRotation();
		m_worldScale		 = glm::vec3(m_transform.GetScale(), 1.0f);
		m_worldTransform	 = m_transform.GetTransformationMatrix();
		return;
	}

	Node2D* parent = static_cast<Node2D*>(m_pOwner);
	m_worldTransform = parent->m_worldTransform * m_transform.GetTransformationMatrix();
	
	m_worldPosition = glm::vec3(m_worldTransform[2][0], m_worldTransform[2][1], m_worldTransform[2][2]);
	float scaleX = glm::length(glm::vec2(m_worldTransform[0][0], m_worldTransform[1][0]));
	float scaleY = glm::length(glm::vec2(m_worldTransform[0][1], m_worldTransform[1][1]));
	m_worldScale = {scaleX, scaleY, 1.0f};

	 glm::mat3 rotation {1.0f};
	 rotation[0][0] = m_worldTransform[0][0] / scaleX;
	 rotation[1][0] = m_worldTransform[1][0] / scaleX;

	 rotation[0][1] = m_worldTransform[0][1] / scaleY;
	 rotation[1][1] = m_worldTransform[1][1] / scaleY;

	 m_worldRotationAngle = glm::eulerAngles(glm::toQuat(rotation)).z;
}

void Node2D::SetWorldScale(glm::vec3 const& _worldScale)
{
	m_worldScale = _worldScale;
	m_worldDirty = true;
	UpdateLocal();
}

void Node2D::SetWorldRotationAngle(float const _worldRot)
{
	m_worldRotationAngle = _worldRot;
	m_worldDirty = true;
	UpdateLocal();
}

void Node2D::SetWorldPosition(glm::vec3 const& _worldPos)
{
	m_worldPosition = _worldPos;
	m_worldDirty = true;
	UpdateLocal();
}

glm::vec3 const & Node2D::GetWorldPosition() const
{
	return m_worldPosition;
}

glm::vec3 const & Node2D::GetWorldScale() const
{
	return m_worldScale;
}

float Node2D::GetWorldRotation() const
{
	return m_worldRotationAngle;
}

void Node2D::OnUpdate(double const _delta)
{
	Node::OnUpdate(_delta);
	m_localDirty = m_transform.GetDirty();
	m_localDirty |= m_isParentNode2D && static_cast<Node2D*>(m_pOwner)->m_localDirty;
	m_transform.Update();
	UpdateWorld();
	UpdateLocal();
}

uptr<Node> Node2D::Clone()
{
	uptr<Node2D> clone = Node::CreateNode<Node2D>(GetName());

	SerializedObject datas;
	Serialize(datas);
	clone->Deserialize(datas);

	return clone;
}