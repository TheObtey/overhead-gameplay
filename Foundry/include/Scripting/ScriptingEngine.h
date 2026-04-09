#ifndef FOUNDRY_ISCRIPTING_ENGINE__H_
#define FOUNDRY_ISCRIPTING_ENGINE__H_

#ifdef LUA
#include <sol/state.hpp>
#include <sol/sol.hpp>
using ScriptEngine = sol::state;
#endif

#include <set>

class ScriptingEngine
{
public:
	static ScriptEngine& GetScriptEngine() { return Instance().m_scriptEngine; }
	static void RegisterTypeNames(std::vector<std::string>& names) 
	{
		for (std::string const& name : names)
			Instance().m_registeredType.insert(name);
	}
	static std::set<std::string>const& GetRegisteredTypesName() { return Instance().m_registeredType; }

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
	std::set<std::string> m_registeredType;
};

#endif