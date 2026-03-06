#ifndef FOUNDRY_NODE3D__H_
#define FOUNDRY_NODE3D__H_

#include "Node.h"
#include "Transform3D.h"

/*
Base class of every 3D object
*/

struct FlagsDirty
{
	char worldDirty : 1 = true;
	char localDirty : 1 = true;
};

class Node3D : public Node
{
public: 
	// Gameplay ------------------------------------------------------------------

	class Proxy;

	Node3D(std::string const& name);
	~Node3D() {};

	const glm::vec4& GetPosition() const		{ return m_transform.GetPosition(); }
	float GetX() const							{ return m_transform.GetX(); }
	float GetY() const							{ return m_transform.GetY(); }
	float GetZ() const							{ return m_transform.GetZ(); }
	const glm::mat4& GetMatrixRotation()		{ return m_transform.GetMatrixRotation(); }
	//const glm::mat4& GetInverseMatrixRotation() { return m_transform.GetInverseMatrixRotation(); }
	const glm::vec4& GetRotation() const		{ return m_transform.GetRotation(); }
	float GetYaw() const						{ return m_transform.GetYaw(); }
	float GetPitch() const						{ return m_transform.GetPitch(); }
	float GetRoll() const						{ return m_transform.GetRoll(); }
	float GetMaxScale() const					{ return m_transform.GetMaxScale(); }
	float GetMinScale() const					{ return m_transform.GetMinScale(); }
	const glm::vec4& GetScale() const			{ return m_transform.GetScale(); }
	const glm::vec4& GetRight()					{ return m_transform.GetRight(); }
	const glm::vec4& GetUp()					{ return m_transform.GetUp(); }
	const glm::vec4& GetForward()				{ return m_transform.GetForward(); }

	void SetPosition(glm::vec4 pos)				{ m_transform.SetPosition(pos); }
	void SetX(float x)							{ m_transform.SetX(x); }
	void SetY(float y)							{ m_transform.SetY(y); }
	void SetZ(float z)							{ m_transform.SetZ(z); }
	void SetRotation(glm::vec4 rot)				{ m_transform.SetRotation(rot); }
	void SetYaw(float yaw)						{ m_transform.SetYaw(yaw); }
	void SetPitch(float pitch)					{ m_transform.SetPitch(pitch); }
	void SetRoll(float roll)					{ m_transform.SetRoll(roll); }
	void SetScale(glm::vec4 scale)				{ m_transform.SetScale(scale); }
												
	void AddPosition(glm::vec4 pos)				{ m_transform.AddPosition(pos); }
	void AddX(float x)							{ m_transform.AddX(x); }
	void AddY(float y)							{ m_transform.AddY(y); }
	void AddZ(float z)							{ m_transform.AddZ(z); }
	void AddRotation(glm::vec4 rot)				{ m_transform.AddRotation(rot); }
	void AddYaw(float yaw)						{ m_transform.AddYaw(yaw); }
	void AddPitch(float pitch)					{ m_transform.AddPitch(pitch); }
	void AddRoll(float roll)					{ m_transform.AddRoll(roll); }
	void AddScale(glm::vec4 scale)				{ m_transform.AddScale(scale); }

	virtual void Reparent(Node& newParent, bool keepGlobalTransform = true) override;

	// Engine ------------------------------------------------------------------

	bool GetDirty()								{ return m_transform.GetDirty(); }

	glm::mat4& GetMatrix()						{ return m_transform.GetMatrix(); }
	glm::mat4& GetInvMatrix()					{ return m_transform.GetInvMatrix(); }

	void ApplyTransform(Node3D& node) { m_transform.ApplyTransform(node.m_transform); }
	void ApplyTransform(Transform3D& Transform3D) { m_transform.ApplyTransform(Transform3D); }

	virtual void OnUpdate(float delta) override;
	glm::mat4x4& GetWorldMatrice();

	glm::vec4 const& GetWorldPosition();
	glm::vec4 const& GetWorldScale();
	glm::quat const& GetWorldRotation();

	void SetWorldPosition(glm::vec4 const& worldPos);
	void SetWorldScale(glm::vec4 const& worldScale);
	void SetWorldRotation(glm::vec4 const& worldRot);

private:

	Transform3D m_transform;
	glm::mat4x4 m_worldTransform{ 1.0f };

	glm::vec4 m_worldPosition{0.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 m_worldScale{ 1.0f };
	//glm::vec4 m_worldRotation{ 0.0f,0.0f,0.0f,1.0f };
	glm::quat m_worldRotation{ 0.0f,0.0f,0.0f,1.0f };

	bool m_isParentNode3D;
	bool m_isDirty;

	FlagsDirty m_flags;

	void CheckParentTransform();
	void UpdateWorldTransform();
	void UpdateLocalTransform();
};

#include "Scripting/Proxies/Node3DProxy.inl"

#endif // !FOUNDRY_NODE3D__H_
