#include "Transform3D.h"
#include "MathUtils.h"
#include "Define.h"

#include "Serialization/SerializeObject.hpp"

Transform3D::Transform3D() :
	m_position(0.0f, 0.0f, 0.0f, 1.0f),
	m_scale(1.0f, 1.0f, 1.0f, 1.0f),
	m_isDirty(true),
	m_isInvDirty(true)
{
	Update();
}

void Transform3D::UpdateTransform()
{
	if (m_isDirty == false) return;
	m_transform = Maths::Scale(m_scale) * m_rotationMatrix * Maths::Translate(m_position); // Ore
	m_isDirty = false;
}

void Transform3D::UpdateRotationMatrix()
{
	if (m_isRotationDirty == false) return;

	m_rotationMatrix = glm::mat4_cast(m_rotationQuat);
	m_invRotationMatrix = glm::transpose(m_rotationMatrix);

	m_right = { m_rotationMatrix[0][0], m_rotationMatrix[1][0], m_rotationMatrix[2][0], 1.0f };
	m_up = { m_rotationMatrix[0][1], m_rotationMatrix[1][1], m_rotationMatrix[2][1], 1.0f };
	m_forward = { m_rotationMatrix[0][2], m_rotationMatrix[1][2], m_rotationMatrix[2][2], 1.0f };


	m_isRotationDirty = false;
}

void Transform3D::UpdateInvTransform()
{
	if (m_isInvDirty == false) return;

	m_invTransform = glm::inverse(m_transform);
	m_isInvDirty = false;
}

const glm::vec4& Transform3D::GetPosition() const
{
	return m_position;
}

float Transform3D::GetX() const
{
	return m_position.x;
}

float Transform3D::GetY() const
{
	return m_position.y;
}

float Transform3D::GetZ() const
{
	return m_position.z;
}

const glm::mat4& Transform3D::GetMatrixRotation() const
{
	return m_rotationMatrix;
}

const glm::mat4& Transform3D::GetInverseMatrixRotation() const
{
	return m_invRotationMatrix;
}

EulerAngles Transform3D::GetRotationRad() const
{
	glm::vec3 euler = glm::eulerAngles(m_rotationQuat);
	return { euler.x, euler.y, euler.z };
}

EulerAngles Transform3D::GetRotationDeg() const
{
	glm::vec3 euler = glm::degrees(glm::eulerAngles(m_rotationQuat));
	return { euler.x, euler.y, euler.z };
}

const glm::quat& Transform3D::GetRotationQuat() const
{
	return m_rotationQuat;
}

float Transform3D::GetYaw()   const
{
	return glm::eulerAngles(m_rotationQuat).y;
}
float Transform3D::GetPitch() const
{
	return glm::eulerAngles(m_rotationQuat).x;
}
float Transform3D::GetRoll()  const
{
	return glm::eulerAngles(m_rotationQuat).z;
}


float Transform3D::GetMaxScale() const
{
	return std::max(m_scale.x, std::max(m_scale.y, m_scale.z));
}
float Transform3D::GetMinScale() const
{
	return std::min(m_scale.x, std::min(m_scale.y, m_scale.z));
}
const glm::vec4& Transform3D::GetScale() const { return m_scale; }

const glm::vec4& Transform3D::GetUp() const
{
	return m_up;
}
const glm::vec4& Transform3D::GetRight() const
{
	return m_right;
}
const glm::vec4& Transform3D::GetForward() const
{
	return m_forward;
}

glm::mat4 const& Transform3D::GetMatrix() const
{
	return m_transform;
}

//glm::mat4& Transform3D::GetInvMatrix()
//{
//	return m_invTransform;
//}

