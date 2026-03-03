#include "../include/Transform2D.h"

#include <cmath>

Transform2D::Transform2D(
	float _x,
	float _y,
	float _width = 0.0f,
	float _height = 0.0f,
	float _scaleX,
	float _scaleY,
	float _theta,
	bool _statism) :
	m_position(_x, _y),
	m_dimensions(_width, _height),
	m_scale(_scaleX, _scaleY),
	m_theta(_theta),
	m_isStatic(_statism),
	m_pParent(nullptr) {}

Transform2D::~Transform2D() 
{
	m_pParent = nullptr;
}


Transform2D::Transform2D(Transform2D const& other)
{
	m_position   = other.m_position;
	m_dimensions = other.m_dimensions;
	m_scale      = other.m_scale;
	m_theta      = other.m_theta;
	m_isStatic   = other.m_isStatic;
	m_pParent    = other.m_pParent;
}

Transform2D& Transform2D::operator=(Transform2D const& other)
{
	m_position   = other.m_position;
	m_dimensions = other.m_dimensions;
	m_scale      = other.m_scale;
	m_theta      = other.m_theta;
	m_isStatic   = other.m_isStatic;
	m_pParent    = other.m_pParent;

	return *this;
}


Transform2D::Transform2D(Transform2D&& other) noexcept
{
	m_position   = other.m_position;
	m_dimensions = other.m_dimensions;
	m_scale      = other.m_scale;
	m_theta      = other.m_theta;
	m_isStatic   = other.m_isStatic;
	m_pParent    = other.m_pParent;
}
Transform2D& Transform2D::operator=(Transform2D&& other) noexcept
{
	m_position   = other.m_position;
	m_dimensions = other.m_dimensions;
	m_scale      = other.m_scale;
	m_theta      = other.m_theta;
	m_isStatic   = other.m_isStatic;
	m_pParent    = other.m_pParent;

	return *this;
}


Transform2D Transform2D::operator*(Transform2D const& other) const
{
	Transform2D toReturn  = {};
	toReturn.m_position   = m_position   * other.m_position;
	toReturn.m_dimensions = m_dimensions * other.m_dimensions;
	toReturn.m_scale      = m_scale      * other.m_scale;
	toReturn.m_theta      = m_theta      * other.m_theta;

	return toReturn;
}
Transform2D& Transform2D::operator*=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_position   *= other.m_position;
	m_dimensions *= other.m_dimensions;
	m_scale      *= other.m_scale;
	m_theta      *= other.m_theta;

	return *this;
}

Transform2D Transform2D::operator+(Transform2D const& other) const
{
	Transform2D toReturn  = {};
	toReturn.m_position   = m_position   + other.m_position;
	toReturn.m_dimensions = m_dimensions + other.m_dimensions;
	toReturn.m_scale      = m_scale      + other.m_scale;
	toReturn.m_theta      = m_theta      + other.m_theta;

	return toReturn;
}
Transform2D& Transform2D::operator+=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_position   += other.m_position;
	m_dimensions += other.m_dimensions;
	m_scale      += other.m_scale;
	m_theta      += other.m_theta;

	return *this;
}

Transform2D Transform2D::operator-(Transform2D const& other) const
{
	Transform2D toReturn  = {};
	toReturn.m_position   = m_position   - other.m_position;
	toReturn.m_dimensions = m_dimensions - other.m_dimensions;
	toReturn.m_scale      = m_scale      - other.m_scale;
	toReturn.m_theta      = m_theta      - other.m_theta;

	return toReturn;
}
Transform2D& Transform2D::operator-=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_position   -= other.m_position;
	m_dimensions -= other.m_dimensions;
	m_scale      -= other.m_scale;
	m_theta      -= other.m_theta;

	return *this;
}

Transform2D Transform2D::operator/(Transform2D const& other) const
{
	Transform2D toReturn = {};
	toReturn.m_position   = m_position   / other.m_position;
	toReturn.m_dimensions = m_dimensions / other.m_dimensions;
	toReturn.m_scale      = m_scale      / other.m_scale;
	toReturn.m_theta      = m_theta      / other.m_theta;

	return toReturn;
}
Transform2D& Transform2D::operator/=(Transform2D const& other)
{
	if (m_isStatic) return *this;

	m_position   /= other.m_position;
	m_dimensions /= other.m_dimensions;
	m_scale      /= other.m_scale;
	m_theta      /= other.m_theta;

	return *this;
}

////////////////////////////////////////////////////////////////////

void Transform2D::SetPosition(vec2 _pos)
{
	if (m_isStatic) return;

	m_position = _pos;
}
void Transform2D::SetPosition(float _x, float _y)
{
	if (m_isStatic) return;

	m_position.x = _x;
	m_position.y = _y;
}
vec2 Transform2D::GetPosition() const
{
	return m_position;
}

void Transform2D::SetDimensions(vec2 _dim)
{
	if (m_isStatic) return;

	m_position = _dim;
}
void Transform2D::SetDimensions(float _width, float _height)
{
	if (m_isStatic) return;

	m_dimensions.x = _width;
	m_dimensions.y = _height;
}
uvec2 Transform2D::GetDimensions() const
{
	return m_dimensions;
}

void Transform2D::SetScale(vec2 _scale)
{
	if (m_isStatic) return;

	m_scale = _scale;
}
void Transform2D::SetScale(float _width, float _height)
{
	if (m_isStatic) return;

	if (_width < 0.f || _height < 0.f)
		return;

	m_scale.x = _width;
	m_scale.y = _height;
}
uvec2 Transform2D::GetScale() const
{
	return m_scale;
}

void Transform2D::AddTheta(float _theta)
{
	if (m_isStatic) return;

	m_theta += _theta;

	if (m_theta > 180.0f)
		m_theta = -180 + (int)_theta % 180;
	else if (m_theta < -180.0f)
		m_theta = 180 - (int)_theta % 180;

	m_position.x = m_position.x * cos(m_theta) - m_position.y * sin(m_theta);
	m_position.y = m_position.x * sin(m_theta) + m_position.y * cos(m_theta);
}
void Transform2D::SetTheta(float _theta)
{
	if (m_isStatic) return;

	if (_theta < -180.0f)
		m_theta = -180 - (int)_theta % 180;
	else if (_theta > 180.0f)
		m_theta = -180 + (int)_theta % 180;
	else
		m_theta = _theta;

	m_position.x = m_position.x * cos(m_theta) - m_position.y * sin(m_theta);
	m_position.y = m_position.x * sin(m_theta) + m_position.y * cos(m_theta);
}
float Transform2D::GetTheta() const
{
	return m_theta;
}

void Transform2D::SetStatism(bool _statism)
{
	m_isStatic = _statism;
}
bool Transform2D::IsStatic() const
{
	return m_isStatic;
}

void Transform2D::SetParent(Transform2D& _parent)
{
	m_pParent = &_parent;
}
Transform2D* Transform2D::GetParent()
{
	return m_pParent;
}