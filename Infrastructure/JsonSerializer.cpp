#include "JsonSerializer.h"
#include "Converter.h"
#include <format>
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>

namespace Fractals::Infrastructure
{
    JsonSerializer::JsonSerializer(const Fractals::Infrastructure::SharedConverter& converter)
        : _converter(converter) { }

    SharedJsonSerializer JsonSerializer::Create(const Fractals::Infrastructure::SharedConverter& converter)
    {
        return MAKE_SHARED_JSON_SERIALIZER(converter);
    }

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
                *_converter->ToString(properties.deviceType),
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
            *_converter->ToString(properties.deviceType),
            properties.deviceName
        ));

        ret->append("]");

        return ret;
    }

    SharedString JsonSerializer::ToJson(const SharedVector<VkLayerProperties> layers)
    {
        auto layersLastIndex = layers->size() - 1;

        SharedString ret = MAKE_SHARED_STRING("[");

        for (auto i = 0; i < layersLastIndex; i++) {
            auto& layer = (*layers)[i];
            ret->append(std::format(
                R"({{"layerName":"{}","specVersion":{},"implementationVersion":{},"description":"{}"}},)",
                layer.layerName,
                layer.specVersion,
                layer.implementationVersion,
                layer.description
            ));
        }

        auto& layer = (*layers)[layersLastIndex];
        ret->append(std::format(
            R"({{"layerName":"{}","specVersion":{},"implementationVersion":{},"description":"{}"}})",
            layer.layerName,
            layer.specVersion,
            layer.implementationVersion,
            layer.description
        ));

        ret->append("]");

        return ret;
    }

    SharedString JsonSerializer::ToJson(const SharedVector<VkExtensionProperties> extensions)
    {
        auto extensionsLastIndex = extensions->size() - 1;

        SharedString ret = MAKE_SHARED_STRING("[");

        for (auto i = 0; i < extensionsLastIndex; i++) {
            auto& extension = (*extensions)[i];
            ret->append(std::format(
                R"({{"extensionName":"{}","specVersion":{}}},)",
                extension.extensionName,
                extension.specVersion
            ));
        }

        auto& extension = (*extensions)[extensionsLastIndex];
        ret->append(std::format(
            R"({{"extensionName":"{}","specVersion":{}}})",
            extension.extensionName,
            extension.specVersion
        ));

        ret->append("]");

        return ret;
    }
}