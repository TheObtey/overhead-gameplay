#include "Window.h"
#include "Logger.hpp"
#include "Define.h"
#include "EventManager.h"
#include "Ore.h"

#define STB_IMAGE_IMPLEMENTATION
#include <dependencies/stb_image.h>

std::unordered_map<GLFWwindow*, Window*> Window::s_windows = {};
Window::Window(int width, int height, std::string name, bool enableTransparency, bool visible)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, enableTransparency);
    glfwWindowHint(GLFW_VISIBLE, visible);

    stbi_set_flip_vertically_on_load(true);

	m_width = width;
	m_height = height;
	m_name = name;
}

Window::~Window()
{
    Close();
}

void Window::SetIcon(std::string const& path)
{
    GLFWimage images[1];
    images[0].pixels = stbi_load(path.c_str(), &images[0].width, &images[0].height, 0, 4);
    glfwSetWindowIcon(m_pWindow, 1, images);
    stbi_image_free(images[0].pixels);
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
    glfwSetKeyCallback(m_pWindow, EventManager::GetKeyCallback);
    glfwSetMouseButtonCallback(m_pWindow, EventManager::GetMouseButtonCallBack);
    glfwSetCursorPosCallback(m_pWindow, EventManager::GetCursorPosCallBack);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::LogWithLevel(LogLevel::ERROR, "Failed to initialize GLAD");
		return;
	}

    Logger::LogWithLevel(LogLevel::WARNING, "OpenGL Version : ", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
#ifdef DEBUG_BUILD
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Ore::MessageCallback, 0);
#endif

    s_windows[m_pWindow] = this;
	onOpenEvent();
}

void Window::Clear()
{
    glfwMakeContextCurrent(m_pWindow);
    for(Viewport const* pViewport : m_viewports)
    {
        pViewport->Clear();
    }
}

void Window::Present()
{
    for(Viewport const* pViewport : m_viewports)
    {
        pViewport->Present();
    }

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
    pCurrentWindow->onResizeEvent(width, height);
}

void Window::SetDecoration(bool hasDecoration)
{
    glfwWindowHint(GLFW_DECORATED, hasDecoration);
}

void Window::RemoveViewport(Viewport const& viewport)
{
    auto it = std::find(m_viewports.begin(), m_viewports.end(), &viewport);
    if(it != m_viewports.end())
    {
        m_viewports.erase(it, m_viewports.end());
    }
}
