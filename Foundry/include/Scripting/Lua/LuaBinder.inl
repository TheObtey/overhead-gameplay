#define StoreUserData() [](Proxy& n, sol::stack_object key, sol::stack_object value) { n.m_userData.set(key, value); }
#define LoadUserData() [](Proxy& n, sol::stack_object key) { return n.m_userData.get<sol::object>(key); }

template <typename T, typename ... Args>
void Binder::BindClass(std::string const& name, Args&& ... params)
{
	m_scriptEngine.new_usertype<T>(name, std::forward<Args>(params)...);
}

template <typename T>
void Binder::BindFunction(std::string const& name, T&& function)
{
	m_scriptEngine[name] = function;
}

template <typename T, typename ... Args>
void Binder::BindEnum(std::string const& name, Args&& ... params)
{
	m_scriptEngine[name] = m_scriptEngine.create_table_with(std::forward<Args>(params)...);
}

inline auto Binder::GetFunction(std::string const& name) const
{
	sol::protected_function f = m_scriptEngine[name];
	return f;
}

inline auto Binder::GetOrCreateNamespace(std::string const& name)
{
	return m_scriptEngine[name].get_or_create<sol::table>();
}
