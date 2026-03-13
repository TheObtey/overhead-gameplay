#include "Transform3D.h"
#include "MathUtils.h"

#include "Serialization/SerializeObject.hpp"

Transform3D::Transform3D() :
	m_position(0.0f, 0.0f, 0.0f, 1.0f),
	m_scale(1.0f, 1.0f, 1.0f, 1.0f),
	m_rotation(0.0f, 0.0f, 0.0f, 1.0f),
	m_isDirty(true),
	m_isInvDirty(true)
{
	Update();
}

void Transform3D::UpdateTransform()
{	
	if (m_isDirty == false) return;
	m_transform = Maths::Translate(m_position) * m_rotationMatrix *  Maths::Scale(m_scale);
	m_isDirty = false;
}

void Transform3D::UpdateRotationMatrix()
{
	if (m_isRotationDirty == false) return;

	//m_rotationMatrix = Maths::RotateYawPitchRoll(m_rotation);
	m_rotationMatrix = glm::mat4_cast(m_rotationQuat);
	m_invRotationMatrix = glm::inverse(m_rotationMatrix);

	m_right = { m_rotationMatrix[0][0], m_rotationMatrix[0][1], m_rotationMatrix[0][2], 1.0f };
	m_up = { m_rotationMatrix[1][0], m_rotationMatrix[1][1], m_rotationMatrix[1][2], 1.0f };
	m_forward = { m_rotationMatrix[2][0], m_rotationMatrix[2][1], m_rotationMatrix[2][2], 1.0f };

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

const glm::vec4 Transform3D::GetRotation() const
{
	return { glm::eulerAngles(m_rotationQuat),1.0f };
}
const glm::quat& Transform3D::GetRotationQuat() const
{
	return m_rotationQuat;
}

float Transform3D::GetYaw() const
{
	return m_rotation.x;
}

float Transform3D::GetPitch() const
{
	return m_rotation.y;
}

float Transform3D::GetRoll() const
{
	return m_rotation.z;
}

float Transform3D::GetMaxScale() const
{
	return std::max(m_scale.x, std::max(m_scale.y, m_scale.z));
}

float Transform3D::GetMinScale() const
{
	return std::min(m_scale.x, std::min(m_scale.y, m_scale.z));
}

const glm::vec4& Transform3D::GetScale() const
{
	return m_scale;
}

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

glm::mat4& Transform3D::GetMatrix()
{
	return m_transform;
}

glm::mat4& Transform3D::GetInvMatrix()
{
	return m_invTransform;
}

void Transform3D::SetPosition(glm::vec4 pos)
{
	m_position = pos;
	m_position.w = 1.0f;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::SetX(float x)
{
	m_position.x = x;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::SetY(float y)
{
	m_position.y = y;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::SetZ(float z)
{
	m_position.z = z;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::SetRotation(glm::vec4 rot)
{

	m_rotation = rot;
	m_rotation.w = 1.0f;
	m_rotationQuat = glm::quat_cast(Maths::RotateYawPitchRoll({rot.y,rot.x,rot.z,1.0f}));

	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}
void Transform3D::SetRotationQuat(glm::quat rot)
{
	m_rotationQuat = rot;
	m_rotation = { glm::eulerAngles(m_rotationQuat),1.0f };
	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}


void Transform3D::SetYaw(float yaw)
{
	m_rotation.y = yaw;
	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::SetPitch(float pitch)
{
	m_rotation.x = pitch;
	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::SetRoll(float roll)
{
	m_rotation.z = roll;
	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::SetScale(glm::vec4 scale)
{
	m_scale = scale;
	m_scale.w = 1.0f;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::AddPosition(glm::vec4 pos)
{
	m_position += pos;
	m_position.w = 1.0f;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::AddX(float x)
{
	m_position.x += x;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::AddY(float y)
{
	m_position.y += y;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::AddZ(float z)
{
	m_position.z += z;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::AddRotation(glm::vec4 rot)
{
	//m_rotation += rot;
	//m_rotation.w = 1.0f;


	glm::vec3 up = GetUp();
	m_rotationQuat = glm::normalize(glm::angleAxis(rot[1], up) * m_rotationQuat);
	glm::vec3 right = GetRight();
	m_rotationQuat = glm::normalize(glm::angleAxis(rot[0], right) * m_rotationQuat);
	glm::vec3 forward = GetForward();
	m_rotationQuat = glm::normalize(glm::angleAxis(rot[3], forward) * m_rotationQuat);


	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::AddYaw(float yaw)
{
	//m_rotation.x += yaw;

	glm::vec3 up = GetUp();
	m_rotationQuat = glm::normalize(glm::angleAxis(yaw, up) * m_rotationQuat);

	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::AddPitch(float pitch)
{
	//m_rotation.y += pitch;
	glm::vec3 right = GetRight();
	m_rotationQuat = glm::normalize(glm::angleAxis(pitch, right) * m_rotationQuat);

	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::AddRoll(float roll)
{
	//m_rotation.z += roll;
	glm::vec3 forward = m_forward;
	m_rotationQuat = glm::normalize(glm::angleAxis(roll, forward) * m_rotationQuat);

	m_isDirty = true;
	m_isInvDirty = true;
	m_isRotationDirty = true;
}

void Transform3D::AddScale(glm::vec4 scale)
{
	m_scale += scale;
	m_scale.w = 1.0f;
	m_isDirty = true;
	m_isInvDirty = true;
}

void Transform3D::ApplyTransform(Transform3D& transform)
{
	m_rotationMatrix = transform.GetMatrixRotation() * GetMatrixRotation();
	m_transform = transform.GetMatrix() * GetMatrix();
	m_isDirty = true;
	m_isRotationDirty = true;
	m_isInvDirty = true;
}

void Transform3D::Update()
{
	UpdateRotationMatrix();
	UpdateTransform();
	UpdateInvTransform();
}

Transform3D Transform3D::operator*(Transform3D& other)
{
	Transform3D newTransform;
	newTransform.m_position = m_position;
	newTransform.m_position.w = 1.0f;
	newTransform.m_transform = other.GetMatrix() * GetMatrix();
	newTransform.m_isDirty = true;
	newTransform.m_isRotationDirty = true;
	newTransform.m_isInvDirty = true;

	return newTransform;
}

void Transform3D::operator*=(Transform3D& other)
{
	ApplyTransform(other);
}

void Transform3D::Serialize(SerializedObject& datas) const
{
	glm::vec3 r = m_right;
	datas.AddPrivateElement("m_right", static_cast<glm::vec3 const*>(&r));
	glm::vec3 u = m_up;
	datas.AddPrivateElement("m_up", static_cast<glm::vec3 const*>(&u));
	glm::vec3 f = m_forward;
	datas.AddPrivateElement("m_forward", static_cast<glm::vec3 const*>(&f));
	//glm::vec4 quat = { m_rotationQuat.x,m_rotationQuat.y,m_rotationQuat.z,m_rotationQuat.w };
	//datas.AddPrivateElement("m_rotationQuat", static_cast<glm::vec4 const*>(&quat));

	datas.SetType("Transform3D");
	glm::vec3 pos = m_position;
	datas.AddPublicElement("m_position", static_cast<glm::vec3 const*>(&pos));
	glm::vec3 rot = GetRotation();
	rot *= pi_t<long double>; // rad to deg
	datas.AddPublicElement("m_rotation", static_cast<glm::vec3 const*>(&rot));
	glm::vec3 scale = m_scale;
	datas.AddPublicElement("m_scale", static_cast<glm::vec3 const*>(&scale));
}

void Transform3D::Deserialize(SerializedObject const& datas)
{
	glm::vec3 r = {};
	datas.GetPrivateElement("m_right", static_cast<glm::vec3*>(&r));
	m_right = { r,1.0f };
	glm::vec3 u = {};
	datas.GetPrivateElement("m_up", static_cast<glm::vec3*>(&u));
	m_up = { u,1.0f };
	glm::vec3 f = {};
	datas.GetPrivateElement("m_forward", static_cast<glm::vec3*>(&f));
	m_forward = { f,1.0f };
	//glm::vec4 quat ;
	//datas.GetPrivateElement("m_rotationQuat", static_cast<glm::vec4*>(&quat));
	//m_rotationQuat = { quat.x,quat.y,quat.z,quat.w };

	glm::vec3 pos = {};
	datas.GetPublicElement("m_position", static_cast<glm::vec3*>(&pos));
	SetPosition({ pos,1.0f });

	glm::vec3 rot = {};
	datas.GetPublicElement("m_rotation", static_cast<glm::vec3*>(&rot));
	rot *= pi_t<long double> / 180; // deg to rad
	SetRotation({ rot,1.0f });

	glm::vec3 scale = { 1.0f,1.0f,1.0f };
	datas.GetPublicElement("m_scale", static_cast<glm::vec3*>(&scale));
	SetScale({ scale,1.0f });
}


ISerializable* Transform3D::CreateInstance()
{
	return std::make_unique<Transform3D>().release();
};