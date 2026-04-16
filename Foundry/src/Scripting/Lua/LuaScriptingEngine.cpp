#include "Scripting/ScriptingEngine.h"
#include "Scripting/RegisterProxies.h"

ScriptingEngine::ScriptingEngine() { OnInitialize(); }
ScriptingEngine::~ScriptingEngine() { OnUninitialize(); }

void ScriptingEngine::Update(double dt)
{
    auto& instance = Instance();
    sol::function timerUpdateFunc = instance.m_scriptEngine["timer"]["Tick"];
    sol::function tweenUpdateFunc = instance.m_scriptEngine["tween"]["Tick"];
    timerUpdateFunc(dt);
    tweenUpdateFunc(dt);
}

void ScriptingEngine::OnInitialize()
{
    m_scriptEngine.open_libraries(sol::lib::base, sol::lib::package, sol::lib::debug, sol::lib::os);
    RegisterProxies(m_scriptEngine);

    m_scriptEngine.script_file("res/scripts/.foundry/hook.lua");
    m_scriptEngine.script_file("res/scripts/.foundry/timer.lua");
    m_scriptEngine.script_file("res/scripts/.foundry/tween.lua");
    m_scriptEngine.script_file("res/scripts/.foundry/ease.lua");
}

void ScriptingEngine::OnUninitialize() { m_scriptEngine.collect_garbage(); }