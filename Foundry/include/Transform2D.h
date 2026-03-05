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

		  
	void		SetScale(glm::vec2 _scale);
	void		SetScale(float _width, float _height);
	glm::uvec2	GetScale() const;

	void		SetRotation(float _theta);
	glm::mat2	GetRotationMatrix() const;

	void		SetPosition(glm::vec2 _pos);
	void		SetPosition(float _x, float _y);
	glm::vec2	GetPosition() const;

	glm::mat3	GetTransformationMatrix() const;

	void		SetStatism(bool _statism);
	bool		IsStatic() const;

	void		Update();

private:
	bool m_isStatic;
	bool m_isDirty;
	
	glm::mat3 m_scale;
	glm::mat3 m_rotation;
	glm::mat3 m_position;
	
	glm::mat3 m_transformationMatrix;
};


#endif