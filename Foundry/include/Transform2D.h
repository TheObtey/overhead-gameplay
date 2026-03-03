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
		float _x = 0.0f, 
		float _y = 0.0f, 
		float _width = 0.0f,
		float _height = 0.0f,
		float _scaleX = 1.0f, 
		float _scaleY = 1.0f, 
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

	void  SetDimensions(vec2 _dim);
	void  SetDimensions(float _width, float _height);
	uvec2  GetDimensions() const;
		  
	void  SetScale(vec2 _scale);
	void  SetScale(float _width, float _height);
	uvec2 GetScale() const;

	void  AddTheta(float _theta);
	void  SetTheta(float _theta);
	float GetTheta() const;	

	void  SetStatism(bool _statism);
	bool  IsStatic() const;

	void SetParent(Transform2D& _parent);
	Transform2D* GetParent();

private:
	vec2		 m_position;
	uvec2		 m_dimensions;
	uvec2		 m_scale;
	float		 m_theta;
	bool		 m_isStatic;
	
	Transform2D* m_pParent;
};


#endif