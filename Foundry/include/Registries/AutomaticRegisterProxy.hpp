#ifndef _AUTOMATIC_REGISTER_PROXY__H
#define _AUTOMATIC_REGISTER_PROXY__H

#include "Scripting/Binder.h"
#include "Scripting/ScriptingEngine.h"

#define REGISTER_SERIALIZABLE_OBJECT(name) inline static name Binding {};

template<typename D>
struct AutomaticRegisterProxy {
    AutomaticRegisterProxy()
    {
        //TODO Find a better way
        static bool guard = false;

        if (guard) return;
        Binder B(ScriptingEngine::GetScriptEngine());
        D::Bind(B);
        ScriptingEngine::RegisterTypeNames(B.GetRegisteredTypesName());
        guard = true;
    }
};

#endif //_AUTOMATIC_REGISTER_PROXY__H