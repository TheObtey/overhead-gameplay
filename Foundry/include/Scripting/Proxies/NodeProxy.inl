#ifndef NODE__H_
#include "Node.h"
#endif

#include "Event.hpp"

#include <string>

/*A node proxy class used to communicate between the scripting language and the engine API.
The engine manage memory, scripting just get reffs.
Should be scripting language agnostic.
*/
class Node::Proxy
{
public:
	static Proxy CreateNodeProxy(std::string const& name);
	//Garbage collect node proxy in case lua loose reference to node
	static void GCNodeProxy(Proxy* node);

public:
	Proxy(Node& node);
	Proxy(Proxy const& other) = delete;
	Proxy(Proxy && other) noexcept;

	Proxy& operator=(Proxy const& other) = delete;
	Proxy& operator=(Proxy&& other) noexcept;

	~Proxy() = default;

	void AddChild(Proxy* node);

	std::string GetName();

	Event<void()> OnSceneEnter;
	Event<void(float)> OnUpdate;
	Event<void()> OnSceneLeave;

private:
	Node* m_pNode;
};
