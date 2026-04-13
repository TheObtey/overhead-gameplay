#include "Scripting/Proxies/MathsProxy.h"
#include "Registries/AutomaticRegisterProxy.hpp"

void MathsProxyBinding::Bind(Binder& binder)
{
    auto maths = binder.GetOrCreateNamespace("fmath");
    maths.new_usertype<glm::vec4>("vec4",
        sol::constructors<glm::vec4(), glm::vec4(float), glm::vec4(float, float, float, float)>(),
        "x", &glm::vec4::x,
        "y", &glm::vec4::y,
        "z", &glm::vec4::z,
        "w", &glm::vec4::z,
        sol::meta_function::addition, MathsProxy::vec4_addition_overload,
        sol::meta_function::subtraction, MathsProxy::vec4_subtraction_overload,
        sol::meta_function::multiplication, MathsProxy::vec4_multiplication_overload,
        sol::meta_function::division, MathsProxy::vec4_division_overload);

    maths.new_usertype<glm::vec3>("vec3",
        sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
        "x", &glm::vec3::x,
        "y", &glm::vec3::y,
        "z", &glm::vec3::z,
        sol::meta_function::addition, MathsProxy::vec3_addition_overload,
        sol::meta_function::subtraction, MathsProxy::vec3_subtraction_overload,
        sol::meta_function::multiplication, MathsProxy::vec3_multiplication_overload,
        sol::meta_function::division, MathsProxy::vec3_division_overload);

    maths.new_usertype<glm::vec2>("vec2",
        sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
        "x", &glm::vec2::x,
        "y", &glm::vec2::y,
        sol::meta_function::addition, MathsProxy::vec2_addition_overload,
        sol::meta_function::subtraction, MathsProxy::vec2_subtraction_overload,
        sol::meta_function::multiplication, MathsProxy::vec2_multiplication_overload,
        sol::meta_function::division, MathsProxy::vec2_division_overload);

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
    maths.set_function("Floor", MathsProxy::Floor);
    maths.set_function("Ceil", MathsProxy::Floor);
    maths.set_function("Rad", MathsProxy::RadToDeg);
    maths.set_function("Deg", MathsProxy::DegToRad);
    maths.set_function("Random", MathsProxy::Random);
    maths.set_function("Cos", MathsProxy::Cos);
    maths.set_function("Acos", MathsProxy::ACos);
    maths.set_function("Sin", MathsProxy::Sin);
    maths.set_function("Asin", MathsProxy::ASin);
    maths.set_function("Tan", MathsProxy::Tan);
    maths.set_function("Atan", MathsProxy::ATan);
    maths.set_function("Atan2", MathsProxy::ATan2);
    maths.set_function("Sqrt", MathsProxy::Sqrt);
    maths.set_function("Exp", MathsProxy::Exp);
    maths.set_function("Pow", MathsProxy::Pow);
    maths.set_function("Log", MathsProxy::Log);

    maths.set("Pi", pi);
}
