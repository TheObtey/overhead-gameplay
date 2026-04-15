#include "Scripting/Lua/LuaScriptInstance.hpp"
#include "Scripting/ScriptingEngine.h"
#include "Logger.hpp"


LuaScriptInstance::LuaScriptInstance(std::string const& scriptPath) :
	m_state(ScriptingEngine::GetScriptEngine()),
	m_stringPath(scriptPath)
{
	Logger::Log("Loading Lua Script : ", scriptPath);
	m_enviro = {m_state, sol::create, m_state.globals()};
}

void LuaScriptInstance::CallScriptOnInit()
{
	if(!m_initFunc.valid()) return;
	auto result = m_initFunc();

	if (!result.valid())
	{
		sol::error err = result;
		Logger::LogWithLevel(LogLevel::ERROR, "Lua script error : ", err.what());
	}
}

void LuaScriptInstance::CallScriptOnUpdate(double dt)
{
	if(!m_updateFunc.valid()) return;
	auto result = m_updateFunc(dt);

	if (!result.valid())
	{
		sol::error err = result;
		Logger::LogWithLevel(LogLevel::ERROR, "Lua script error : ", err.what());
	}
}

void LuaScriptInstance::CallScriptOnPhysicsUpdate(double dt)
{
	if(!m_updatePhysicsFunc.valid()) return;
	auto result = m_updatePhysicsFunc(dt);

	if (!result.valid())
	{
		sol::error err = result;
		Logger::LogWithLevel(LogLevel::ERROR, "Lua script error : ", err.what());
	}
}

void LuaScriptInstance::CallScriptOnDestroy()
{
	if(!m_destroyFunc.valid()) return;
	auto result = m_destroyFunc();

	if (!result.valid())
	{
		sol::error err = result;
		Logger::LogWithLevel(LogLevel::ERROR, "Lua script error : ", err.what());
	}
}
