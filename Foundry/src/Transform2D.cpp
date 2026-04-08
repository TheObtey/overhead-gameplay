#include "Transform2D.h"

#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>

Transform2D::Transform2D(
	float _x,		     float _y,
	float _scaleX,	     float _scaleY,
	bool _statism) :
	m_isStatic(_statism),
	m_scale({ _scaleX, _scaleY }),
	m_shear({ 0.0f, 0.0f }),
	m_theta(0.0f),
	m_position(_x, _y ),
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
	m_theta = _other.m_theta;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;
}

Transform2D& Transform2D::operator=(Transform2D const& _other)
{
	m_scale = _other.m_scale;
	m_theta = _other.m_theta;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;

	return *this;
}


Transform2D::Transform2D(Transform2D&& _other) noexcept
{
	m_scale = _other.m_scale;
	m_theta = _other.m_theta;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;
}
Transform2D& Transform2D::operator=(Transform2D&& _other) noexcept
{
	m_scale = _other.m_scale;
	m_theta = _other.m_theta;
	m_position = _other.m_position;

	m_isStatic = _other.m_isStatic;
	m_isDirty = true;

	return *this;
}


Transform2D Transform2D::operator*(Transform2D const& _other) const
{
	Transform2D toReturn  = {};
	toReturn.m_scale      = m_scale      * _other.m_scale;
	toReturn.m_theta   = m_theta   * _other.m_theta;
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
	m_theta   *= _other.m_theta;
	m_position *= _other.m_position;

	m_transformationMatrix *= _other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator+(Transform2D const& _other) const
{
	Transform2D toReturn  = {};
	toReturn.m_scale      = m_scale      + _other.m_scale;
	toReturn.m_theta = m_theta + _other.m_theta;
	toReturn.m_position = m_position + _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix + _other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator+=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale += _other.m_scale;
	m_theta += _other.m_theta;
	m_position += _other.m_position;

	m_transformationMatrix += _other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator-(Transform2D const& _other) const
{
	Transform2D toReturn  = {};

	toReturn.m_scale = m_scale - _other.m_scale;
	toReturn.m_theta = m_theta - _other.m_theta;
	toReturn.m_position = m_position - _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix - _other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator-=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale -= _other.m_scale;
	m_theta -= _other.m_theta;
	m_position -= _other.m_position;

	m_transformationMatrix -= _other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator/(Transform2D const& _other) const
{
	Transform2D toReturn = {};

	toReturn.m_scale = m_scale / _other.m_scale;
	toReturn.m_theta = m_theta / _other.m_theta;
	toReturn.m_position = m_position / _other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix / _other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator/=(Transform2D const& _other)
{
	if (m_isStatic) return *this;

	m_scale /= _other.m_scale;
	m_theta /= _other.m_theta;
	m_position /= _other.m_position;

	m_transformationMatrix /= _other.m_transformationMatrix;

	return *this;
}


void Transform2D::SetShearing(glm::vec2 const& _shear)
{
	m_shear = { _shear.x, _shear.y };

	m_isDirty = true;
}
void Transform2D::SetShearing(float const _shearX, float const _shearY)
{
	m_shear = { _shearX, _shearY };

	m_isDirty = true;
}
void Transform2D::SetShearingOnAxis(Axis _axis, float _shear)
{
	m_shear[static_cast<int32>(_axis)] = _shear;

	m_isDirty = true;
}
glm::vec2 const& Transform2D::GetShearing() const
{
	return m_shear;
}

void Transform2D::SetMirroringOnAxis(Axis _axis)
{
	m_position[static_cast<int32>(_axis)] *= -1.0f;

	m_isDirty = true;
}

void Transform2D::SetScale(glm::vec2 const& _scale)
{
	if (m_isStatic) return;

	m_scale.x = _scale.x;
	m_scale.y = _scale.y;

	m_isDirty = true;
}
void Transform2D::SetScale(float _x, float _y)
{
	if (m_isStatic) return;

	if (_x < 0.f || _y < 0.f)
		return;

	m_scale.x = _x;
	m_scale.y = _y;

	m_isDirty = true;
}
glm::vec2 const& Transform2D::GetScale() const
{
	return m_scale;
}

void Transform2D::SetRotation(float const _theta)
{
	if (m_isStatic) return;

	m_theta = _theta;

	m_isDirty = true;
}
float Transform2D::GetRotation() const
{
	return m_theta;
}

void Transform2D::SetPosition(float const _u, float const _v)
{
	if (m_isStatic) return;

	m_position.x = _u;
	m_position.y = _v;

	m_isDirty = true;
}
void Transform2D::SetPosition(glm::vec2 const& _position)
{
	if (m_isStatic) return;

	m_position.x = _position.x;
	m_position.y = _position.y;

	m_isDirty = true;
}
glm::vec2 const& Transform2D::GetPosition() const
{
	return m_position;
}

glm::mat3 const& Transform2D::GetTransformationMatrix() const
{
	return m_transformationMatrix;
}

void Transform2D::SetStatism(bool const _statism)
{
	m_isStatic = _statism;
}
bool Transform2D::IsStatic() const
{
	return m_isStatic;
}

void Transform2D::Update()
{
	if (m_isStatic || m_isDirty == false) return;

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

	float const c = cos(m_theta);
	float const s = sin(m_theta);

	glm::mat3 R = glm::mat3(
		c, -s, 0,
		s, c, 0,
		0, 0, 1
	);

	glm::mat3 T = glm::mat3(
		1, 0, 0,
		0, 1, 0,
		m_position.x, m_position.y, 1
	);

	m_transformationMatrix = T * R * Sh *  S;

	m_isDirty = false;
}

bool Transform2D::GetDirty() const
{
	return m_isDirty;
}
