#include "Converter.h"
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include <string>
#include "../Core/Interfaces/ILogger.h"

namespace Fractals::Infrastructure
{
    Converter::Converter(const Fractals::Core::Interfaces::SharedILogger& logger)
        : _logger(logger) { }

    SharedConverter Converter::Create(const Fractals::Core::Interfaces::SharedILogger& logger)
    {
        return MAKE_SHARED_CONVERTER(logger);
    }

    SharedString Converter::ToString(const VkPhysicalDeviceType deviceType)
    {
        switch (deviceType)
        {
            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_OTHER:
                return MAKE_SHARED_STRING("OTHER");

            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                return MAKE_SHARED_STRING("INTEGRATED_GPU");

            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                return MAKE_SHARED_STRING("DISCRETE_GPU");

            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                return MAKE_SHARED_STRING("VIRTUAL_GPU");

            case VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_CPU:
                return MAKE_SHARED_STRING("CPU");
                
            default:
                _logger->Error(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("converter", "unknown VkPhysicalDeviceType"));

                return MAKE_SHARED_STRING("UNKNOWN");
        }
    }
}