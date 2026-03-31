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

	auto GetOrCreateNamespace(std::string const& name);

	std::vector<std::string>& GetRegisteredTypesName() { return m_registeredTypesName; }

private:
	ScriptEngine& m_scriptEngine;
	std::vector<std::string> m_registeredTypesName {};
};

//ifdef LUA
#include "Scripting/Lua/LuaBinder.inl"

#endif // !FOUNDRY_BINDER__H_
