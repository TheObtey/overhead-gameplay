enum Binder::MetaFunction : uint32
{
	GarbageCollect = static_cast<uint32>(sol::meta_function::garbage_collect),
	Constructor = static_cast<uint32>(sol::meta_function::construct)
};

enum Binder::Operator : uint32
{
	Addition = static_cast<uint32>(sol::meta_function::addition)
};

template <typename T, typename ... Args> 
void Binder::BindClass(std::string const& name, Args&& ... params)
{
	m_scriptEngine.new_usertype<T>(name, std::forward<Args>(params)...);
	m_registeredTypesName.push_back(std::move(name));
}

template <typename T>
void Binder::BindFunction(std::string const& name, T&& function)
{
	m_scriptEngine[name] = function;
	m_registeredTypesName.push_back(std::move(name));
}
