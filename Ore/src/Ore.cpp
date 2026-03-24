#include "Ore.h"
#include "Logger.hpp"

void GLAPIENTRY Ore::MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    Logger::Log("GL CALLBACK: ",( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) ,
            " type = ", type,
            " severity = ", severity,
            " message = ", message);
}
