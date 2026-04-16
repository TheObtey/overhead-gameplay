#ifndef FOUNDRY_NODE3D__H_
#define FOUNDRY_NODE3D__H_

#include "Node.h"
#include "Transform3D.h"

/*
Base class of every 3D object
*/
class Node3D : public Node
{
public: 

	////////////////////////////////////////////////////////////
	// Engine

	class Proxy;

	Node3D(std::string const& name);
	~Node3D() override = default;
	//const glm::mat4& GetInverseMatrixRotation() { return m_transform.GetInverseMatrixRotation(); }
												
	virtual void Reparent(Node& newParent, bool keepGlobalTransform = true) override;
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	virtual void OnUpdate(double delta) override;
	static ISerializable* CreateInstance();

	glm::mat4 const& GetMatrix() const				{ return m_transform.GetMatrix(); }

	// Engine
	////////////////////////////////////////////////////////////	 
	

	// =========== Getters ===========

	glm::vec3 GetPosition() const					{ return m_transform.GetPosition(); }
	float GetX() const								{ return m_transform.GetX(); }
	float GetY() const								{ return m_transform.GetY(); }
	float GetZ() const								{ return m_transform.GetZ(); }
	glm::mat4 GetMatrixRotation() const				{ return m_transform.GetMatrixRotation(); }
	//const glm::mat4& GetInverseMatrixRotation()	{ return m_transform.GetInverseMatrixRotation(); }

	EulerAngles GetLocalRotationRad() const			{ return m_transform.GetRotationRad(); }
	EulerAngles GetLocalRotationDeg() const			{ return m_transform.GetRotationDeg(); }
	// { w, x, y, z }
	const glm::quat& GetLocalRotationQuat() const	{ return m_transform.GetRotationQuat(); }
	// -- Angle around Y axis in Degrees
	float GetLocalYaw() const						{ return m_transform.GetYaw(); }
	// -- Angle around X axis in Degrees
	float GetLocalPitch() const						{ return m_transform.GetPitch(); }
	// -- Angle around Z axis in Degrees
	float GetLocalRoll() const						{ return m_transform.GetRoll(); }
	float GetMaxScale() const						{ return m_transform.GetMaxScale(); }
	float GetMinScale() const						{ return m_transform.GetMinScale(); }
	glm::vec3 GetScale() const						{ return m_transform.GetScale(); }
	glm::vec3 GetLocalRight() const					{ return m_transform.GetRight(); }
	glm::vec3 GetLocalUp() const					{ return m_transform.GetUp(); }
	glm::vec3 GetLocalForward() const				{ return m_transform.GetForward(); }

	glm::mat4x4 const& GetWorldMatrix() const;
	glm::vec3 GetWorldPosition() const;
	glm::vec3 GetWorldScale() const;
	// Pitch, Yaw, Roll  / x, y, z
	// -- Angles in Degrees 
	glm::vec3 GetWorldRotation() const;
	// { w, x, y, z }
	glm::quat const& GetWorldRotationQuaternion() const;


	// =========== Setters ===========

	void SetLocalPosition(glm::vec3 const pos)		{ m_transform.SetPosition(glm::vec4(pos, 1.0f)); }
	void SetLocalX(float const x)					{ m_transform.SetX(x); }
	void SetLocalY(float const y)					{ m_transform.SetY(y); }
	void SetLocalZ(float const z)					{ m_transform.SetZ(z); }

	void SetLocalRotationDeg(float x, float y, float z)				{ m_transform.SetRotationDeg(x, y, z); }
	void SetLocalRotationDeg(glm::vec3 const& rotation)				{ m_transform.SetRotationDeg(rotation.x, rotation.y, rotation.z); }
	void SetLocalRotationRad(float pitch, float yaw, float roll)	{ m_transform.SetRotationRad(pitch, yaw, roll); }
	void SetLocalRotationRad(glm::vec3 const& rotation)				{ m_transform.SetRotationRad(rotation.x, rotation.y, rotation.z); }
	// { w, x, y, z }
	void SetLocalRotationQuat(glm::quat rot)						{ m_transform.SetRotationQuat(rot); }

	void SetScale(glm::vec3 const scale)			{ m_transform.SetScale(glm::vec4(scale, 1.0f)); }

	void SetWorldPosition(glm::vec3 const& worldPos);
	void SetWorldScale(glm::vec3 const& worldScale);
	// -- Angles in Degrees
	void SetWorldRotation(glm::vec3 const& worldRot);
	// { w, x, y, z }
	// -- Angles in Radians
	void SetWorldRotationQuaternion(glm::quat const& worldRotQuat);

	// =========== Adders ===========

	void AddScale(glm::vec3 const scale)			{ m_transform.AddScale(glm::vec4(scale, 1.0f)); }
	void AddLocalPosition(glm::vec3 const pos)		{ m_transform.AddPosition(glm::vec4(pos, 1.0f)); }
	void AddLocalX(float const x)					{ m_transform.AddX(x); }
	void AddLocalY(float const y)					{ m_transform.AddY(y); }
	void AddLocalZ(float const z)					{ m_transform.AddZ(z); }
	// -- Angles in Radians							
	void AddLocalRotation(glm::vec3 const rot)		{ m_transform.AddRotation(glm::vec4(rot, 1.0f)); }
	// -- Angles in Radians							
	void AddLocalYaw(float const yaw)				{ m_transform.AddYaw(yaw); }
	// -- Angles in Radians							
	void AddLocalPitch(float const pitch)			{ m_transform.AddPitch(pitch); }
	// -- Angles in Radians							
	void AddLocalRoll(float const roll)				{ m_transform.AddRoll(roll); }
	uptr<Node> Clone() override;

private:
	void CheckParentTransform();
	void UpdateWorldTransform();
	void UpdateLocalTransform();

protected:
	bool IsTransformDirty() const { return m_transform.GetDirty() || (m_isParentNode3D && static_cast<Node3D*>(m_pOwner)->m_localDirty); }
	virtual void AttachScriptDeserialize(uptr<LuaScriptInstance>& script);

protected:
	Transform3D m_transform {};

	glm::mat4x4 m_worldTransform{ 1.0f };
	glm::vec4 m_worldPosition{0.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 m_worldScale{ 1.0f };
	glm::quat m_worldRotation{ 0.0f,0.0f,0.0f,1.0f };

	bool m_isParentNode3D = false;
	bool m_worldDirty : 1 = true;
	bool m_localDirty : 1 = true;
};

REGISTER_ISERIALIZABLE(Node3D, Node3D::CreateInstance);

#include "Scripting/Proxies/Node3DProxy.inl"

#endif // !FOUNDRY_NODE3D__H_
