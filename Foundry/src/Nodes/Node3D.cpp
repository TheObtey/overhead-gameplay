#include "Nodes/Node3D.h"
#include "MathUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

Node3D::Node3D(std::string const& name) : 
	Node(name),
	m_isParentNode3D(false)
{
	OnParentChange.Subscribe([&](Node& n) {CheckParentTransform(); });
}

void Node3D::OnUpdate(float delta)
{
	Node::OnUpdate(delta);
	m_transform.Update();
	UpdateWorldTransform();
}
glm::mat4x4& Node3D::GetWorldMatrice()
{
	return m_worldTransform;
	//if (m_isParentNode3D == false)
	//{
	//	return GetMatrix();
	//}
	//else
	//{
	//	auto* parent = static_cast<Node3D*>(m_pOwner);
	//	auto& pMat = parent->GetWorldMatrice();
	//	glm::mat4x4 worldMat = pMat * GetMatrix();
	//	return worldMat;
	//}
}

void Node3D::Reparent(Node& newParent, bool keepGlobalTransform)
{
	Node::Reparent(newParent, keepGlobalTransform);
	UpdateLocalTransform();
}

void Node3D::CheckParentTransform()
{
	if (m_pOwner == nullptr) return;
		m_isParentNode3D = dynamic_cast<Node3D*>(GetParent());
}

void Node3D::UpdateWorldTransform()
{
	Node3D* parent = static_cast<Node3D*>(m_pOwner);

	if (m_isParentNode3D)
	{
		m_worldScale	= m_transform.GetScale()	* parent->m_worldScale;
		m_worldRotation = m_transform.GetRotation() + parent->m_worldRotation;
		m_worldPosition = m_transform.GetPosition() * parent->m_worldScale;

		glm::mat4 rotMatrix = glm::transpose(glm::toMat4(glm::quat(parent->m_worldRotation)));
		m_worldPosition = m_worldPosition * rotMatrix;
		m_worldPosition += parent->m_worldPosition;

		//m_worldTransform = parent->m_worldTransform * m_transform.GetMatrix();

		 //glm::mat4 posMatrix = Maths::Translate(m_worldPosition);
		 //posMatrix = glm::toMat4(glm::quat(parent->m_worldRotation)) * posMatrix;
		 //m_worldPosition.x = posMatrix[3][0];
		 //m_worldPosition.y = posMatrix[3][1];
		 //m_worldPosition.z = posMatrix[3][2];

		//m_worldPosition += parent->m_worldPosition;
	}
	else
	{
		m_worldPosition = m_transform.GetPosition();
		m_worldScale	= m_transform.GetScale();
		m_worldRotation = m_transform.GetRotation();
		m_worldTransform = m_transform.GetMatrix();
		//m_worldTransform = Maths::Translate(m_worldPosition) * glm::toMat4(glm::quat(m_worldRotation))* Maths::Scale(m_worldScale);
	}
	m_worldTransform = Maths::Translate(m_worldPosition) * glm::toMat4(glm::quat(m_worldRotation))* Maths::Scale(m_worldScale);

}

void Node3D::UpdateLocalTransform()
{
	if (!m_isParentNode3D) return;
	Node3D* parent = static_cast<Node3D*>(m_pOwner);

	glm::vec4 newLocal = m_worldPosition - parent->m_worldPosition;
	glm::mat4 rotMatrix = glm::toMat4(glm::quat(parent->m_worldRotation));
	newLocal = newLocal * rotMatrix;
	newLocal /= parent->m_worldScale;
	m_transform.SetPosition(newLocal);

	glm::vec4 newScale = m_worldScale / parent->m_worldScale;
	m_transform.SetScale(newScale);

	glm::vec4 newRot = m_worldRotation - parent->m_worldRotation;
	m_transform.SetRotation(newRot);

	//glm::mat4 posMatrix = Maths::Translate(newLocal);
	//posMatrix = glm::inverse(glm::toMat4(glm::quat(parent->m_worldRotation))) * posMatrix;
	//newLocal.x = posMatrix[3][0];
	//newLocal.y = posMatrix[3][1];
	//newLocal.z = posMatrix[3][2];
	//newLocal /= parent->m_worldScale;
	//m_transform.SetPosition(newLocal);
}
