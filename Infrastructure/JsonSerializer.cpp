#include "JsonSerializer.h"
#include "Converter.h"
#include <format>
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>

namespace Fractals::Infrastructure
{
    SharedString JsonSerializer::ToJson(const SharedVector<VkPhysicalDevice> devices,
        void(* const getProperties)(const VkPhysicalDevice, VkPhysicalDeviceProperties*))
    {
        VkPhysicalDeviceProperties properties;
        SharedString ret = MAKE_SHARED_STRING("[");

        for (auto i = 0; i < devices->size() - 1; i++) {
            getProperties((*devices)[i], &properties);
            ret->append(std::format(
                R"({{"apiVersion":{},"driverVersion":{},"vendorId":{},"deviceId":{},"deviceType":"{}","deviceName":"{}"}},)",
                properties.apiVersion,
                properties.driverVersion,
                properties.vendorID,
                properties.deviceID,
                *Converter::ToString(properties.deviceType),
                properties.deviceName
            ));
        }

        getProperties((*devices)[devices->size() - 1], &properties);
        ret->append(std::format(
            R"({{"apiVersion":{},"driverVersion":{},"vendorId":{},"deviceId":{},"deviceType":"{}","deviceName":"{}"}})",
            properties.apiVersion,
            properties.driverVersion,
            properties.vendorID,
            properties.deviceID,
            *Converter::ToString(properties.deviceType),
            properties.deviceName
        ));

        ret->append("]");

        return ret;
    }
}