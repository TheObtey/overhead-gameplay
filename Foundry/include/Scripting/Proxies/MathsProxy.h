#ifndef FOUNDRY_GLMPROXY__H_
#define FOUNDRY_GLMPROXY__H_

#include <random>

#include "Define.h"

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <sol/sol.hpp>

class Binder;

namespace MathsProxy
{

    //===================== vec2 =====================
    inline auto vec2_addition_overload = sol::overload(
        [](glm::vec2 const& left, glm::vec2 const& right) { return left + right ; },
        [](glm::vec2 const& left, float const right)      { return left + right ; },
        [](float const left, glm::vec2 const& right)      { return left + right ; }
    );

    inline auto vec2_subtraction_overload = sol::overload(
        [](glm::vec2 const& left, glm::vec2 const& right) { return left - right ; },
        [](glm::vec2 const& left, float const right)      { return left - right ; },
        [](float const left, glm::vec2 const& right)      { return left - right ; }
    );

    inline auto vec2_multiplication_overload = sol::overload(
        [](glm::vec2 const& left, glm::vec2 const& right) { return left * right ; },
        [](glm::vec2 const& left, float const right)      { return left * right ; },
        [](float const left, glm::vec2 const& right)      { return left * right ; }
    );

    inline auto vec2_division_overload = sol::overload(
        [](glm::vec2 const& left, glm::vec2 const& right) { return left / right ; },
        [](glm::vec2 const& left, float const right)      { return left / right ; },
        [](float const left, glm::vec2 const& right)      { return left / right ; }
    );

    //===================== vec3 =====================

    inline auto vec3_addition_overload = sol::overload(
        [](glm::vec3 const& left, glm::vec3 const& right) { return left + right ; },
        [](glm::vec3 const& left, float const right)      { return left + right ; },
        [](float const left, glm::vec3 const& right)      { return left + right ; }
    );

    inline auto vec3_subtraction_overload = sol::overload(
        [](glm::vec3 const& left, glm::vec3 const& right) { return left - right ; },
        [](glm::vec3 const& left, float const right)      { return left - right ; },
        [](float const left, glm::vec3 const& right)      { return left - right ; }
    );

    inline auto vec3_multiplication_overload = sol::overload(
        [](glm::vec3 const& left, glm::vec3 const& right) { return left * right ; },
        [](glm::vec3 const& left, float const right)      { return left * right ; },
        [](float const left, glm::vec3 const& right)      { return left * right ; }
    );

    inline auto vec3_division_overload = sol::overload(
        [](glm::vec3 const& left, glm::vec3 const& right) { return left / right ; },
        [](glm::vec3 const& left, float const right)      { return left / right ; },
        [](float const left, glm::vec3 const& right)      { return left / right ; }
    );

    //===================== vec4 =====================

    inline auto vec4_addition_overload = sol::overload(
        [](glm::vec4 const& left, glm::vec4 const& right) { return left + right ; },
        [](glm::vec4 const& left, float const right)      { return left + right ; },
        [](float const left, glm::vec4 const& right)      { return left + right ; }
    );

    inline auto vec4_subtraction_overload = sol::overload(
        [](glm::vec4 const& left, glm::vec4 const& right) { return left - right ; },
        [](glm::vec4 const& left, float const right)      { return left - right ; },
        [](float const left, glm::vec4 const& right)      { return left - right ; }
    );

    inline auto vec4_multiplication_overload = sol::overload(
        [](glm::vec4 const& left, glm::vec4 const& right) { return left * right ; },
        [](glm::vec4 const& left, float const right)      { return left * right ; },
        [](float const left, glm::vec4 const& right)      { return left * right ; }
    );

    inline auto vec4_division_overload = sol::overload(
        [](glm::vec4 const& left, glm::vec4 const& right) { return left / right ; },
        [](glm::vec4 const& left, float const right)      { return left / right ; },
        [](float const left, glm::vec4 const& right)      { return left / right ; }
    );


    inline auto Length =  sol::overload(
        [](glm::vec4 const& vec) { return glm::length(vec); },
        [](glm::vec3 const& vec) { return glm::length(vec); },
        [](glm::vec2 const& vec) { return glm::length(vec); }
    );

    inline auto Normalized =  sol::overload(
        [](glm::vec4 const& vec) { return glm::normalize(vec); },
        [](glm::vec3 const& vec) { return glm::normalize(vec); },
        [](glm::vec2 const& vec) { return glm::normalize(vec); }
    );

