#include "Scripting/Lua/LuaScriptInstance.hpp"
#include "Servers/LuaServer.h"
#include "Logger.hpp"

LuaScriptInstance::LuaScriptInstance(std::string const& scriptPath) :
	m_state(LuaServer::GetLuaState()),
	m_stringPath(scriptPath)
{
	m_enviro = {m_state, sol::create};
	m_enviro["CreateNode"] = m_state["CreateNode"];
	m_enviro["print"] = m_state["print"];
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

void LuaScriptInstance::CallScriptOnUpdate(float dt)
{
	if(!m_updateFunc.valid()) return;
	auto result = m_updateFunc(dt);

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
