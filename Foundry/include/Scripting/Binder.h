#ifndef FOUNDRY_BINDER__H_
#define FOUNDRY_BINDER__H_

#define OVERLOAD(Class, Ret, ...) static_cast<Ret (Class::*)(__VA_ARGS__)>
#define OVERLOAD_CONST(Class, Ret, ...) static_cast<Ret (Class::*)(__VA_ARGS__) const>

#include "ScriptingEngine.h"

class Binder
{
public:
	explicit Binder(ScriptEngine& scriptEngine) : m_scriptEngine(scriptEngine) {  }

	template <typename T, typename ... Args>
	void BindClass(std::string const& name, Args&& ... params);

	template <typename T>
	void BindFunction(std::string const& name, T&& function);

	template <typename T, typename ... Args>
	void BindEnum(std::string const& name, Args&& ... params);

	auto GetFunction(std::string const& name) const;
	auto GetOrCreateNamespace(std::string const& name);

private:
	ScriptEngine& m_scriptEngine;
};

//ifdef LUA
#include "Scripting/Lua/LuaBinder.inl"

#endif // !FOUNDRY_BINDER__H_
