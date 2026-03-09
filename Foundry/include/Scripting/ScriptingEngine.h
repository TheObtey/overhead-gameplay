#ifndef FOUNDRY_ISCRIPTING_ENGINE__H_
#define FOUNDRY_ISCRIPTING_ENGINE__H_

#define LUA

#ifdef LUA
#include <sol/state.hpp>
#include <sol/sol.hpp>
using ScriptEngine = sol::state;
#endif

class ScriptingEngine
{
public:
	static ScriptEngine& GetScriptEngine() { return Instance().m_scriptEngine; }
	static void RegisterTypeNames(std::vector<std::string>& names) { Instance().m_registeredType.insert(Instance().m_registeredType.end(), names.begin(), names.end()); }
	static std::vector<std::string>const& GetRegisteredTypesName() { return Instance().m_registeredType; }

protected:
  	ScriptingEngine();
	~ScriptingEngine();

	void OnInitialize();
	void OnUninitialize();

    static ScriptingEngine& Instance()
    {
      static ScriptingEngine engine;
      return engine;
    }

	ScriptEngine m_scriptEngine;
	std::vector<std::string> m_registeredType;
};

#endif