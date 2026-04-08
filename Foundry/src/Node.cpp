#include "Node.h"
#include "SceneTree.h"
#include "Debug.h"
#include "Servers/EngineServer.h"
#include "Serialization/SerializeObject.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


Node::Node(std::string const& name) : m_name(name) , m_scriptPath("")
{
    DEBUG("Node : " << m_name << " has been " << ANSI_GREEN << "created !" << ANSI_RESET << std::endl);
	OnHierarchyChanged += [&]() { NotifyHierarchyChanged(); };
}

Node::~Node()
{
    DEBUG("Node : " << m_name << " has been " << ANSI_RED << "deleted !" << ANSI_RESET << std::endl);
}

void Node::Update(double const delta)
{
    OnUpdate(delta);
	OnNodeUpdated(*this, delta);
    for (auto& name : m_childrenOrder)
        m_children[name]->Update(delta);

}

void Node::PhysicsUpdate(double delta)
{
	OnPhysicsUpdate(delta);
	OnNodePhysicsUpdated(*this, delta);
	for (auto& name : m_childrenOrder)
		m_children[name]->PhysicsUpdate(delta);
}

void Node::AddChild(std::unique_ptr<Node>&& child)
{
    EngineServer::QueueAttach(child, this);
}

void Node::AttachChildImmediate(std::unique_ptr<Node>& child)
{
   	//return nothing if we allready have a parent -> use the reparent method instead
	if (child->m_pOwner) return;

	//Child with the same name allready exist
	std::string childName = child->m_name;
	if (m_children.contains(childName)) return;

	m_children[childName] = std::move(child);
	m_children[childName]->m_pOwner = this;
	m_children[childName]->m_pSceneTree = m_pSceneTree;
	m_childrenOrder.push_back(childName);

	m_children[childName]->OnSceneEnter(*m_children[childName]);
	m_children[childName]->OnParentChange(*this);
	NotifyHierarchyChanged();

	DEBUG("Node : " << childName << " is now a child of : " << m_name << std::endl);
}

void Node::NotifyHierarchyChanged()
{
	for (auto const& node: m_children | std::views::values)
		node->OnHierarchyChanged();
}

void Node::AddChild(std::unique_ptr<Node>& child)
{
    AddChild(std::move(child));
}

void Node::AddSibling(std::unique_ptr<Node>& child)
{
    AddSibling(std::move(child));
}

void Node::AddSibling(std::unique_ptr<Node>&& sibling)
{
	if (m_pOwner == nullptr) return;

	m_pOwner->AddChild(std::move(sibling));
}

void Node::RemoveChild(Node& child)
{
	if (m_children.contains(child.m_name) == false) return;
	RemoveChild(child.m_name);
}

void Node::RemoveChild(std::string const& childName)
{
	if (m_children.contains(childName) == false) return;

	Node* const child = m_children[childName].get();

	child->OnSceneLeave(*child);
	child->m_pOwner = nullptr;
	child->m_pSceneTree = nullptr;
	EngineServer::QueueFree(m_children[childName]);
	std::erase(m_childrenOrder, childName);
	m_children.erase(childName);
}

OptionalRef<Node> Node::FindChild(std::string const& name)
{
	for (auto&[nodeName, nodePtr] : m_children)
	{
		if (nodeName == name)
			return {*nodePtr.get()};
	}
	return {};
}

Node& Node::GetChild(uint32 index)
{
    if (m_children.empty()) throw std::exception();
	return *m_children[m_childrenOrder[index]];
}

std::vector<std::reference_wrapper<Node>> Node::GetChildren()
{
	std::vector<std::reference_wrapper<Node>> res;

	for (auto &nodePtr: m_children | std::views::values)
		res.emplace_back(*nodePtr.get());

	return res;
}

uint32 Node::GetChildCount() const
{
    return m_children.size();
}

void Node::Destroy()
{
    if (m_pOwner)
        m_pOwner->RemoveChild(*this);
	else if (m_pSceneTree)
		m_pSceneTree->m_root.reset();

	NotifyHierarchyChanged();
}

void Node::Reparent(Node& newParent, bool keepGlobalTransform)
{
    if (m_pOwner == nullptr) return;
	if (newParent.m_name == m_pOwner->m_name)
	{
		OnParentChange(newParent);
		OnHierarchyChanged();
		return;
	}

    newParent.m_children[m_name] = std::move(m_pOwner->m_children[m_name]);
    newParent.m_childrenOrder.push_back(m_name);
    std::erase(m_pOwner->m_childrenOrder, m_name);
    m_pOwner->m_children.erase(m_name);
    m_pOwner = &newParent;

	DEBUG(m_name << " reparented to " << newParent.m_name);

	OnParentChange(*this);
}

