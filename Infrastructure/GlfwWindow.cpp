#include "GlfwWindow.h"
#include <cstdint>
#include "../Core/Interfaces/ILogger.h"
#include <iostream>
#include "../Core/Exceptions/Critical.h"
#include "JsonSerializer.h"

namespace Fractals::Infrastructure
{
	GlfwWindow::GlfwWindow(const Fractals::Core::Interfaces::SharedILogger& logger, const Fractals::Infrastructure::SharedJsonSerializer& jsonSerializer, const Fractals::Infrastructure::SharedConverter& converter, GLFWwindow* const& window)
		: _logger(logger), _jsonSerializer(jsonSerializer), _converter(converter), _window(window) {
	}

	GlfwWindow::~GlfwWindow()
	{
		glfwDestroyWindow(_window);

		glfwTerminate();

		_logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("glfw", "window destroyed"));
	}

	SharedGlfwWindow GlfwWindow::Create(const Fractals::Core::Interfaces::SharedILogger& logger, const uint32_t& width, const uint32_t& height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		const auto window = glfwCreateWindow(width, height, "Fractals", nullptr, nullptr);

		logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("glfw", "window created"));

		return MAKE_SHARED_GLFW_WINDOW(logger, window);
	}

	SharedVector<const char*> GlfwWindow::GetRequiredExtensionsToAttachVulkanRenderer()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		return MAKE_SHARED_VECTOR(const char*)(glfwExtensions, glfwExtensions + glfwExtensionCount);
	}

	VkSurfaceKHR GlfwWindow::CreateVulkanSurface(VkInstance vulkanInstance)
	{
		VkSurfaceKHR ret;
		const auto result = glfwCreateWindowSurface(vulkanInstance, _window, nullptr, &ret);
		if (result != VK_SUCCESS)
			throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("glfw", "create surface error", *_jsonSerializer->WrapInQuotes(_converter->ToString(result))));

		return ret;
	}

	void GlfwWindow::Loop()
	{
		while (!glfwWindowShouldClose(_window))
			glfwPollEvents();
	}
}