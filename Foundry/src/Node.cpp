#include "Node.h"
#include "Debug.h"
#include "Servers/EngineServer.h"

#include <algorithm>
#include <exception>
#include <functional>
#include <memory>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

Node::Node(std::string const& name) :  m_name(name)
{
    DEBUG("Node : " << m_name << " has been " << ANSI_GREEN << "created !" << ANSI_RESET << std::endl);
}

Node::~Node()
{
    DEBUG("Node : " << m_name << " has been " << ANSI_RED << "deleted !" << ANSI_RESET << std::endl);
}

void Node::Update(float const delta)
{
    OnUpdate(delta);
	OnNodeUpdated(*this, delta);
    for (auto& name : m_childrenOrder)
    {
        m_children[name]->Update(delta);
    }
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

	DEBUG("Node : " << childName << " is now a child of : " << m_name << std::endl);
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

	for (auto&[nodeName, nodePtr] : m_children)
	{
		res.emplace_back(*nodePtr.get());
	}
	return res;
}

uint32 Node::GetChildCount()
{
    return m_children.size();
}

void Node::Destroy()
{
	//TODO make that work in all case (maybe ?)
    if (m_pOwner)
    {
        m_pOwner->RemoveChild(*this);
        return;
    }
}

void Node::Reparent(Node& newParent, bool keepGlobalTransform)
{
    if (m_pOwner == nullptr) return;

    newParent.m_children[m_name] = std::move(m_pOwner->m_children[m_name]);
    newParent.m_childrenOrder.push_back(m_name);
    std::erase(m_pOwner->m_childrenOrder, m_name);
    m_pOwner->m_children.erase(m_name);
    m_pOwner = &newParent;
}

void Node::MoveChild(Node const& child, uint32 to)
{
    if (m_children.contains(child.m_name) == false) return;

    auto childIt = std::find(m_childrenOrder.begin(), m_childrenOrder.end(), child.m_name);
    std::iter_swap(childIt, m_childrenOrder.begin() + to);
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

std::string Node::GetName() { return m_name; }
Node* Node::GetParent() { return m_pOwner; }
SceneTree* Node::GetSceneTree() { return m_pSceneTree; }
