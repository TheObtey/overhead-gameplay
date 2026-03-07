#ifndef _AUTOMATIC_REGISTER_ISERIALIZABLE__H
#define _AUTOMATIC_REGISTER_ISERIALIZABLE__H

#include <iostream>

//template <typename T>
//concept CanRegisterISerializable = requires(T t)
//{
//    { T::Register() } -> std::same_as<std::function<ISerializable*()>>;
//};

template<typename D>
struct AutomaticRegisterISerializable {
public:
    AutomaticRegisterISerializable()
    {
        if (!s_registered)
        {
            ISerializable::s_constructors[typeid(D).name()] = D::Register();
            s_registered = true;
        }
    };

private:
    static bool s_registered;
};

template <typename T>
inline bool AutomaticRegisterISerializable<T>::s_registered = false;

#endif //_AUTOMATIC_REGISTER_ISERIALIZABLE__H