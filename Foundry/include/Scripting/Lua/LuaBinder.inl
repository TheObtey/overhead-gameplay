#include "Servers/LuaServer.h"
#include <sol/types.hpp>

inline unsigned Binder::GarbageCollect = static_cast<unsigned>(sol::meta_function::garbage_collect);

template <typename T, typename ... Args> 
void Binder::BindClass(std::string const& name, Args&& ... params)
{
	LuaServer::GetLuaState().new_usertype<T>(name, std::forward<Args>(params)...);
}


template <typename T>
void Binder::BindFunction(std::string const& name, T&& function)
{
	LuaServer::GetLuaState()[name] = function;
}
