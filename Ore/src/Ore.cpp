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
    std::string severityStr;
    switch(severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION :
        severityStr = "NOTIFICATION";
        break;
    case GL_DEBUG_SEVERITY_LOW :
        severityStr = "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM :
        severityStr = "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH :
        severityStr = "HIGHT";
        break;
    }

    switch(type)
    {
    case GL_DEBUG_TYPE_OTHER:
        Logger::LogWithLevel(LogLevel::WARNING, "GL CALLBACK : " "**GL OTHER**","\ntype = ", type, "\nseverity = ", severityStr, "\nmessage = ", message);
        break;

    case GL_DEBUG_TYPE_ERROR:
        Logger::LogWithLevel(LogLevel::ERROR, "GL CALLBACK: ", "** GL ERROR **", "\ntype = ", type, "\nseverity = ", severity, "\nmessage = ", message);
        break;
    }
}
