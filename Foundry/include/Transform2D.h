#ifndef ENGINE_TRANSFORM2D__H_
#define ENGINE_TRANSFORM2D__H_

#include <glm/glm.hpp>
using namespace glm;

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


	void  SetPosition(vec2 _pos);
	void  SetPosition(float _x, float _y);
	vec2  GetPosition() const;
		  
	void  SetScale(vec2 _scale);
	void  SetScale(float _width, float _height);
	uvec2 GetScale() const;

	void  SetRotation(float _theta);
	mat2  GetRotationMatrix() const;

	mat3 GetTransformationMatrix() const;

	void  SetStatism(bool _statism);
	bool  IsStatic() const;

	void Update();

private:
	mat3 m_scale;
	mat3 m_rotation;
	mat3 m_position;

	mat3 m_transformationMatrix;

	bool m_isDirty;

	bool m_isStatic;
};


#endif