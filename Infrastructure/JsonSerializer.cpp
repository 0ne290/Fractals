#include "JsonSerializer.h"
#include "Converter.h"
#include <vulkan/vulkan_core.h>
#include <string>
#include <vector>
#include <format>

namespace Fractals::Infrastructure
{
    std::string JsonSerializer::ToJson(const std::vector<VkPhysicalDevice>& devices, void
    (*&getProperties)(const VkPhysicalDevice&, VkPhysicalDeviceProperties&)) {
        VkPhysicalDeviceProperties properties;
        std::string ret("[");

        for (auto i = 0; i < devices.size() - 1; i++) {
            getProperties(devices[i], properties);
            ret.append(std::format(
                R"({{"apiVersion":{},"driverVersion":{},"vendorId":{},"deviceId":{},"deviceType":"{}","deviceName":"{}"}},)",
                properties.apiVersion,
                properties.driverVersion,
                properties.vendorID,
                properties.deviceID,
                Converter::ToString(properties.deviceType),
                properties.deviceName
            ));
        }

        getProperties(devices[devices.size() - 1], properties);
        ret.append(std::format(
            R"({{"apiVersion":{},"driverVersion":{},"vendorId":{},"deviceId":{},"deviceType":"{}","deviceName":"{}"}})",
            properties.apiVersion,
            properties.driverVersion,
            properties.vendorID,
            properties.deviceID,
            Converter::ToString(properties.deviceType),
            properties.deviceName
        ));

        ret.append("]");

        return ret;
    }
}