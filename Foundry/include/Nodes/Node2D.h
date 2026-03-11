#ifndef FOUNDRY_NODE2D__H_
#define FOUNDRY_NODE2D__H_

#include "Node.h"
#include "Transform2D.h"

class Node2D : public Node
{
public:
	class Proxy;

	Node2D(
		float _x = 0.0f, float _y = 0.0f,
		float _scaleX = 1.0f, float _scaleY = 1.0f,
		bool _statism = false
	);
	Node2D(Transform2D _transform);
	Node2D(std::string const& name) : Node(name) {
		OnParentChange += [&](Node& node) {
			CheckParentTransform(node);
			UpdateWorld();
		};
	}
	~Node2D() override;


	Node2D  operator*(Node2D const& other) const;
	Node2D& operator*=(Node2D const& other);
	
	Node2D  operator+(Node2D const& other) const;
	Node2D& operator+=(Node2D const& other);
	
	Node2D  operator-(Node2D const& other) const;
	Node2D& operator-=(Node2D const& other);
	
	Node2D  operator/(Node2D const& other) const;
	Node2D& operator/=(Node2D const& other);


	void		SetScale(glm::vec2 _scale);
	void		SetScale(float _width, float _height);
	glm::vec2	GetScale() const;

	void		SetRotation(float _x, float _y);
	glm::vec2	GetRotation() const;

	void		SetPosition(glm::vec2 _pos);
	void		SetPosition(float _x, float _y);
	glm::vec2	GetPosition() const;

	glm::mat3&	GetTransformationMatrix();

	void		SetStatism(bool _statism);
	bool		IsStatic() const;

	void		UpdateLocal();
	void		UpdateWorld();

	void		SetWorldScale(glm::vec3& _worldScale);
	void		SetWorldRotation(glm::vec3& _worldRot);
	void		SetWorldPosition(glm::vec3& _worldPos);

	virtual void OnUpdate(double _delta) override;
	virtual void Reparent(Node& _newParent, bool _keepGlobalTransform = true) override;
	
	void		CheckParentTransform(Node& node);

private:
	Transform2D m_transform;

	glm::vec3 m_worldPosition;
	glm::vec3 m_worldRotation;
	glm::vec3 m_worldScale;
	glm::mat3 m_worldTransform;

	bool m_isParentNode2D = false;

	bool m_worldDirty : 1 = true;
	bool m_localDirty : 1 = true;
};

#include "Scripting/Proxies/Node2DProxy.inl"

#endif