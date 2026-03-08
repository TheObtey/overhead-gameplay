#ifndef FOUNDRY_BINDER__H_
#define FOUNDRY_BINDER__H_

#define OVERLOAD(Class, Ret, ...) static_cast<Ret (Class::*)(__VA_ARGS__)>

#include <list>

#include "Define.h"
#include "ScriptingEngine.h"

class Binder
{
public:
	enum MetaFunction : uint32;
	enum Operator : uint32;

	explicit Binder(ScriptEngine& scriptEngine) : m_scriptEngine(scriptEngine) {  }

	template <typename T, typename ... Args>
	void BindClass(std::string const& name, Args&& ... params);

	template <typename T>
	void BindFunction(std::string const& name, T&& function);

	std::vector<std::string>& GetRegisteredTypesName() { return m_registeredTypesName; }

private:
	ScriptEngine& m_scriptEngine;
	std::vector<std::string> m_registeredTypesName {};
};

//ifdef LUA
#include "Scripting/Lua/LuaBinder.inl"

#endif // !FOUNDRY_BINDER__H_
