#ifndef FOUNDRY_TRANSFORM2D__H_
#define FOUNDRY_TRANSFORM2D__H_

#include "Define.h"

#include <glm/glm.hpp>

/// <summary>
/// Describes a 2D element in a 2D space world.
/// Handles positionning, scaling, rotation and statism (parameters locked or unlocked).
/// </summary>
class Transform2D
{
public:
	enum class Axis : uint32
	{
		Y = 0,
		X = 1,
		Z = 2
	};

	Transform2D(
		float _x = 0.0f, float _y = 0.0f,
		float _scaleX = 1.0f, float _scaleY = 1.0f,
		bool _statism = false
	);
	~Transform2D();

	Transform2D(Transform2D const& other);
	Transform2D& operator=(Transform2D const& other);

	Transform2D(Transform2D&& other) noexcept;
	Transform2D& operator=(Transform2D&& other) noexcept;


	Transform2D  operator*(Transform2D const& other) const;
	Transform2D& operator*=(Transform2D const& other);

	Transform2D  operator+(Transform2D const& other) const;
	Transform2D& operator+=(Transform2D const& other);
	
	Transform2D  operator-(Transform2D const& other) const;
	Transform2D& operator-=(Transform2D const& other);

	Transform2D  operator/(Transform2D const& other) const;
	Transform2D& operator/=(Transform2D const& other);


	void				SetShearing(glm::vec2 const& _shear);
	void				SetShearing(float _u, float _v);
	void				SetShearingOnAxis(Axis _axis, float _shear);
	glm::vec2 const&	GetShearing() const;

	void				SetMirroringOnAxis(Axis _axis);

	void				SetScale(glm::vec2 const& _scale);
	void				SetScale(float _width, float _height);
	glm::vec2 const&	GetScale() const;

	void				SetRotation(float _theta);
	float				GetRotation() const;

	void				SetPosition(glm::vec2 const& _pos);
	void				SetPosition(float _x, float _y);
	glm::vec2 const&	GetPosition() const;

	glm::mat3 const&	GetTransformationMatrix() const;
	void				SetTransformationMatrix(glm::mat3 const& mat) { m_transformationMatrix = mat; }

	void				SetStatism(bool _statism);
	bool				IsStatic() const;

	void				Update();

	bool GetDirty() const;

private:
	bool m_isStatic = false;
	bool m_isDirty;

	glm::vec2 m_scale;
	glm::vec2 m_shear;
	float	  m_theta;
	glm::vec2 m_position;
	
	glm::mat3 m_transformationMatrix;
};


#endif
