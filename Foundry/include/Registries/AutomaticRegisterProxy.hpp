#ifndef _AUTOMATIC_REGISTER_PROXY__H
#define _AUTOMATIC_REGISTER_PROXY__H

#include "Scripting/Binder.h"
#include "Scripting/ScriptingEngineImplementation.h"

#include <iostream>

//template <typename T>
//concept CanRegisterISerializable = requires(T t)
//{
//    { T::Register() } -> std::same_as<std::function<ISerializable*()>>;
//};

template<typename D>
struct AutomaticRegisterProxy
{
public:
    AutomaticRegisterProxy()
    {
        if (!s_registered)
        {
            s_registered = true;
            Binder B(ScriptingEngineImplementation::GetScriptEngine());
            D proxy; 
            proxy.Bind(B);
        }
    };

private:
    static bool s_registered;
};

template <typename T> 
inline bool AutomaticRegisterProxy<T>::s_registered = false;

#endif //_AUTOMATIC_REGISTER_PROXY__H