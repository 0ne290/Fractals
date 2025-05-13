#include "Converter.h"
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include <string>

namespace Fractals::Infrastructure
{
    SharedString Converter::ToString(const VkPhysicalDeviceType deviceType) {
        static const std::string stringsByDeviceType[5] = {
            "OTHER", "INTEGRATED_GPU", "DISCRETE_GPU", "VIRTUAL_GPU", "CPU"
        };

        return MAKE_SHARED_STRING(stringsByDeviceType[deviceType]);
    }
}