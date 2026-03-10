#include "Scripting/ScriptingEngine.h"

ScriptingEngine::ScriptingEngine() { OnInitialize(); }
ScriptingEngine::~ScriptingEngine() { OnUninitialize(); }

void ScriptingEngine::OnInitialize() { m_scriptEngine.open_libraries(sol::lib::base, sol::lib::package, sol::lib::debug); }
void ScriptingEngine::OnUninitialize() { m_scriptEngine.collect_garbage(); }