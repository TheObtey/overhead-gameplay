#ifndef _AUTOMATIC_REGISTER__H
#define _AUTOMATIC_REGISTER__H

//Concept impossible a cause du CRTP. Mais de toute facon vous ne pouvez pas compiler si la classe n'implemente pas CreateInstance()

#include <string>
#include <unordered_map>
#include <functional>

class ISerializable;

#define REGISTER_ISERIALIZABLE(ISerializable_name, create_func) \
    inline bool ISerializable_name ## _entry = AutomaticRegisterISerializable<ISerializable>::add(#ISerializable_name, (create_func))

template <typename T>
struct AutomaticRegisterISerializable
{
public:
    typedef std::function<T* ()> FactoryFunction;
    typedef std::unordered_map<std::string, FactoryFunction> FactoryMap;

    static bool add(const std::string& name, FactoryFunction fac) {
        auto map = getFactoryMap();
        if (map.contains(name)) {
            return false;
        }

        getFactoryMap()[name] = fac;
        return true;
    }

    static T* create(const std::string& name) {
        auto map = getFactoryMap();
        if (!map.contains(name)) {
            return nullptr;
        }

        return map[name]();
    }

    private:
        // Use Meyer's singleton to prevent SIOF
    static FactoryMap& getFactoryMap() {
        static FactoryMap map;
        return map;
    }
};

#endif //_AUTOMATIC_REGISTER__H