#include "Window.h"
#include <cstdint>
#include "../Core/Interfaces/ILogger.h"

namespace Fractals::Infrastructure
{
	Window::Window(const Fractals::Core::Interfaces::SharedILogger& logger, GLFWwindow*const& window)
		: _logger(logger), _window(window) {}

	Window::~Window()
	{
		glfwDestroyWindow(_window);

		glfwTerminate();

		_logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("glfw", "window destroyed"));
	}

	SharedWindow Window::Create(const Fractals::Core::Interfaces::SharedILogger& logger, const uint32_t& width, const uint32_t& height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		const auto window = glfwCreateWindow(width, height, "Fractals", nullptr, nullptr);

		logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("glfw", "window created"));

		return MAKE_SHARED_WINDOW(logger, window);
	}

	HWND Window::GetHwnd()
	{
		return glfwGetWin32Window(_window);
	}

	void Window::Loop()
	{
		while (!glfwWindowShouldClose(_window))
			glfwPollEvents();
	}
}