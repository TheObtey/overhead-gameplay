#ifndef FOUNDRY_BINDER__H_
#define FOUNDRY_BINDER__H_

#include <string>

class Binder
{
public:
	static unsigned GarbageCollect;

	template <typename T, typename ... Args> 
	static void BindClass(std::string const& name, Args&& ... params);

	template <typename T>
	static void BindFunction(std::string const& name, T&& funciton);
};

//ifdef LUA
#include "Scripting/Lua/LuaBinder.inl"

#endif // !FOUNDRY_BINDER__H_
