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

// { pitch, yaw, roll } = { x, y, z }
struct EulerAngles 
{
	union
	{
		 struct { float x, y, z; };
		 struct { float pitch, yaw, roll; };
	};
};

class Transform3D : public ISerializable
{
public:

	Transform3D();
	~Transform3D() = default;

	Transform3D operator*(const Transform3D& other);
	void operator*=(const Transform3D& other);

	glm::vec4 const& GetPosition() const;
	float GetX() const;
	float GetY() const;
	float GetZ() const;
	glm::mat4 const& GetMatrixRotation() const;
	glm::mat4 const& GetInverseMatrixRotation() const;

	EulerAngles GetRotationRad() const;
	EulerAngles GetRotationDeg() const;
	glm::quat const& GetRotationQuat() const;
	float GetYaw() const;
	float GetPitch() const;
	float GetRoll() const;
	float GetMaxScale() const;
	float GetMinScale() const;
	glm::vec4 const& GetScale() const;
	glm::vec4 const& GetRight() const;
	glm::vec4 const& GetUp() const;
	glm::vec4 const& GetForward() const;
				   
	glm::mat4 const& GetMatrix() const;
	//glm::mat4& GetInvMatrix();

	bool GetDirty() const { return m_isDirty; }

	void SetPosition(const glm::vec4& pos);
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetRotationDeg(float x, float y, float z);
	void SetRotationRad(float pitch, float yaw, float roll);
	void SetRotationQuat(const glm::quat& rot);

	void SetScale(const glm::vec4& scale);

	void AddPosition(const glm::vec4& pos);
	void AddX(float x);
	void AddY(float y);
	void AddZ(float z);
	void AddRotation(const glm::vec4& rot);
	void AddYaw(float yaw);
	void AddPitch(float pitch);
	void AddRoll(float roll);
	void AddScale(const glm::vec4& scale);

	void ApplyTransform(const Transform3D& Transform3D); 

	void Update();

	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	static ISerializable* CreateInstance();

private:
	glm::vec4 m_position;
	glm::vec4 m_scale;

	glm::vec4 m_right;
	glm::vec4 m_up;
	glm::vec4 m_forward;

	glm::quat m_rotationQuat{ 1, 0, 0, 0 }; // w, x, y, z
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
