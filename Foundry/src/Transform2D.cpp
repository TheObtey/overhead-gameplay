#include "Transform2D.h"

#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

Transform2D::Transform2D(
	float _x,		     float _y,
	float _scaleX,	     float _scaleY,
	float _theta,
	bool _statism) :
	m_isStatic(_statism),
	m_scale({ _scaleX, _scaleY, 1.0f }),
	m_shear({ 0.0f, 0.0f, 0.0f }),
	m_rotation({ 0.0f,0.0f,0.0f } ),
	m_position(_x, _y, 0.0f),
	m_isDirty(false)
{
	m_transformationMatrix = glm::mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);

	Update();
}

Transform2D::~Transform2D() {}


Transform2D::Transform2D(Transform2D const& _other)
{
	m_scale = _other.m_scale;
	m_rotation = _other.m_rotation;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;
}

Transform2D& Transform2D::operator=(Transform2D const& _other)
{
	m_scale = _other.m_scale;
	m_rotation = _other.m_rotation;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;

	return *this;
}


Transform2D::Transform2D(Transform2D&& _other) noexcept
{
	m_scale = _other.m_scale;
	m_rotation = _other.m_rotation;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;
}
Transform2D& Transform2D::operator=(Transform2D&& _other) noexcept
{
	m_scale = _other.m_scale;
	m_rotation = _other.m_rotation;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;

	return *this;
}


Transform2D Transform2D::operator*(Transform2D const& _other) const
{
	Transform2D toReturn  = {};
	toReturn.m_scale      = m_scale      * _other.m_scale;
	toReturn.m_rotation   = m_rotation   * _other.m_rotation;
	toReturn.m_position = m_position * _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix * _other.m_transformationMatrix;

	toReturn.m_isDirty = false;
	toReturn.m_isStatic = false;

	return toReturn;
}
Transform2D& Transform2D::operator*=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale      *= _other.m_scale;
	m_rotation   *= _other.m_rotation;
	m_position *= _other.m_position;

	m_transformationMatrix *= _other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator+(Transform2D const& _other) const
{
	Transform2D toReturn  = {};
	toReturn.m_scale      = m_scale      + _other.m_scale;
	toReturn.m_rotation = m_rotation + _other.m_rotation;
	toReturn.m_position = m_position + _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix + _other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator+=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale += _other.m_scale;
	m_rotation += _other.m_rotation;
	m_position += _other.m_position;

	m_transformationMatrix += _other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator-(Transform2D const& _other) const
{
	Transform2D toReturn  = {};

	toReturn.m_scale = m_scale - _other.m_scale;
	toReturn.m_rotation = m_rotation - _other.m_rotation;
	toReturn.m_position = m_position - _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix - _other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator-=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale -= _other.m_scale;
	m_rotation -= _other.m_rotation;
	m_position -= _other.m_position;

	m_transformationMatrix -= _other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator/(Transform2D const& _other) const
{
	Transform2D toReturn = {};

	toReturn.m_scale = m_scale / _other.m_scale;
	toReturn.m_rotation = m_rotation / _other.m_rotation;
	toReturn.m_position = m_position / _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix / _other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator/=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale /= _other.m_scale;
	m_rotation /= _other.m_rotation;
	m_position /= _other.m_position;

	m_transformationMatrix /= _other.m_transformationMatrix;

	return *this;
}


void Transform2D::SetShearing(glm::vec2 _shear)
{
	m_shear = { _shear.x, _shear.y, 1.0f };

	m_isDirty = true;
}
void Transform2D::SetShearing(float _shearX, float _shearY)
{
	m_shear = { _shearX, _shearY, 1.0f };

	m_isDirty = true;
}
void Transform2D::SetShearingOnAxis(Axis _axis, float _shear)
{
	m_shear[static_cast<int>(_axis)] = _shear;

	m_isDirty = true;
}
glm::vec2 Transform2D::GetShearing() const
{
	return { m_shear.x, m_shear.y };
}

void Transform2D::SetMirroringOnAxis(Axis _axis)
{
	m_position[static_cast<int>(_axis)] *= -1.0f;

	m_isDirty = true;
}

void Transform2D::SetScale(glm::vec2 _scale)
{
	if (m_isStatic) return;

	m_scale.x = _scale.x;
	m_scale.y = _scale.y;
	m_scale.z = 1.0f;

	m_isDirty = true;
}
void Transform2D::SetScale(float _x, float _y)
{
	if (m_isStatic) return;

	if (_x < 0.f || _y < 0.f)
		return;

	m_scale.x = _x;
	m_scale.y = _y;
	m_scale.z = 1.0f;

	m_isDirty = true;
}
glm::uvec2 Transform2D::GetScale() const
{
	return { m_scale.x, m_scale.y };
}

void Transform2D::SetRotation(float _rotX, float _rotY)
{
	if (m_isStatic) return;

	m_rotation.x = _rotX;
	m_rotation.y = _rotY;
	m_rotation.z = 1.0f;

	m_isDirty = true;
}
glm::vec2 Transform2D::GetRotation() const
{
	return { m_rotation.x, m_rotation.y };
}

void Transform2D::SetPosition(float _u, float _v)
{
	if (m_isStatic) return;

	m_position.x = _u;
	m_position.y = _v;

	m_isDirty = true;
}
void Transform2D::SetPosition(glm::vec2 _position)
{
	if (m_isStatic) return;

	m_position.x = _position.x;
	m_position.y = _position.y;

	m_isDirty = true;
}
glm::vec2 Transform2D::GetPosition() const
{
	return { m_position.x, m_position.y };
}

glm::mat3 Transform2D::GetTransformationMatrix() const
{
	return m_transformationMatrix;
}

void Transform2D::SetStatism(bool _statism)
{
	m_isStatic = _statism;
}
bool Transform2D::IsStatic() const
{
	return m_isStatic;
}

void Transform2D::Update()
{
	if (m_isStatic) return;

	m_transformationMatrix = glm::mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1 
	);

	glm::mat3 S = glm::mat3(
		m_scale.x, 0, 0,
		0, m_scale.y, 0,
		0, 0, 1
	);

	glm::mat3 Sh = glm::mat3(
		1, m_shear.x, 0,
		m_shear.y, 1, 0,
		0, 0, 1
	);

	glm::mat3 R = glm::toMat3(glm::quat(m_rotation));

	glm::mat3 T = glm::mat3(
		1, 0, m_position.x,
		0, 1, m_position.y,
		0, 0, 1
	);

	m_transformationMatrix = ((S * Sh) * R) * T;

	m_isDirty = false;
}