#include "Window.h"
#include "Logger.hpp"
#include "Define.h"
#include <glad/glad.h>
#include "EventManager.h"

void GLAPIENTRY
MessageCallback( GLenum source,
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

std::unordered_map<GLFWwindow*, Window*> Window::s_windows = {};
Window::Window(int width, int height, std::string name, bool enableTransparency)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, enableTransparency);

	m_width = width;
	m_height = height;
	m_name = name;

	Open();
}

Window::~Window()
{

}

bool Window::IsOpen()
{
	if (m_pWindow == nullptr)
		return false;

	return !glfwWindowShouldClose(m_pWindow);
}

void Window::Close()
{
	onCloseEvent();
	glfwTerminate();
}

void Window::Open()
{
	m_pWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);

	if (m_pWindow == NULL)
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Failed to create GLFW Window");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_pWindow);

	glfwSetFramebufferSizeCallback(m_pWindow, Window::FrameBufferResizeCallback);
    glfwSetJoystickCallback(EventManager::JoystickCallback);

#ifdef DEBUG_BUILD
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
#endif

    s_windows[m_pWindow] = this;
	onOpenEvent();
}

void Window::Present()
{
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

void Window::SetSize(uint16 width, uint16 height)
{
    m_width = width;
    m_height = height;

    glfwSetWindowSize(m_pWindow, m_width, m_height);
}

void Window::FrameBufferResizeCallback(GLFWwindow* pWindow, int width, int height)
{
    Window* pCurrentWindow = s_windows[pWindow];
    pCurrentWindow->onResizeEvent();
}

void Window::SetDecoration(bool hasDecoration)
{
    glfwWindowHint(GLFW_DECORATED, hasDecoration);
}
