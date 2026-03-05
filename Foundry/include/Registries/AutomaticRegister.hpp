#pragma once

#include <iostream>

//template <typename T>
//concept CanRegister = requires(T t)
//{
//    { T::Register() } -> std::same_as<std::function<ISerializable*()>>;
//};

template<typename D>
struct automatic_register {
private:
    struct exec_register {
        exec_register() 
        {
            ISerializable::s_constructors[typeid(D).name()] = D::Register();
        }
    };
    // will force instantiation of definition of static member
    template<exec_register&> struct ref_it {};

    static exec_register register_object;
    static ref_it<register_object> referrer;
};

template<typename D> typename automatic_register<D>::exec_register
    automatic_register<D>::register_object;
