#ifndef _AUTOMATIC_REGISTER_PROXY__H
#define _AUTOMATIC_REGISTER_PROXY__H

#include "Scripting/Binder.h"
#include "Scripting/ScriptingEngine.h"

#define REGISTER_SERIALIZABLE_OBJECT(name) inline static name Binding {};

template<typename D>
struct AutomaticRegisterProxy {
    AutomaticRegisterProxy()
    {
        Binder B(ScriptingEngine::GetScriptEngine());
        D::Bind(B);
        ScriptingEngine::RegisterTypeNames(B.GetRegisteredTypesName());
    }
};

#endif //_AUTOMATIC_REGISTER_PROXY__H