#ifndef FOUNDRY_ISCRIPTING_ENGINE__H_
#define FOUNDRY_ISCRIPTING_ENGINE__H_

#ifdef LUA
#include <sol/state.hpp>
#include <sol/sol.hpp>
using ScriptEngine = sol::state;
#endif

class ScriptingEngine
{
public:
	static ScriptEngine& GetScriptEngine() { return Instance().m_scriptEngine; }

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
};

#endif