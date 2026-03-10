template <typename T, typename ... Args>
void Binder::BindClass(std::string const& name, Args&& ... params)
{
	m_scriptEngine.new_usertype<T>(name, std::forward<Args>(params)...);
	m_registeredTypesName.push_back(name);
}

template <typename T>
void Binder::BindFunction(std::string const& name, T&& function)
{
	m_scriptEngine[name] = function;
	m_registeredTypesName.push_back(name);
}

inline auto Binder::GetOrCreateNamespace(std::string const& name)
{
	auto it = std::find(m_registeredTypesName.begin(), m_registeredTypesName.end(), name);
	if (it == m_registeredTypesName.end())
		m_registeredTypesName.push_back(name);
	return m_scriptEngine[name].get_or_create<sol::table>();
}
