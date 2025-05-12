#pragma once

#include <vulkan/vulkan_core.h>
#include <memory>
#include "../Core/Interfaces/ILogger.h"

using std::shared_ptr;

namespace Fractals::Infrastructure
{
	class Vulkan
	{
	public:
        // Constructors
        Vulkan() = delete;

        Vulkan(const VkInstance&, const shared_ptr<Fractals::Core::Interfaces::ILogger>&);

        // Copy constructors
        Vulkan(const Vulkan&) = delete;

        Vulkan(Vulkan&&) = delete;

        // Operators
        Vulkan& operator=(const Vulkan&) = delete;

        Vulkan& operator=(Vulkan&&) = delete;

        // Destructors
        ~Vulkan();

        // Methods
        static shared_ptr<Vulkan> Create(const shared_ptr<Fractals::Core::Interfaces::ILogger>&);

        void LogPhysicalDevices() const;

        //void LogExtensions() const;

        //void LogLayers() const;

    private:
        // Methods
        static VkInstance createInstance();

        // Fields
        const VkInstance _instance;

        const shared_ptr<Fractals::Core::Interfaces::ILogger> _logger;
	};
}