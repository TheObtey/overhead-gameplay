#ifndef FOUNDRY_LUA_ENVIRO__H_
#define FOUNDRY_LUA_ENVIRO__H_

#include "Scripting/ScriptInstance.h"

#include <functional>
#include <sol/sol.hpp>

class LuaScriptInstance : public ScriptInstance
{
public:
	LuaScriptInstance(std::string const& scriptPath);

	template <typename T>
	void AttachToProxy(T* const proxy);

private:
	void CallScriptOnInit() override;
	void CallScriptOnUpdate(double dt) override;
	void CallScriptOnPhysicsUpdate(double dt) override;
	void CallScriptOnDestroy() override;

private:
	sol::environment m_enviro;
	sol::state& m_state;
	std::string m_stringPath;

	sol::protected_function m_initFunc;
	sol::protected_function m_updateFunc;
	sol::protected_function m_timerUpdateFunc;
	sol::protected_function m_updatePhysicsFunc;
	sol::protected_function m_destroyFunc;
};

template <typename T>
inline void LuaScriptInstance::AttachToProxy(T* const proxy)
{
	m_enviro["self"] = proxy;

	proxy->OnSceneEnter			+= [this] { CallScriptOnInit(); };
	proxy->OnUpdate				+= [this](double const dt) { CallScriptOnUpdate(dt); };
	proxy->OnPhysicsUpdate		+= [this](double const dt) { CallScriptOnUpdate(dt); };
	proxy->OnSceneLeave			+= [this] { CallScriptOnDestroy(); } ;

	auto file = m_state.script_file(m_stringPath, m_enviro);

	m_initFunc = m_enviro["OnInit"];
	m_updateFunc = m_enviro["OnUpdate"];
	m_timerUpdateFunc = m_enviro["timer"]["Tick"];
	m_destroyFunc = m_enviro["OnDestroy"];
}


#endif // !FOUNDRY_LUA_ENVIRO__H_
