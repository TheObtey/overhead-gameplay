#ifndef ORE_ORE__H_
#define ORE_ORE__H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Ore
{
    void GLAPIENTRY MessageCallback(GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );
}

#endif
