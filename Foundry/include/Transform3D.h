#ifndef FOUNDRY_TRANSFORM3D__H_
#define FOUNDRY_TRANSFORM3D__H_

#include "Serialization/ISerializable.h"
#include "Registries/AutomaticRegister.hpp"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

/// /////////////////////////////////////////////////////////////////////////////////
/// Classe Transform
/// /////////////////////////////////////////////////////////////////////////////////


class Transform3D : public ISerializable
{
public:

	Transform3D();
	~Transform3D() = default;

	Transform3D operator*(Transform3D& other);
	void operator*=(Transform3D& other);

	const glm::vec4& GetPosition() const;
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	const glm::mat4& GetMatrixRotation() const;
	const glm::mat4& GetInverseMatrixRotation() const;
	const glm::vec4 GetRotation() const;
	const glm::quat& GetRotationQuat() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	float GetMaxScale() const;
	float GetMinScale() const;
	const glm::vec4& GetScale() const;
	const glm::vec4& GetRight() const;
	const glm::vec4& GetUp() const;
	const glm::vec4& GetForward() const;

	glm::mat4& GetMatrix();
	glm::mat4& GetInvMatrix();

	bool GetDirty() const { return m_isDirty; }

	void SetPosition(glm::vec4 pos);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetRotation(glm::vec4 rot);
	void SetRotationQuat(glm::quat rot);
	void SetYaw(float yaw);
	void SetPitch(float pitch);
	void SetRoll(float roll);
	void SetScale(glm::vec4 scale);

	void AddPosition(glm::vec4 pos);
	void AddX(float x);
	void AddY(float y);
	void AddZ(float z);
	void AddRotation(glm::vec4 rot);
	void AddYaw(float yaw);
	void AddPitch(float pitch);
	void AddRoll(float roll);
	void AddScale(glm::vec4 scale);

	void ApplyTransform(Transform3D& Transform3D); 

	void Update();

	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	static ISerializable* CreateInstance();

private:
	glm::vec4 m_position;
	glm::vec4 m_rotation;
	glm::vec4 m_scale;

	glm::vec4 m_right;
	glm::vec4 m_up;
	glm::vec4 m_forward;

	glm::quat m_rotationQuat{ 1, 0, 0, 0 };
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_invRotationMatrix;
	glm::mat4 m_transform{ 1.0f };
	glm::mat4 m_invTransform;

	bool m_isDirty = true;
	bool m_isRotationDirty = true;
	bool m_isInvDirty = true;

	void UpdateRotationMatrix();
	void UpdateTransform();
	void UpdateInvTransform();
};

REGISTER_ISERIALIZABLE(Transform3D, Transform3D::CreateInstance);


#endif //FOUNDRY_TRANSFORM3D__H_
