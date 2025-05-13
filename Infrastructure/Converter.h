#pragma once
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>

namespace Fractals::Infrastructure
{
	class Converter
	{
	public:
        // Constructors
        Converter() = delete;

        // Copy constructors
        Converter(const Converter&) = delete;

        Converter(Converter&&) = delete;

        // Operators
        Converter& operator=(const Converter&) = delete;

        Converter& operator=(Converter&&) = delete;

        // Destructors
        ~Converter() = delete;

        // Methods
        static SharedString ToString(const VkPhysicalDeviceType);
	};
}