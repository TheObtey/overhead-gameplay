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
	// Gameplay ------------------------------------------------------------------

	class Proxy;

	Node3D(std::string const& name);
	~Node3D() override = default;

	glm::vec3 GetPosition() const				{ return m_transform.GetPosition(); }
	float GetX() const							{ return m_transform.GetX(); }
	float GetY() const							{ return m_transform.GetY(); }
	float GetZ() const							{ return m_transform.GetZ(); }
	glm::mat4 GetMatrixRotation() const			{ return m_transform.GetMatrixRotation(); }
	//const glm::mat4& GetInverseMatrixRotation() { return m_transform.GetInverseMatrixRotation(); }

	//Euler angles
	glm::vec3 GetRotation() const				{ return m_transform.GetRotation(); }
	float GetYaw() const						{ return m_transform.GetYaw(); }
	float GetPitch() const						{ return m_transform.GetPitch(); }
	float GetRoll() const						{ return m_transform.GetRoll(); }
	float GetMaxScale() const					{ return m_transform.GetMaxScale(); }
	float GetMinScale() const					{ return m_transform.GetMinScale(); }
	glm::vec3 GetScale() const					{ return m_transform.GetScale(); }
	glm::vec3 GetRight() const					{ return m_transform.GetRight(); }
	glm::vec3 GetUp() const						{ return m_transform.GetUp(); }
	glm::vec3 GetForward() const				{ return m_transform.GetForward(); }

	void SetPosition(glm::vec3 const pos)		{ m_transform.SetPosition(glm::vec4(pos, 1.0f)); }
	void SetX(float const x)					{ m_transform.SetX(x); }
	void SetY(float const y)					{ m_transform.SetY(y); }
	void SetZ(float const z)					{ m_transform.SetZ(z); }
	void SetRotation(glm::vec3 const rot)		{ m_transform.SetRotation(glm::vec4(rot, 1.0f)); }
	void SetYaw(float const yaw)				{ m_transform.SetYaw(yaw); }
	void SetPitch(float const pitch)			{ m_transform.SetPitch(pitch); }
	void SetRoll(float const roll)				{ m_transform.SetRoll(roll); }
	void SetScale(glm::vec3 const scale)		{ m_transform.SetScale(glm::vec4(scale, 1.0f)); }

	glm::mat4x4 const& GetWorldMatrix() const;
	glm::vec3 GetWorldPosition() const;
	glm::vec3 GetWorldScale() const;
	glm::vec3 GetWorldRotation() const;

	void SetWorldPosition(glm::vec3 const& worldPos);
	void SetWorldScale(glm::vec3 const& worldScale);
	void SetWorldRotation(glm::vec3 const& worldRot);
												
	void AddPosition(glm::vec3 const pos)		{ m_transform.AddPosition(glm::vec4(pos, 1.0f)); }
	void AddX(float const x)					{ m_transform.AddX(x); }
	void AddY(float const y)					{ m_transform.AddY(y); }
	void AddZ(float const z)					{ m_transform.AddZ(z); }
	void AddRotation(glm::vec3 const rot)		{ m_transform.AddRotation(glm::vec4(rot, 1.0f)); }
	void AddYaw(float const yaw)				{ m_transform.AddYaw(yaw); }
	void AddPitch(float const pitch)			{ m_transform.AddPitch(pitch); }
	void AddRoll(float const roll)				{ m_transform.AddRoll(roll); }
	void AddScale(glm::vec3 const scale)		{ m_transform.AddScale(glm::vec4(scale, 1.0f)); }

	virtual void Reparent(Node& newParent, bool keepGlobalTransform = true) override;
	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;

	// Engine ------------------------------------------------------------------

	glm::mat4& GetMatrix()						{ return m_transform.GetMatrix(); }
	glm::mat4& GetInvMatrix()					{ return m_transform.GetInvMatrix(); }

	virtual void OnUpdate(double delta) override;

	static ISerializable* CreateInstance();

private:
	void CheckParentTransform();
	void UpdateWorldTransform();
	void UpdateLocalTransform();

private:
	Transform3D m_transform;

	glm::mat4x4 m_worldTransform{ 1.0f };
	glm::vec4 m_worldPosition{0.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 m_worldScale{ 1.0f };
	glm::quat m_worldRotation{ 0.0f,0.0f,0.0f,1.0f };

	bool m_isParentNode3D = false;
	bool m_worldDirty : 1 = true;
	bool m_localDirty : 1 = true;
};

inline REGISTER_ISERIALIZABLE(Node3D, Node3D::CreateInstance);

#include "Scripting/Proxies/Node3DProxy.inl"

#endif // !FOUNDRY_NODE3D__H_
