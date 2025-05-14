#pragma once

#include <vulkan/vulkan_core.h>
#include "../Core/Interfaces/ILogger.h"
#include "JsonSerializer.h"
#include <memory>

namespace Fractals::Infrastructure
{
    class Vulkan;
    using SharedVulkan = std::shared_ptr<Vulkan>;
    #define MAKE_SHARED_VULKAN std::make_shared<Vulkan>

    class Vulkan
	{
	public:
        // Constructors
        Vulkan() = delete;

        Vulkan(const Fractals::Core::Interfaces::SharedILogger&, const Fractals::Infrastructure::SharedJsonSerializer&);

        // Copy constructors
        Vulkan(const Vulkan&) = delete;

        Vulkan(Vulkan&&) = delete;

        // Operators
        Vulkan& operator=(const Vulkan&) = delete;

        Vulkan& operator=(Vulkan&&) = delete;

        // Destructors
        ~Vulkan();

        // Methods
        static SharedVulkan Create(const Fractals::Core::Interfaces::SharedILogger&, const Fractals::Infrastructure::SharedJsonSerializer&);

        void LogPhysicalDevices() const;

        //void LogExtensions() const;

        //void LogLayers() const;

    private:
        // Methods
        void createInstance();

        //void createLogicDevice();

        // Fields
        VkInstance _instance;

        const Fractals::Core::Interfaces::SharedILogger _logger;

        const Fractals::Infrastructure::SharedJsonSerializer _jsonSerializer;
	};
}