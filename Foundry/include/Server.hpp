#ifndef SERVER__H_
#define SERVER__H_

#include <concepts>
#include <mutex>
#include <queue>

class TaskGraph;

template <typename T>
struct Command {};


/*
 * Servers are singleton interface to lower level tasks like opening a window or physics, for instance.
 * Servers are independant. Please avoid including other server in your server header file.
 * Please call your server NameServer e.g : RenderServer
 */
template <typename Derived> // The Curiously Recurring Template Pattern (CRTP)
class Server
{
private:
    Server() 
	{ 
		static_assert(std::derived_from<Derived, Server>, "Derived must inherit from Server<Derived>"); 
	}


public:
    static void FlushCommands() { static_cast<Server<Derived>&>(Instance()).FlushCommandsImpl(); }
    static void BuildTasks(TaskGraph& graph) { static_cast<Server<Derived>&>(Instance()).BuildTasksImpl(graph); }

    virtual void FlushCommandsImpl() = 0;
    virtual void BuildTasksImpl(TaskGraph& graph) = 0;

protected:

    static Derived& Instance();
	virtual void OnInitialize() {} ;
	virtual void OnUnInitialize() {};

    friend Derived;

protected:
    static std::mutex s_mutex;
    std::queue<Command<Derived>> m_commands;
	bool m_initialized = false;
};

template <typename Derived>
std::mutex Server<Derived>::s_mutex {};

template<typename Derived>
Derived& Server<Derived>::Instance()
{
    //making the singleton thread safe
    std::lock_guard<std::mutex> lock(s_mutex);
    static Derived instance;
    return instance;
}

#endif