void Node::MoveChild(Node const& child, uint32 to)
{
    if (m_children.contains(child.m_name) == false) return;

    auto childIt = std::find(m_childrenOrder.begin(), m_childrenOrder.end(), child.m_name);
    std::iter_swap(childIt, m_childrenOrder.begin() + to);
}

uptr<Node> Node::DetachFromTree()
{
	if (HasParent() == true)
	{
		uptr<Node> detachedNode = std::move(m_pOwner->m_children[m_name]);
		m_pOwner->m_children.erase(m_name);
		return detachedNode;
	}

	if (GetChildCount() == 0) return std::move(m_pSceneTree->m_root);

	uptr<Node> detachedNode = std::move(m_pSceneTree->m_root);
	m_pSceneTree->ChangeSceneToNode(m_children[m_childrenOrder[0]]);
	return detachedNode;
}

std::unique_ptr<Node> Node::Clone()
{
    std::unique_ptr<Node> copy = Node::CreateNode<std::remove_reference_t<decltype(*this)>>(m_name + "Copy");

    for (auto& [Name, nodePtr] : m_children)
    {
        auto childClone = nodePtr->Clone();
        copy->AddChild(childClone);
    }

    return copy;
}

void Node::Serialize(SerializedObject& datas) const
{
	// Call baseClass::Serialize(datas) : Example Node::Serialize(datas)
	datas.SetType("Node");
	datas.AddPublicElement("Name", &m_name);
	datas.AddPrivateArray("Children");
	std::string parent = "";
	if (m_pOwner != nullptr)
		parent = m_pOwner->GetName();

	datas.AddPrivateElement("m_pOwner", &parent);

	std::string scriptPath = m_scriptPath;
	if (m_pScriptInstance.get() != nullptr)
		scriptPath = m_pScriptInstance->GetPath();

	datas.AddPublicElement("m_scriptPath", &scriptPath);

	for (uint32 i = 0; i < m_children.size(); i++)
	{
		datas.AddPrivateElementInArray("Children", static_cast<ISerializable const*>(m_children.at(m_childrenOrder[i]).get()));
	}

}



void Node::Deserialize(SerializedObject const& datas)
{
	// Call baseClass::Deserialize(datas) : Example Node::Deserialize(datas)
	std::string t = datas.GetType();
	std::string newname;
	datas.GetPublicElement("Name",&newname);
	SetName(newname);

	datas.GetPublicElement("m_scriptPath", &m_scriptPath);
	if (m_scriptPath != "" && !s_IsInEditor )
	{
		DEBUG("ATTACH " << m_scriptPath << std::endl);
		uptr<LuaScriptInstance> script = std::make_unique<LuaScriptInstance>(m_scriptPath);
		Node::AttachScript(script, *this);
	}

	std::vector<ISerializable*> tempList = datas.GetPrivateArray<ISerializable*>("Children");
	for (uint32 i = 0; i < tempList.size(); i++)
	{
		uptr<Node> pNode = uptr<Node>((Node*)tempList[i]);
		AddChild(pNode);
	}
}

ISerializable* Node::CreateInstance()
{
	return CreateNode<Node>("Node").release();
}
void Node::SetName(const std::string& newName)
{
	if (m_name == newName)
		return;

	if (m_pOwner)
	{
		if (m_pOwner->m_children.contains(newName))
		{
			DEBUG("[Node] Name already exists: " << newName << std::endl);
			return;
		}

		std::string oldName = m_name;

		auto it = m_pOwner->m_children.find(oldName);
		if (it != m_pOwner->m_children.end())
		{
			auto nodePtr = std::move(it->second);
			m_pOwner->m_children.erase(it);

			m_name = newName;

			m_pOwner->m_children[newName] = std::move(nodePtr);
		}

		for (std::string& name : m_pOwner->m_childrenOrder)
		{
			if (name == oldName)
			{
				name = newName;
				break;
			}
		}
	}
	else
	{
		m_name = newName;
	}
}
std::string Node::GetName()							{ return m_name; }
void Node::SetScriptPath(std::string const& path)	{ m_scriptPath = path;}
Node* Node::GetParent() const						{ return m_pOwner; }
bool Node::HasParent() const						{ return m_pOwner != nullptr; }
SceneTree* Node::GetSceneTree() const				{ return m_pSceneTree; }
Node::Proxy& Node::GetNodeProxy() const				{ return *m_pProxy; }
