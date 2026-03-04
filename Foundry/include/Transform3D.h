#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

/// /////////////////////////////////////////////////////////////////////////////////
/// Classe Transform
/// /////////////////////////////////////////////////////////////////////////////////


class Transform3D
{
public:

	Transform3D();
	~Transform3D();

	Transform3D operator*(Transform3D& other);

	const glm::vec3& GetPosition() const;
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	const glm::mat4& GetMatrixRotation();
	const glm::mat4& GetInverseMatrixRotation();
	const glm::vec3& GetRotation() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	float GetMaxScale() const;
	float GetMinScale() const;
	const glm::vec3& GetScale() const;
	const glm::vec3& GetRight();
	const glm::vec3& GetUp();
	const glm::vec3& GetForward();

	glm::mat4& GetMatrix();
	glm::mat4& GetInvMatrix();

	bool GetDirty() { return m_isDirty; }

	void SetPosition(glm::vec3 pos);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetRotation(glm::vec3 rot);
	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetRoll(float roll);
	void SetScale(glm::vec3 scale);

	void AddPosition(glm::vec3 pos);
	void AddX(float x);
	void AddY(float y);
	void AddZ(float z);
	void AddRotation(glm::vec3 rot);
	void AddYaw(float yaw);
	void AddPitch(float pitch);
	void AddRoll(float roll);
	void AddScale(glm::vec3 scale);

	void ApplyTransform(Transform3D& Transform3D); 

	void Update();

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_forward;

	glm::mat4 m_rotationMatrix;
	glm::mat4 m_invRotationMatrix;
	glm::mat4 m_transform;
	glm::mat4 m_invTransform;

	bool m_isDirty;
	bool m_isRotationDirty;
	bool m_isInvDirty;

	void UpdateRotationMatrix();
	void UpdateTransform();
	void UpdateInvTransform();
};