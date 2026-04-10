#ifndef FOUNDRY_NODE2D__H_
#define FOUNDRY_NODE2D__H_

#include "Node.h"
#include "Transform2D.h"

class Node2D : public Node
{
public:
	class Proxy;

	Node2D(std::string const& name);
	~Node2D() override;

	virtual void Serialize(SerializedObject& datas) const override;
	virtual void Deserialize(SerializedObject const& datas) override;
	static ISerializable* CreateInstance();


	void				SetScale(glm::vec2 const& _scale);
	void				SetScale(float _width, float _height);
	glm::vec2 const&	GetScale() const;

	void				SetRotation(float _theta);
	float				GetRotation() const;

	void				SetShearing(glm::vec2 const& shear);
	glm::vec2 const&	GetShearing() const;

	void				SetPosition(glm::vec2 const& _pos);
	void				SetPosition(float _x, float _y);
	glm::vec2 const&	GetPosition() const;

	glm::mat3 const&	GetTransformationMatrix();

	void				SetMirroringOnAxis(Transform2D::Axis _axis);

	void				SetStatism(bool _statism);
	bool				IsStatic() const;

	void				SetWorldScale(glm::vec3 const& _worldScale);
	void				SetWorldRotationAngle(float _worldRot);
	void				SetWorldPosition(glm::vec3 const& _worldPos);

	glm::vec3			const& GetWorldPosition() const;
	glm::vec3			const& GetWorldScale() const;
	float				GetWorldRotation() const;

	virtual void		OnUpdate(double _delta) override;
	virtual void		Reparent(Node& _newParent, bool _keepGlobalTransform = true) override;

	uptr<Node> Clone() override;
protected:
	void AttachScriptDeserialize(uptr<LuaScriptInstance>& script) override;

private:
	void				CheckParentTransform(Node& node);
	void				UpdateLocal();
	void				UpdateWorld();

protected:
	Transform2D m_transform;

	glm::vec3 m_worldPosition {};
	float	  m_worldRotationAngle = 0.0f;
	glm::vec3 m_worldScale {1.0f};
	glm::mat3 m_worldTransform {1.0f};

	bool m_isParentNode2D = false;

	bool m_worldDirty : 1 = false;
	bool m_localDirty : 1 = true;
};

REGISTER_ISERIALIZABLE(Node2D, Node2D::CreateInstance);

#include "Scripting/Proxies/Node2DProxy.inl"

#endif
