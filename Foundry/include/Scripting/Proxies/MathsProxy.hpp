#ifndef FOUNDRY_GLMPROXY__H_
#define FOUNDRY_GLMPROXY__H_

#include <random>

#include "Registries/AutomaticRegisterProxy.hpp"
#include "Scripting/Binder.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <sol/sol.hpp>

namespace MathsProxy
{
    inline auto vec3_addition_overload = sol::overload(
        [](glm::vec3 const& left, glm::vec3 const& right) { return left + right ; },
        [](glm::vec3 const& left, float right)      { return left + right ; },
        [](float left, glm::vec3 const& right)      { return left + right ; }
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

    inline auto Length =  sol::overload(
        [](glm::vec3 const& vec) { return glm::length(vec); }
    );

    inline auto Normalized =  sol::overload(
        [](glm::vec3 const& vec) { return glm::normalize(vec); }
    );

    inline auto Equal =  sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::equal(vec1, vec2); }
    );

    inline auto Dot = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::dot(vec1, vec2); }
    );

    inline auto Cross = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::cross(vec1, vec2); }
    );

    inline auto Lerp = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2, float const delta) { return (1.0f - delta) * vec1 + delta * vec2; },
        [](float const a, float const b, float const c){ return std::lerp(a, b, c); }
    );

    inline auto Abs = sol::overload(
        [](glm::vec3 const& vec) { return glm::abs(vec); },
        [](float const a){ return glm::abs(a); },
        [](int32 const a){ return glm::abs(a); }
    );

    inline auto Min = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::min(vec1, vec2); },
        [](glm::vec3 const& vec, float const a) { return glm::min(vec, a); },
        [](float const a, glm::vec3 const& vec) { return glm::min(vec, a); },
        [](float const a, float const b) { return glm::min(a, b); }
    );

    inline auto Max = sol::overload(
        [](glm::vec3 const& vec1, glm::vec3 const& vec2) { return glm::max(vec1, vec2); },
        [](glm::vec3 const& vec, float const a) { return glm::max(vec, a); },
        [](float const a, glm::vec3 const& vec) { return glm::max(vec, a); },
        [](float const a, float const b) { return glm::max(a, b); }
    );

    inline auto Clamp = sol::overload(
        [](glm::vec3 const& vec, glm::vec3 const& min, glm::vec3 const& max) { return glm::clamp(vec, min, max); },
        [](glm::vec3 const& vec, float const min, float const max) { return glm::clamp(vec, min, max); },
        [](float const value, float const min, float const max) { return glm::clamp(value, min, max); }
    );

    inline auto Random = [](int32 const min, int32 const max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());\
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    };
};

struct Vec3ProxyBinder : AutomaticRegisterProxy<Vec3ProxyBinder> {
    static void Bind(Binder& binder)
    {
        auto maths = binder.GetOrCreateNamespace("maths");
        maths.new_usertype<glm::vec3>("vec3",
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z,
            sol::meta_function::addition,      MathsProxy::vec3_addition_overload,
            sol::meta_function::subtraction,   MathsProxy::vec3_subtraction_overload,
            sol::meta_function::multiplication,MathsProxy::vec3_multiplication_overload,
            sol::meta_function::division,      MathsProxy::vec3_division_overload);

        maths.set_function("Length", MathsProxy::Length);
        maths.set_function("Normalized", MathsProxy::Normalized);
        maths.set_function("Equal", MathsProxy::Equal);
        maths.set_function("Dot", MathsProxy::Dot);
        maths.set_function("Cross", MathsProxy::Cross);
        maths.set_function("Lerp", MathsProxy::Lerp);
        maths.set_function("Abs", MathsProxy::Abs);
        maths.set_function("Min", MathsProxy::Min);
        maths.set_function("Max", MathsProxy::Max);
        maths.set_function("Clamp", MathsProxy::Clamp);
        maths.set("Random", MathsProxy::Random);
    }
};

inline static Vec3ProxyBinder Vec3Binding {};

#endif