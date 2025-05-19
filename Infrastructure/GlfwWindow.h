#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <memory>
#include <cstdint>
#include "../Core/Interfaces/ILogger.h"

namespace Fractals::Infrastructure
{
    class GlfwWindow;
    using SharedGlfwWindow = std::shared_ptr<GlfwWindow>;
    #define MAKE_SHARED_GLFW_WINDOW std::make_shared<GlfwWindow>

    class GlfwWindow
    {
    public:
        // Constructors
        GlfwWindow() = delete;

        GlfwWindow(const Fractals::Core::Interfaces::SharedILogger&, const Fractals::Infrastructure::SharedJsonSerializer&, const Fractals::Infrastructure::SharedConverter&, GLFWwindow* const&);

        // Copy constructors
        GlfwWindow(const GlfwWindow&) = delete;

        GlfwWindow(GlfwWindow&&) = delete;

        // Operators
        GlfwWindow& operator=(const GlfwWindow&) = delete;

        GlfwWindow& operator=(GlfwWindow&&) = delete;

        // Destructors
        ~GlfwWindow();

        // Methods
        static SharedGlfwWindow Create(const Fractals::Core::Interfaces::SharedILogger&, const uint32_t&, const uint32_t&);

        SharedVector<const char*> GetRequiredExtensionsToAttachVulkanRenderer();
        
        VkSurfaceKHR CreateVulkanSurface(VkInstance);

        void Loop();

    private:
        // Fields
        GLFWwindow* _window;

        const Fractals::Core::Interfaces::SharedILogger _logger;

		const Fractals::Infrastructure::SharedJsonSerializer _jsonSerializer;

		const Fractals::Infrastructure::SharedConverter _converter;
    };
}