#ifndef _AUTOMATIC_REGISTER_ISERIALIZABLE__H
#define _AUTOMATIC_REGISTER_ISERIALIZABLE__H

//Concept impossible a cause du CRTP. Mais de toute facon vous ne pouvez pas compiler si la classe n'implemente pas CreateInstance()

template<typename D>
struct AutomaticRegisterISerializable {
public:
    struct exec_register
    {
        exec_register()
        {
            if (s_registered) return;

            ISerializable::s_constructors[typeid(D).name()] = D::CreateInstance();
            s_registered = true;
        }
    };

    template<exec_register&> struct ref_it { };

    static exec_register register_object;
    static ref_it<register_object> referrer;

private:
    static bool s_registered;
};

template<typename D> AutomaticRegisterISerializable<D>::exec_register
    AutomaticRegisterISerializable<D>::register_object;

template <typename D>
inline bool AutomaticRegisterISerializable<D>::s_registered = false;

#endif //_AUTOMATIC_REGISTER_ISERIALIZABLE__H