void Transform3D::SetPosition(const glm::vec4& pos)
{
	m_position = pos;
	m_position.w = 1.0f;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::SetX(float x)
{
	m_position.x = x;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::SetY(float y)
{
	m_position.y = y;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::SetZ(float z)
{
	m_position.z = z;
	m_isDirty = m_isInvDirty = true;
}


void Transform3D::SetRotationDeg(float x, float y, float z)
{
	m_rotationQuat = glm::quat(glm::vec3(glm::radians(x), glm::radians(y), glm::radians(z)));
	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::SetRotationRad(float pitch, float yaw, float roll)
{
	m_rotationQuat = glm::quat(glm::vec3(pitch, yaw, roll));
	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}


void Transform3D::SetRotationQuat(const glm::quat& rot)
{
	m_rotationQuat = rot;

	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::SetScale(const glm::vec4& scale)
{
	m_scale = scale;
	m_scale.w = 1.0f;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::AddPosition(const glm::vec4& pos)
{
	m_position += pos;
	m_position.w = 1.0f;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::AddX(float x)
{
	m_position.x += x;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::AddY(float y)
{
	m_position.y += y;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::AddZ(float z)
{
	m_position.z += z;
	m_isDirty = m_isInvDirty = true;
}

void Transform3D::AddRotation(const glm::vec4& rot)
{
	glm::vec3 up = m_up;
	glm::vec3 right = m_right;
	glm::vec3 forward = m_forward;

	m_rotationQuat = glm::normalize(glm::angleAxis(rot[1], up) * m_rotationQuat);
	m_rotationQuat = glm::normalize(glm::angleAxis(rot[0], right) * m_rotationQuat);
	m_rotationQuat = glm::normalize(glm::angleAxis(rot[2], forward) * m_rotationQuat);


	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::AddYaw(float yaw)
{
	glm::vec3 up = GetUp();
	m_rotationQuat = glm::normalize(glm::angleAxis(yaw, up) * m_rotationQuat);

	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::AddPitch(float pitch)
{
	glm::vec3 right = GetRight();
	m_rotationQuat = glm::normalize(glm::angleAxis(pitch, right) * m_rotationQuat);

	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::AddRoll(float roll)
{
	glm::vec3 forward = m_forward;
	m_rotationQuat = glm::normalize(glm::angleAxis(roll, forward) * m_rotationQuat);

	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::AddScale(const glm::vec4& scale)
{
	m_scale += scale;
	m_scale.w = 1.0f;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::ApplyTransform(const Transform3D& transform)
{
	m_rotationMatrix = transform.GetMatrixRotation() * GetMatrixRotation();
	m_transform = transform.GetMatrix() * GetMatrix();

	m_isDirty = m_isInvDirty = m_isRotationDirty = true;
}

void Transform3D::Update()
{
	UpdateRotationMatrix();
	UpdateTransform();
	UpdateInvTransform();
}

Transform3D Transform3D::operator*(const Transform3D& other)
{
	Transform3D newTransform;
	newTransform.m_position = m_position;
	newTransform.m_position.w = 1.0f;
	newTransform.m_transform = other.GetMatrix() * GetMatrix();
	newTransform.m_isDirty = newTransform.m_isRotationDirty = newTransform.m_isInvDirty = true;

	return newTransform;
}

void Transform3D::operator*=(const Transform3D& other)
{
	ApplyTransform(other);
}

void Transform3D::Serialize(SerializedObject& datas) const
{
	glm::vec3 r = m_right;
	datas.AddPrivateElement("Right", static_cast<glm::vec3 const*>(&r));
	glm::vec3 u = m_up;
	datas.AddPrivateElement("Up", static_cast<glm::vec3 const*>(&u));
	glm::vec3 f = m_forward;
	datas.AddPrivateElement("Forward", static_cast<glm::vec3 const*>(&f));
	//glm::vec4 quat = { m_rotationQuat.x,m_rotationQuat.y,m_rotationQuat.z,m_rotationQuat.w };
	//datas.AddPrivateElement("m_rotationQuat", static_cast<glm::vec4 const*>(&quat));

	datas.SetType("Transform3D");
	glm::vec3 pos = m_position;
	datas.AddPublicElement("Position", static_cast<glm::vec3 const*>(&pos));
	glm::vec3 rot = { GetRotationDeg().x,GetRotationDeg().y,GetRotationDeg().z };
	datas.AddPublicElement("Rotation", static_cast<glm::vec3 const*>(&rot));
	glm::vec3 scale = m_scale;
	datas.AddPublicElement("Scale", static_cast<glm::vec3 const*>(&scale));
}

void Transform3D::Deserialize(SerializedObject const& datas)
{
	glm::vec3 r = {};
	datas.GetPrivateElement("Right",&r);
	m_right = { r,1.0f };
	glm::vec3 u = {};
	datas.GetPrivateElement("Up", &u);
	m_up = { u,1.0f };
	glm::vec3 f = {};
	datas.GetPrivateElement("Forward", &f);
	m_forward = { f,1.0f };
	//glm::vec4 quat ;
	//datas.GetPrivateElement("m_rotationQuat", static_cast<glm::vec4*>(&quat));
	//m_rotationQuat = { quat.x,quat.y,quat.z,quat.w };

	glm::vec3 pos = {};
	datas.GetPublicElement("Position", &pos);
	SetPosition({ pos,1.0f });

	glm::vec3 rot = {};
	datas.GetPublicElement("Rotation", &rot);
	SetRotationDeg(rot.x, rot.y, rot.z);

	glm::vec3 scale = { 1.0f,1.0f,1.0f };
	datas.GetPublicElement("Scale", &scale);
	SetScale({ scale,1.0f });
}


ISerializable* Transform3D::CreateInstance()
{
	return std::make_unique<Transform3D>().release();
};