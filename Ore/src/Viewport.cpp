#include "Viewport.h"
#include "Logger.hpp"
#include "Window.h"
#include "Passes/Pass.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void GLAPIENTRY
OpenGLMessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
    if(type == GL_DEBUG_TYPE_ERROR)
        Logger::LogWithLevel(LogLevel::ERROR, "GL ERROR :", "type : ", type, "severity : ", severity, "message : ", message); 
}

Viewport::Viewport(uint16 x, uint16 y, uint16 width, uint16 height, Color const& backgroundColor, Window& window)
{
    m_x = x;
    m_y = y;
    m_width = width;
    m_height = height;
    m_backgroundColor = backgroundColor;
    m_pWindow = std::make_shared<Window>(window);

    Display();
    m_pRenderGraph = std::make_unique<RenderGraph>(width, height);
}

Viewport::~Viewport()
{
}

void Viewport::AddPass(Pass &pass)
{
    m_pRenderGraph->AddPass(pass);
}

void Viewport::SetSize(uint16 width, uint16 height)
{
    m_width = width;
    m_height = height;
}

void Viewport::Display()
{
    glfwMakeContextCurrent(m_pWindow.get()->Get());

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Failed to initialize GLAD");
		return;
	}
    glEnable(GL_DEPTH_TEST);

#ifdef DEBUG_BUILD
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(OpenGLMessageCallback, 0);
#endif
    glViewport(m_x, m_y, m_width, m_height);
}

void Viewport::Clear()
{
    glfwMakeContextCurrent(m_pWindow.get()->Get());

    glEnable(GL_SCISSOR_TEST);
    glScissor(m_x, m_y, m_width, m_height);
    glClearColor(m_backgroundColor.r, m_backgroundColor.g, m_backgroundColor.b, m_backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
}

void Viewport::Present()
{
    m_pRenderGraph->Execute();
}
