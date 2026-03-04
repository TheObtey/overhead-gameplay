#ifndef FOUNDRY_BINDER__H_
#define FOUNDRY_BINDER__H_

#include <string>

using ScriptEngine = sol::state;

class Binder
{
public:
	static unsigned GarbageCollect;

	explicit Binder(ScriptEngine& scriptEngine) : m_scriptEngine(scriptEngine) { }

	template <typename T, typename ... Args> 
	void BindClass(std::string const& name, Args&& ... params);

	template <typename T>
	void BindFunction(std::string const& name, T&& function);

private:
	ScriptEngine& m_scriptEngine;
};

//ifdef LUA
#include "Scripting/Lua/LuaBinder.inl"

#endif // !FOUNDRY_BINDER__H_
