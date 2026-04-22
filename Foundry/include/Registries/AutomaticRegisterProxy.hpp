#ifndef _AUTOMATIC_REGISTER_PROXY__H
#define _AUTOMATIC_REGISTER_PROXY__H

#include "Scripting/Binder.h"
#include "Scripting/ScriptingEngine.h"

#define LINE __LINE__
#define REGISTER_PROXY(Class, name) static bool name = AutomaticRegisterProxy::Register<Class>();

struct AutomaticRegisterProxy
{
    template <typename T>
    static bool Register(ScriptEngine& engine)
    {
        Binder B(engine);
        T::Bind(B);
        return true;
    }
};

#endif //_AUTOMATIC_REGISTER_PROXY__H