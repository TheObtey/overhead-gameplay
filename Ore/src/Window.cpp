#include "Window.h"
#include "Logger.hpp"
#include "Define.h"
#include "EventManager.h"
#include "Ore.h"

#define STB_IMAGE_IMPLEMENTATION
#include <dependencies/stb_image.h>

using namespace Ore;
std::unordered_map<GLFWwindow*, Window*> Window::s_windows = {};
Window::Window(int width, int height, std::string name, bool enableTransparency, bool visible)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, enableTransparency);
    glfwWindowHint(GLFW_VISIBLE, visible);
#ifdef DEBUG_BUILD
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,true);
#endif

    stbi_set_flip_vertically_on_load(true);

	m_width = width;
	m_height = height;
	m_name = name;
}

Window::~Window()
{
	Window::Close();
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

    Logger::LogWithLevel(LogLevel::WARNING,  "OpenGL Version : ", glGetString(GL_VERSION));
	Logger::LogWithLevel(LogLevel::WARNING,  "Renderer : ", glGetString(GL_RENDERER));

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#ifdef DEBUG_BUILD
	glfwSetWindowTitle(m_pWindow, reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Ore::MessageCallback, 0);
#endif

    s_windows[m_pWindow] = this;
	onOpenEvent();
}

void Window::Clear()
{
    glfwMakeContextCurrent(m_pWindow);
	Color blue = Color::DARK_BLUE;
	glClearColor(blue.r, blue.g, blue.b, blue.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Present()
{
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
	if (m_currentCursorState == CursorState::LOCKED)
		glfwSetCursorPos(m_pWindow, m_width / 2, m_height / 2);
}

void Window::SetSize(uint16 width, uint16 height)
{
    m_width = width;
    m_height = height;

    glfwSetWindowSize(m_pWindow, m_width, m_height);
	glfwSetWindowAspectRatio(m_pWindow, m_width, m_height);
}

void Window::SetCursorState(CursorState state)
{
	m_currentCursorState = state;
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, static_cast<int32>(state));
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
