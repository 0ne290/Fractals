#pragma once
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include "../Core/Interfaces/ILogger.h"
#include <memory>

namespace Fractals::Infrastructure
{
    class Converter;
    using SharedConverter = std::shared_ptr<Converter>;
    #define MAKE_SHARED_CONVERTER std::make_shared<Converter>

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
        ~Converter() = default;

        // Methods
        static SharedConverter Create(const Fractals::Core::Interfaces::SharedILogger&);

        SharedString ToString(const VkPhysicalDeviceType);

        SharedString ToString(const VkResult);

    private:
        // Fields
        const Fractals::Core::Interfaces::SharedILogger _logger;
	};
}