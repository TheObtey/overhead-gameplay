#include "Scripting/ScriptingEngine.h"

ScriptingEngine::ScriptingEngine() { OnInitialize(); }
ScriptingEngine::~ScriptingEngine() { OnUninitialize(); }

void ScriptingEngine::OnInitialize()
{
    m_scriptEngine.open_libraries(sol::lib::base, sol::lib::package, sol::lib::debug, sol::lib::os);
    m_scriptEngine.script_file("res/.foundry/hook.lua");
    m_scriptEngine.script_file("res/.foundry/timer.lua");
}

void ScriptingEngine::OnUninitialize() { m_scriptEngine.collect_garbage(); }