#ifndef EVENT__H_
#define EVENT__H_

#include <functional>
#include <vector>
#include <algorithm>

using Connection = int;

template <typename RV, typename... Args>
class Event {};

//hack to force the rv(param) syntax in template parameter
template <typename RV, typename... Args>
class Event<RV(Args...)>
{
public:
	Event() = default;

	Connection Subscribe(std::function<RV(Args ...)> const& func);
	void Unsubscribe(Connection connection);
	void Invoke(Args ... params);

	Connection operator+=(std::function<RV(Args ...)> const& func) { return Subscribe(func); }
	void operator-=(std::function<RV(Args ...)> const& func) { Unsubscribe(func); }
	void operator()(Args ... params) { Invoke(params ...); };

private:
	std::vector<std::function<RV(Args...)>> m_observer {};
	std::vector<int> m_indexToId {};
	int m_nextId = 0;
};


template <typename RV, typename... Args>
Connection Event<RV(Args...)>::Subscribe(std::function<RV(Args ...)> const& func)
{
	m_indexToId.push_back(m_nextId);
	m_observer.push_back(func);
	m_nextId++;
	return m_nextId - 1;
};

template <typename RV, typename... Args>
void Event<RV(Args...)>::Unsubscribe(Connection connection)
{
	int id = m_indexToId[connection];
	std::iter_swap(m_observer.begin() + id,  m_observer.end() - 1);
	m_observer.pop_back();
	std::iter_swap(m_indexToId.begin() + id, m_indexToId.end() - 1);
};

template <typename RV, typename... Args>
void Event<RV(Args...)>::Invoke(Args ... params)
{
	for (auto& func : m_observer)
		std::invoke(func, params ...);
}

#endif // !EVENT__H_
