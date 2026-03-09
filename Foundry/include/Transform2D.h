#ifndef FOUNDRY_TRANSFORM2D__H_
#define FOUNDRY_TRANSFORM2D__H_

#include <glm/glm.hpp>

/// <summary>
/// Describes a 2D element in a 2D space world.
/// Handles positionning, scaling, rotation and statism (parameters locked or unlocked).
/// </summary>
class Transform2D
{
public:
	enum class Axis : unsigned int
	{
		X = 0,
		Y = 1,
		Z = 2
	};

	Transform2D(
		float _x = 0.0f, float _y = 0.0f,
		float _scaleX = 1.0f, float _scaleY = 1.0f,
		float _theta = 0.0f,
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


	void		SetShearing(glm::vec2 _shear);
	void		SetShearing(float _u, float _v);
	void		SetShearingOnAxis(Axis _axis, float _shear);
	glm::vec2	GetShearing() const;

	void		SetMirroringOnAxis(Axis _axis);

	void		SetScale(glm::vec2 _scale);
	void		SetScale(float _width, float _height);
	glm::uvec2	GetScale() const;

	void		SetRotation(float _rotX, float _rotY);
	glm::vec2	GetRotation() const;

	void		SetPosition(glm::vec2 _pos);
	void		SetPosition(float _x, float _y);
	glm::vec2	GetPosition() const;

	glm::mat3	GetTransformationMatrix() const;
	void		SetTransformationMatrix();

	void		SetStatism(bool _statism);
	bool		IsStatic() const;

	void		Update();

private:
	bool m_isStatic;
	bool m_isDirty;

	glm::vec3 m_scale;
	glm::vec3 m_shear;
	glm::vec3 m_rotation;
	glm::vec3 m_position;
	
	glm::mat3 m_transformationMatrix;
};


#endif