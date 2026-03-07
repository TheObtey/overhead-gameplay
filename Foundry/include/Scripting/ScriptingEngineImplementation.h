#ifndef _LUA_STATE__H
#define _LUA_STATE__H

#include <sol/sol.hpp>
#include <sol/state.hpp>

using ScriptEngine = sol::state;

class ScriptingEngineImplementation
{
public:
	ScriptingEngineImplementation();

	static ScriptEngine& GetScriptEngine() { return m_scriptEngine; }
private:


	inline static ScriptEngine m_scriptEngine = {};
};

inline ScriptingEngineImplementation::ScriptingEngineImplementation()
{
	m_scriptEngine.open_libraries(sol::lib::base, sol::lib::package, sol::lib::debug);
}

#endif