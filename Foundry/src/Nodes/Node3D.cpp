#include "Nodes/Node3D.h"
#include "MathUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

Node3D::Node3D(std::string const& name) : Node(name), m_isParentNode3D(false)
{
	OnParentChange.Subscribe([&](Node& n) { CheckParentTransform(); });
	UpdateWorldTransform();
}

void Node3D::OnUpdate(double delta)
{
	Node::OnUpdate(delta);
	m_localDirty = m_transform.GetDirty();
	m_localDirty |= m_isParentNode3D && static_cast<Node3D*>(m_pOwner)->m_localDirty;
	m_transform.Update();
	UpdateWorldTransform();
	UpdateLocalTransform();
}

void Node3D::Reparent(Node& newParent, bool keepGlobalTransform)
{
	Node::Reparent(newParent, keepGlobalTransform);
	m_worldDirty = true;
}

void Node3D::CheckParentTransform()
{
	if (m_pOwner == nullptr) return;
	m_isParentNode3D = dynamic_cast<Node3D*>(GetParent());
}

void Node3D::UpdateWorldTransform()
{
	if (m_localDirty == false) return;

	if (m_isParentNode3D == false)
	{
		m_worldPosition = m_transform.GetPosition();
		m_worldScale = m_transform.GetScale();
		m_worldRotation = m_transform.GetRotationQuat();
		m_worldTransform = m_transform.GetMatrix();
		return;
	}

	Node3D* parent = static_cast<Node3D*>(m_pOwner);
	m_worldScale = m_transform.GetScale() * parent->m_worldScale;
	m_worldRotation = parent->m_worldRotation * m_transform.GetRotationQuat();
	m_worldPosition = m_transform.GetPosition() * parent->m_worldScale;

	glm::mat4 rotMatrix = glm::transpose(glm::toMat4(parent->m_worldRotation));
	m_worldPosition = m_worldPosition * rotMatrix;

	m_worldPosition += parent->m_worldPosition;
	m_worldPosition.w = 1.0f;

	m_worldTransform = Maths::Translate(m_worldPosition) * glm::toMat4(m_worldRotation) * Maths::Scale(m_worldScale);
}

void Node3D::UpdateLocalTransform()
{
	if (!m_worldDirty) return;

	if (m_isParentNode3D == false)
	{
		m_transform.SetPosition(m_worldPosition);
		m_transform.SetScale(m_worldScale);
		m_transform.SetRotationQuat(m_worldRotation);
		m_worldDirty = false;
		return;
	}

	Node3D* parent = static_cast<Node3D*>(m_pOwner);

	glm::vec4 newLocal = m_worldPosition - parent->m_worldPosition;
	newLocal.w = 1.0f;
	glm::mat4 rotMatrix = glm::toMat4(glm::quat(parent->m_worldRotation));
	newLocal = newLocal * rotMatrix;
	newLocal /= parent->m_worldScale;
	newLocal.w = 1.0f;
	m_transform.SetPosition(newLocal);

	glm::vec4 newScale = m_worldScale / parent->m_worldScale;
	newScale.w = 1.0f;
	m_transform.SetScale(newScale);

	glm::quat newRot = glm::inverse(parent->m_worldRotation) * m_worldRotation;
	m_transform.SetRotationQuat(newRot);
	m_worldDirty = false;
}

glm::mat4x4 const& Node3D::GetWorldMatrix() const
{
	return m_worldTransform;
}

glm::vec3  Node3D::GetWorldPosition() const
{
	return m_worldPosition;
}

glm::vec3 Node3D::GetWorldScale() const
{
	return m_worldScale;
}

glm::vec3 Node3D::GetWorldRotation() const
{
	return glm::eulerAngles(m_worldRotation);
}

void Node3D::SetWorldPosition(glm::vec3 const& worldPos)
{
	m_worldPosition = glm::vec4(worldPos, 1.0f);
	m_worldDirty = true;
	UpdateLocalTransform();
}
void Node3D::SetWorldScale(glm::vec3 const& worldScale)
{
	m_worldScale = glm::vec4(worldScale, 1.0f);
	m_worldDirty = true;
	UpdateLocalTransform();
}
void Node3D::SetWorldRotation(glm::vec3 const& worldRot)
{
	m_worldRotation = glm::quat(worldRot);
	m_worldDirty = true;
	UpdateLocalTransform();
}
