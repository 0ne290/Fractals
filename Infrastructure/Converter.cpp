#include "Converter.h"

namespace Fractals::Infrastructure
{
    std::string Converter::ToString(const VkPhysicalDeviceType& deviceType) {
        static const std::string stringsByDeviceType[5] = {
            "OTHER", "INTEGRATED_GPU", "DISCRETE_GPU", "VIRTUAL_GPU", "CPU"
        };

        return stringsByDeviceType[deviceType];
    }
}