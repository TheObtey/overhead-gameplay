#ifndef DEFINE__H_
#define DEFINE__H_

//Warning : this header is precompiled, please do not include any other header file inside
#include <cstdint>
#include <memory>
#include <numbers>

#include <string>
#include <vector>
#include <glad/glad.h>

using char8 = std::int8_t;
using int8  = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using byte   = std::uint8_t;
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

//=== ANSI ESCAPE CODE ========
#define ESC "\x1B"
#define ANSI_RED      ESC"[31m"
#define ANSI_GREEN    ESC"[32m"
#define ANSI_GOLD     ESC"[38;5;178m"

#define ANSI_ITALIC   ESC"[3m"
#define ANSI_BOLD     ESC"[1m"

#define ANSI_RESET    ESC"[0m"
//=============================

template <typename T>
constexpr T pi_t = std::numbers::pi_v<T>;

constexpr float pi = pi_t<float>;

//Move in own file ?
template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using wptr = std::weak_ptr<T>;

//===== UDL =======

inline constexpr int32 operator""_o (unsigned long long val) { return val; }
inline constexpr int32 operator""_ko(unsigned long long val) { return val * 1024; }
inline constexpr int32 operator""_mo(unsigned long long val) { return val * 1024 * 1024; }
inline constexpr int32 operator""_go(unsigned long long val) { return val * 1024 * 1024 * 1024; }

inline constexpr long double operator""_deg_to_rad(long double deg) { return deg * pi_t<long double> / 180; }
inline constexpr long double operator""_rad_to_deg(long double rad) { return rad * pi_t<long double>; }

inline constexpr int32 operator""_mg(unsigned long long val) { return val / 1000; }
inline constexpr int32 operator""_g (unsigned long long val) { return val; }
inline constexpr int32 operator""_kg(unsigned long long val) { return val * 1000; }

//==================

#define STRINGIFY2(X) STRINGIFY(X)
#define STRINGIFY(X) #X

//====================
// OS Macros
//====================

#define OPERATING_SYSTEM_OTHER    0
#define OPERATING_SYSTEM_LINUX    1
#define OPERATING_SYSTEM_WINDOWS  2
#define OPERATING_SYSTEM_MACOS    3

//====================
// SIMD GLM
//====================

#ifdef RELEASE_BUILD
    #define GLM_FORCE_INTRINSICS
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define OPERATING_SYSTEM OPERATING_SYSTEM_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
    #define OPERATING_SYSTEM OPERATING_SYSTEM_MACOS
#elif defined(__linux__)
    #define OPERATING_SYSTEM OPERATING_SYSTEM_LINUX
#else
    #define OPERATING_SYSTEM OPERATING_SYSTEM_OTHER
    #error "Unsupported operating system for this project."
#endif


#define ENUM_CLASS_FLAGS(EnumType)                                                \
using T = std::underlying_type_t<EnumType>;                                       \
inline constexpr EnumType operator|(EnumType lhs, EnumType rhs) {                 \
    return static_cast<EnumType>(static_cast<T>(lhs) | static_cast<T>(rhs));      \
}                                                                                 \
inline constexpr EnumType& operator|=(EnumType& lhs, EnumType rhs) {              \
    lhs = lhs | rhs;                                                              \
    return lhs;                                                                   \
}                                                                                 \
inline constexpr EnumType operator&(EnumType lhs, EnumType rhs) {                 \
    return static_cast<EnumType>(static_cast<T>(lhs) & static_cast<T>(rhs));      \
}                                                                                 \
inline constexpr EnumType& operator&=(EnumType& lhs, EnumType rhs) {              \
    lhs = lhs & rhs;                                                              \
    return lhs;                                                                   \
}                                                                                 \
inline constexpr EnumType operator~(EnumType rhs) {                               \
    return static_cast<EnumType>(~static_cast<T>(rhs));                           \
}                                                                                 \
inline constexpr bool Any(EnumType value) {                                       \
    return static_cast<T>(value) != 0;                                            \
}                                                                                 \
inline constexpr T cast(EnumType value)                                           \
{                                                                                 \
    return static_cast<T>(value);                                                 \
} 


#endif
