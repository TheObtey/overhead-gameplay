#ifndef FOUNDRY_GLMPROXY__H_
#define FOUNDRY_GLMPROXY__H_

#include "Registries/AutomaticRegisterProxy.hpp"
#include "Scripting/Binder.h"

#include <glm/vec3.hpp>
#include <sol/sol.hpp>

struct Vec3ProxyBinder : AutomaticRegisterProxy<Vec3ProxyBinder> {
    static void Bind(Binder& binder)
    {
        binder.BindClass<glm::vec3>("Vec3",
            sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z);
    }
};

#endif