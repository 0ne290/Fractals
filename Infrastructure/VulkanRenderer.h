#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan_core.h>
#include "../Core/Interfaces/ILogger.h"
#include "JsonSerializer.h"
#include <memory>
#include "Converter.h"
#include <windows.h>

namespace Fractals::Infrastructure
{
    class VulkanRenderer;
    using SharedVulkan = std::shared_ptr<VulkanRenderer>;
    #define MAKE_SHARED_VULKAN_RENDERER std::make_shared<VulkanRenderer>

    class VulkanRenderer
	{
	public:
        // Constructors
        VulkanRenderer() = delete;

        VulkanRenderer(const Fractals::Core::Interfaces::SharedILogger&, const Fractals::Infrastructure::SharedJsonSerializer&, const Fractals::Infrastructure::SharedConverter&);

        // Copy constructors
        VulkanRenderer(const VulkanRenderer&) = delete;

        VulkanRenderer(VulkanRenderer&&) = delete;

        // Operators
        VulkanRenderer& operator=(const VulkanRenderer&) = delete;

        VulkanRenderer& operator=(VulkanRenderer&&) = delete;

        // Destructors
        ~VulkanRenderer();

        // Methods
        static SharedVulkan Create(const Fractals::Core::Interfaces::SharedILogger&,
            const Fractals::Infrastructure::SharedJsonSerializer&,
            const Fractals::Infrastructure::SharedConverter&,
            const SharedGlfwWindow window);

        //void LogExtensions() const;

        //void LogLayers() const;

    private:
        // Methods
        void setupInstance(const SharedVector<const char*>);

        void setupPhysicalDevice();

        void setupLogicalDeviceAndQueues();

        void setupSwapchain();

        //void createLogicDevice();

        // Fields
        VkInstance _instance;

        VkSurfaceKHR _surface;

        VkPhysicalDevice _physicalDevice;

        VkDevice _logicalDevice;

        VkQueue _graphicQueue, _presentationQueue;

        VkSwapchainKHR _swapchain;

        const Fractals::Core::Interfaces::SharedILogger _logger;

        const Fractals::Infrastructure::SharedJsonSerializer _jsonSerializer;

        const Fractals::Infrastructure::SharedConverter _converter;
	};
}