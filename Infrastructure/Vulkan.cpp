#include "Vulkan.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Core/Exceptions/Critical.h"
#include "JsonSerializer.h"
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include <cstdint>

namespace Fractals::Infrastructure
{
    Vulkan::Vulkan(const Fractals::Core::Interfaces::SharedILogger& logger, const Fractals::Infrastructure::SharedJsonSerializer& jsonSerializer, const Fractals::Infrastructure::SharedConverter& converter)
        : _logger(logger), _jsonSerializer(jsonSerializer), _converter(converter) {
    }

    SharedVulkan Vulkan::Create(const Fractals::Core::Interfaces::SharedILogger& logger, const Fractals::Infrastructure::SharedJsonSerializer& jsonSerializer, const Fractals::Infrastructure::SharedConverter& converter)
	{
        const auto ret = MAKE_SHARED_VULKAN(logger, jsonSerializer, converter);
		ret->setupInstance();
		logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "vulkan created"));

		return ret;
	}

	Vulkan::~Vulkan()
	{
        vkDestroyInstance(_instance, nullptr);
		_logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "vulkan destroyed"));
	}

    void Vulkan::setupInstance() {
        // Get layer names for enable them all
        uint32_t layerCount;
        auto result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        if (result != VK_SUCCESS) {
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "failed to get layer count", _jsonSerializer->ErrorDetailToJson(_converter->ToString(result))));
        }

        SharedVector<VkLayerProperties> layers = MAKE_SHARED_VECTOR(VkLayerProperties)(layerCount);
        SharedVector<const char*> layerNames = MAKE_SHARED_VECTOR(const char*)(layerCount);
        result = vkEnumerateInstanceLayerProperties(&layerCount, layers->data());
        if (result != VK_SUCCESS) {
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "failed to get layers", _jsonSerializer->ErrorDetailToJson(_converter->ToString(result))));
        }
        for (auto i = 0; i < layerCount; i++) {
            (*layerNames)[i] = (*layers)[i].layerName;
        }

        // Logging layers
        const auto layersJson = _jsonSerializer->ToJson(layers);
        _logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "layers", *layersJson));

        // Get extension names for enable them all
        uint32_t extensionCount;
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        if (result != VK_SUCCESS) {
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "failed to get extension count", _jsonSerializer->ErrorDetailToJson(_converter->ToString(result))));
        }

        SharedVector<VkExtensionProperties> extensions = MAKE_SHARED_VECTOR(VkExtensionProperties)(extensionCount);
        SharedVector<const char*> extensionNames = MAKE_SHARED_VECTOR(const char*)(extensionCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions->data());
        if (result != VK_SUCCESS) {
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "failed to get extensions", _jsonSerializer->ErrorDetailToJson(_converter->ToString(result))));
        }
        for (auto i = 0; i < extensionCount; i++) {
            (*extensionNames)[i] = (*extensions)[i].extensionName;
        }

        // Logging extensions
        const auto extensionsJson = _jsonSerializer->ToJson(extensions);
        _logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "extensions", *extensionsJson));

        // Create instance
        VkInstance instance;

        constexpr VkApplicationInfo applicationInfo = {
            VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "vulkan_sandbox", 1,
            "vulkan_sandbox", 1, VK_MAKE_VERSION(1, 4, 309)
        };

        const VkInstanceCreateInfo createInfo = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &applicationInfo, layerCount,
            layerNames->data(), extensionCount, extensionNames->data()
        };

        result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "failed to setup instance", _jsonSerializer->ErrorDetailToJson(_converter->ToString(result))));

        _instance = instance;
        _logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "instance setuped"));
    }

    void Vulkan::LogPhysicalDevices() const
    {
        // Get physical devices
        uint32_t physicalDeviceCount;
        auto result = vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);
        if (result != VK_SUCCESS) {
            _logger->Warn(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "failed to get physical device count"));
        }

        SharedVector<VkPhysicalDevice> physicalDevices = MAKE_SHARED_VECTOR(VkPhysicalDevice)(physicalDeviceCount);
        result = vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevices->data());
        if (result != VK_SUCCESS) {
            _logger->Warn(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "failed to get physical devices"));
        }

        // Serialization physical devices to JSON
        static void (*getProperties)(const VkPhysicalDevice, VkPhysicalDeviceProperties*) =
            [](const VkPhysicalDevice device, VkPhysicalDeviceProperties* properties) {
                vkGetPhysicalDeviceProperties(device, properties);
            };
        static void (*getFeatures)(const VkPhysicalDevice, VkPhysicalDeviceFeatures*) =
            [](const VkPhysicalDevice device, VkPhysicalDeviceFeatures* features) {
                vkGetPhysicalDeviceFeatures(device, features);
            };
        const auto devicesJson = _jsonSerializer->ToJson(physicalDevices, getProperties);

        // Logging
        _logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "physical devices", *devicesJson));
    }
}