    inline auto Equal =  sol::overload(
        [](glm::vec4 const& vec1, glm::vec4 const& vec2) { return glm::equal(vec1, vec2); },
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::equal(vec1, vec2); },
        [](glm::vec2 const& vec1, glm::vec2 const& vec2) { return glm::equal(vec1, vec2); }
    );

    inline auto Dot = sol::overload(
        [](glm::vec4 const& vec1, glm::vec4 const& vec2) { return glm::dot(vec1, vec2); },
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::dot(vec1, vec2); },
        [](glm::vec2 const& vec1, glm::vec2 const& vec2) { return glm::dot(vec1, vec2); }
    );

    inline auto Cross = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::cross(vec1, vec2); }
    );

    inline auto Lerp = sol::overload(
        [](glm::vec4 const& vec1, glm::vec4 const& vec2, float const delta) { return (1.0f - delta) * vec1 + delta * vec2; },
        [](glm::vec3 const& vec1, glm::vec3 const& vec2, float const delta) { return (1.0f - delta) * vec1 + delta * vec2; },
        [](glm::vec2 const& vec1, glm::vec2 const& vec2, float const delta) { return (1.0f - delta) * vec1 + delta * vec2; },
        [](float const a, float const b, float const c){ return std::lerp(a, b, c); }
    );

    inline auto Abs = sol::overload(
        [](glm::vec4 const& vec) { return glm::abs(vec); },
        [](glm::vec3 const& vec) { return glm::abs(vec); },
        [](glm::vec2 const& vec) { return glm::abs(vec); },
        [](float const a){ return glm::abs(a); },
        [](int32 const a){ return glm::abs(a); }
    );

    inline auto Min = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::min(vec1, vec2); },
        [](glm::vec3 const& vec, float const a) { return glm::min(vec, a); },
        [](float const a, glm::vec3 const& vec) { return glm::min(vec, a); },
        [](float const a, float const b) { return glm::min(a, b); },
        [](int32 const a, int32 const b) { return glm::min(a, b); }
    );

    inline auto Max = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::max(vec1, vec2); },
        [](glm::vec3 const& vec, float const a) { return glm::max(vec, a); },
        [](float const a, glm::vec3 const& vec) { return glm::max(vec, a); },
        [](float const a, float const b) { return glm::max(a, b); },
        [](int32 const a, int32 const b) { return glm::max(a, b); }
    );

    inline auto Clamp = sol::overload(
        [](glm::vec3 const& vec, glm::vec3 const& min, glm::vec3 const& max) { return glm::clamp(vec, min, max); },
        [](glm::vec3 const& vec, float const min, float const max) { return glm::clamp(vec, min, max); },
        [](float const value, float const min, float const max) { return glm::clamp(value, min, max); },
        [](int32 const value, int32 const min, int32 const max) { return glm::clamp(value, min, max); }
    );

    inline auto Floor = sol::overload(
        [](glm::vec4 const& vec) { return glm::floor(vec); },
        [](glm::vec3 const& vec) { return glm::floor(vec); },
        [](glm::vec2 const& vec) { return glm::floor(vec); },
        [](float const& v) { return glm::floor(v); }
    );

    inline auto Ceil = sol::overload(
        [](glm::vec4 const& vec) { return glm::ceil(vec); },
        [](glm::vec3 const& vec) { return glm::ceil(vec); },
        [](glm::vec2 const& vec) { return glm::ceil(vec); },
        [](float const& v) { return glm::ceil(v); }
    );

    inline auto Random = [](int32 const min, int32 const max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());\
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    };

    inline auto RadToDeg = sol::overload(
        [](float const rad) { return glm::degrees(rad); },
        [](double const rad) { return glm::degrees(rad); }
    );

    inline auto DegToRad = sol::overload(
        [](float const deg) { return glm::radians(deg); },
        [](double const deg) { return glm::radians(deg); }
    );

    inline auto Cos = sol::overload(
        [](const float theta) { return std::cos(theta); },
        [](const double theta) { return std::cos(theta); }
    );

    inline auto ACos = sol::overload(
        [](const float theta) { return std::acos(theta); },
        [](const double theta) { return std::acos(theta); }
    );

    inline auto Sin = sol::overload(
        [](const float theta) { return std::sin(theta); },
        [](const double theta) { return std::sin(theta); }
    );

    inline auto ASin = sol::overload(
        [](const float theta) { return std::asin(theta); },
        [](const double theta) { return std::asin(theta); }
    );

    inline auto Tan = sol::overload(
        [](const float theta) { return std::tan(theta); },
        [](const double theta) { return std::tan(theta); }
    );

    inline auto ATan = sol::overload(
        [](const float theta) { return std::atan(theta); },
        [](const double theta) { return std::atan(theta); }
    );

    inline auto ATan2 = sol::overload(
        [](const float x, const float y) { return std::atan2(y, x); },
        [](const float x, const float y) { return std::atan2(y, x);}
    );

    inline auto Sqrt = sol::overload(
        [](const glm::vec4 v) { return glm::sqrt(v); },
        [](const glm::vec3 v) { return glm::sqrt(v); },
        [](const glm::vec2 v) { return glm::sqrt(v); },
        [](const float v) { return glm::sqrt(v); },
        [](const double v) { return glm::sqrt(v); }
    );

    inline auto Exp = sol::overload(
        [](const glm::vec4 v) { return glm::exp(v); },
        [](const glm::vec3 v) { return glm::exp(v); },
        [](const glm::vec2 v) { return glm::exp(v); },
        [](const float v) { return glm::exp(v); },
        [](const double v) { return glm::exp(v); }
    );

    inline auto Pow = sol::overload(
        [](const glm::vec4 v, const glm::vec4 expo) { return glm::pow(v, expo); },
        [](const glm::vec3 v, const glm::vec3 expo) { return glm::pow(v, expo); },
        [](const glm::vec2 v, const glm::vec2 expo) { return glm::pow(v, expo); },
        [](const float v, const float expo) { return glm::pow(v, expo); },
        [](const int v, const int expo) { return glm::pow(v, expo); },
        [](const double v, const double expo) { return glm::pow(v, expo); }
    );

    inline auto Log = sol::overload(
        [](const glm::vec4& v) { return glm::log(v); },
        [](const glm::vec3& v) { return glm::log(v); },
        [](const glm::vec2& v) { return glm::log(v); },
        [](const float v) { return glm::log(v); },
        [](const double v) { return glm::log(v); }
    );
};

struct MathsProxyBinding 
{
    static void Bind(Binder& binder);
};


#endif