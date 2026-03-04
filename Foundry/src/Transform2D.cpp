#include "../include/Transform2D.h"

#include <cmath>

Transform2D::Transform2D(
	float _x,		     float _y,
	float _scaleX,	     float _scaleY,
	float _theta,
	bool _statism) :
	m_isStatic(_statism),
	m_isDirty(false)
{
	m_scale = mat3(
		_scaleX, 0, 0, 
		0, _scaleY, 0,
		0,    0,    1
	);
	m_rotation = mat3(
		cos(_theta), -sin(_theta),	0,
		sin(_theta), cos(_theta),	0,
		0,				0,			1
	);
	m_position = mat3(
		1, 0, _x,
		0, 1, _y,
		0, 0,		1
	);
	m_transformationMatrix = mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	);

	Update();
}

Transform2D::~Transform2D() {}


Transform2D::Transform2D(Transform2D const& other)
{
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;
	m_position = other.m_position;

	m_transformationMatrix = (m_scale * m_rotation) * m_position;

	m_isStatic = other.m_isStatic;
	m_isDirty = other.m_isDirty;
}

Transform2D& Transform2D::operator=(Transform2D const& other)
{
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;
	m_position = other.m_position;

	m_transformationMatrix = (m_scale * m_rotation) * m_position;

	m_isStatic = other.m_isStatic;
	m_isDirty = other.m_isDirty;

	return *this;
}


Transform2D::Transform2D(Transform2D&& other) noexcept
{
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;
	m_position = other.m_position;

	m_transformationMatrix = (m_scale * m_rotation) * m_position;

	m_isStatic = other.m_isStatic;
	m_isDirty = other.m_isDirty;
}
Transform2D& Transform2D::operator=(Transform2D&& other) noexcept
{
	m_scale = other.m_scale;
	m_rotation = other.m_rotation;
	m_position = other.m_position;

	m_transformationMatrix = (m_scale * m_rotation) * m_position;

	m_isStatic = other.m_isStatic;
	m_isDirty = other.m_isDirty;

	return *this;
}


Transform2D Transform2D::operator*(Transform2D const& other) const
{
	Transform2D toReturn  = {};
	toReturn.m_scale      = m_scale      * other.m_scale;
	toReturn.m_rotation   = m_rotation   * other.m_rotation;
	toReturn.m_position = m_position * other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix * other.m_transformationMatrix;

	toReturn.m_isDirty = false;
	toReturn.m_isStatic = false;

	return toReturn;
}
Transform2D& Transform2D::operator*=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_scale      *= other.m_scale;
	m_rotation   *= other.m_rotation;
	m_position *= other.m_position;

	m_transformationMatrix *= other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator+(Transform2D const& other) const
{
	Transform2D toReturn  = {};
	toReturn.m_scale      = m_scale      + other.m_scale;
	toReturn.m_rotation = m_rotation + other.m_rotation;
	toReturn.m_position = m_position + other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix + other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator+=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_scale += other.m_scale;
	m_rotation += other.m_rotation;
	m_position += other.m_position;

	m_transformationMatrix += other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator-(Transform2D const& other) const
{
	Transform2D toReturn  = {};

	toReturn.m_scale = m_scale - other.m_scale;
	toReturn.m_rotation = m_rotation - other.m_rotation;
	toReturn.m_position = m_position - other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix - other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator-=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_scale -= other.m_scale;
	m_rotation -= other.m_rotation;
	m_position -= other.m_position;

	m_transformationMatrix -= other.m_transformationMatrix;

	return *this;
}

Transform2D Transform2D::operator/(Transform2D const& other) const
{
	Transform2D toReturn = {};

	toReturn.m_scale = m_scale / other.m_scale;
	toReturn.m_rotation = m_rotation / other.m_rotation;
	toReturn.m_position = m_position / other.m_position;

	toReturn.m_transformationMatrix = m_transformationMatrix / other.m_transformationMatrix;

	return toReturn;
}
Transform2D& Transform2D::operator/=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_scale /= other.m_scale;
	m_rotation /= other.m_rotation;
	m_position /= other.m_position;

	m_transformationMatrix /= other.m_transformationMatrix;

	return *this;
}


void Transform2D::SetScale(vec2 _scale)
{
	if (m_isStatic) return;

	m_scale[0][0] = _scale.x;
	m_scale[1][1] = _scale.y;

	m_isDirty = true;
}
void Transform2D::SetScale(float _x, float _y)
{
	if (m_isStatic) return;

	if (_x < 0.f || _y < 0.f)
		return;

	m_scale[0][0] = _x;
	m_scale[1][1] = _y;

	m_isDirty = true;
}
uvec2 Transform2D::GetScale() const
{
	return { m_scale[0][0], m_scale[1][1] };
}

void Transform2D::SetRotation(float _theta)
{
	if (m_isStatic) return;

	m_rotation[0][0] = cos(_theta);
	m_rotation[0][1] = -sin(_theta);
	m_rotation[1][0] = sin(_theta);
	m_rotation[1][1] = cos(_theta);

	m_isDirty = true;
}
mat2 Transform2D::GetRotationMatrix() const
{
	return mat2(
		m_rotation[0][0], m_rotation[0][1],
		m_rotation[1][0], m_rotation[1][1]
	);
}

void Transform2D::SetPosition(float _u, float _v)
{
	if (m_isStatic) return;

	m_position[0][2] = _u;
	m_position[1][2] = _v;

	m_isDirty = true;
}
void Transform2D::SetPosition(vec2 _translation)
{
	if (m_isStatic) return;

	m_position[0][2] = _translation.x;
	m_position[1][2] = _translation.y;

	m_isDirty = true;
}
vec2 Transform2D::GetPosition() const
{
	return { m_position[0][2], m_position[1][2] };
}

mat3 Transform2D::GetTransformationMatrix() const
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

	m_transformationMatrix = mat3(
		1, 0, 0,
		0, 1, 0,
		0, 0, 1 
	);

	m_transformationMatrix = (m_scale * m_rotation) * m_position;

	m_isDirty = false;
}