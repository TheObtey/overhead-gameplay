#include "Window.h"
#include "Logger.hpp"
#include "Define.h"

#include <GLFW/glfw3.h>

Window::Window(int width, int height, std::string name) : 
	m_width(width),
	m_height(height),
	m_name(name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
}

Window::~Window()
{

}

void Window::Close()
{

}

void Window::Open()
{
	uptr<GLFWwindow> pWindow(glfwCreateWindow(m_width, m_height, m_name, NULL, NULL));

	if (window == NULL)
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Failed to create GLFW Window");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(pWindow.get());

	onOpenEvent();
}
