#ifndef EXPECTED__H_
#define EXPECTED__H_

#include <type_traits>
#include <utility>

template <typename T>
concept destructible = requires (T t) 
{
	std::is_destructible_v<T>;
};

template <destructible E> 
class Unexpected
{
public:
	constexpr Unexpected(const Unexpected&) = default;
	constexpr Unexpected(Unexpected&&) = default;
	constexpr explicit Unexpected(E&& e) : m_error(std::forward<E>(e)) {}

	constexpr const E&  Error() const& noexcept  { return m_error; }
	constexpr		E&  Error() & noexcept 		 { return m_error; };
	constexpr const E&& Error() const&& noexcept { return m_error; };
	constexpr 		E&& Error() && noexcept		 { return m_error; };

private:
	E m_error;
};


template <destructible VAL, destructible ERR> 
class Expected
{
public:
	constexpr Expected() = default;
	constexpr Expected(Expected<VAL, ERR> const& other);
	constexpr Expected(Expected<VAL, ERR>&& other) noexcept;

	template <typename T>
	constexpr explicit(!std::is_convertible_v<T const&, ERR>) Expected(Unexpected<T>& e) : m_unex(std::forward<const T&>(e.error())) {};

	template <typename T>
	constexpr explicit(!std::is_convertible_v<T, ERR>) Expected(Unexpected<T>&& e) : m_unex(std::forward<T>(e.Error())) {};

	template< class T = std::remove_cv_t<VAL> >
	constexpr explicit(!std::is_convertible_v<T, VAL>) Expected( T&& v ) : m_value(std::forward<T>(v)), m_hasValue(true) {};
	
	constexpr ~Expected() = default;

	constexpr const ERR&  Error() const& noexcept  { return m_unex; }
	constexpr		ERR&  Error() & noexcept 	   { return m_unex; };
	constexpr const ERR&& Error() const&& noexcept { return m_unex; };
	constexpr 		ERR&& Error() && noexcept	   { return m_unex; };

	constexpr const VAL&  Value() const& noexcept  { return m_value; }
	constexpr		VAL&  Value() & noexcept 	   { return m_value; };
	constexpr const VAL&& Value() const&& noexcept { return m_value; };
	constexpr 		VAL&& Value() && noexcept	   { return m_value; };

	constexpr VAL& operator*()  { return m_value;  }
	constexpr VAL* operator->() { return &m_value; }
	constexpr operator bool()   { return m_hasValue;  }

	constexpr bool HasValue() const { return m_hasValue; }

private:
	union
	{
		VAL m_value;
		ERR m_unex;
	};

	bool m_hasValue = false;
};

template <destructible VAL, destructible ERR>
constexpr inline Expected<VAL, ERR>::Expected(Expected<VAL, ERR> const& other)
{
	if (other.HasValue())
	{
		m_value = *other;
		m_hasValue = true;
		return;
	}

	m_unex = other.Error();
}

template <destructible VAL, destructible ERR>
constexpr inline Expected<VAL, ERR>::Expected(Expected<VAL, ERR>&& other) noexcept
{
	if (other.HasValue())
	{
		m_value = std::move(*other);
		m_hasValue = true;
		return;
	}

	m_unex = std::move(other.Error());
}

#endif // !EXPECTED_H__
