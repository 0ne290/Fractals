#include "Vulkan.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Core/Exceptions/Critical.h"
#include "JsonSerializer.h"
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include <cstdint>

namespace Fractals::Infrastructure
{
	Vulkan::~Vulkan()
	{
        vkDestroyInstance(_instance, nullptr);
		_logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "vulkan destroyed"));
	}

	SharedVulkan Vulkan::Create(const Fractals::Core::Interfaces::SharedILogger& logger)
	{
		const auto instance = createInstance();

		const auto ret = MAKE_SHARED_VULKAN(instance, logger);
		logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "vulkan created"));

		return ret;
	}

    VkInstance Vulkan::createInstance() {
        VkInstance ret;

        constexpr VkApplicationInfo applicationInfo = {
            VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "vulkan_sandbox", 1,
            "vulkan_sandbox", 1, VK_MAKE_API_VERSION(1, 4, 309, 0)
        };

        const VkInstanceCreateInfo createInfo = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &applicationInfo, 0,
            nullptr, 0, nullptr
        };

        const auto result = vkCreateInstance(&createInfo, nullptr, &ret);
        if (result != VK_SUCCESS)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "failed to create instance"));

        return ret;
    }

    Vulkan::Vulkan(const VkInstance& instance, const Fractals::Core::Interfaces::SharedILogger& logger)
        : _instance(instance), _logger(logger) { }

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
        const auto devicesJson = JsonSerializer::ToJson(physicalDevices, getProperties);

        // Logging
        _logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "physical devices", *devicesJson));
    }
}