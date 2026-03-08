#ifndef _AUTOMATIC_REGISTER_PROXY__H
#define _AUTOMATIC_REGISTER_PROXY__H

#include "Scripting/Binder.h"
#include "Scripting/ScriptingEngine.h"

template<typename D>
struct AutomaticRegisterProxy
{
private:
    struct exec_register
    {
        exec_register()
        {
            Binder B(ScriptingEngine::GetScriptEngine());
            D::Bind(B);
            ScriptingEngine::RegisterTypeNames(B.GetRegisteredTypesName());
        }
    };

    template<exec_register&> struct ref_it { };

    static exec_register register_object;
    static ref_it<register_object> referrer;
};

template<typename D> typename AutomaticRegisterProxy<D>::exec_register
    AutomaticRegisterProxy<D>::register_object;

#endif //_AUTOMATIC_REGISTER_PROXY__H