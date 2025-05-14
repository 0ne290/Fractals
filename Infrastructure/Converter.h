#pragma once
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include "../Core/Interfaces/ILogger.h"

namespace Fractals::Infrastructure
{
	class Converter
	{
	public:
        // Constructors
        Converter() = delete;

        Converter(const Fractals::Core::Interfaces::SharedILogger&);

        // Copy constructors
        Converter(const Converter&) = delete;

        Converter(Converter&&) = delete;

        // Operators
        Converter& operator=(const Converter&) = delete;

        Converter& operator=(Converter&&) = delete;

        // Destructors
        ~Converter() = delete;

        // Methods
        SharedString ToString(const VkPhysicalDeviceType);

    private:
        // Fields
        const Fractals::Core::Interfaces::SharedILogger _logger;
	};
}