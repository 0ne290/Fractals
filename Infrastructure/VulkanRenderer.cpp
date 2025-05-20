#include "VulkanRenderer.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Core/Exceptions/Critical.h"
#include "JsonSerializer.h"
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include <cstdint>
#include "GlfwWindow.h"
#include <unordered_map>

namespace Fractals::Infrastructure
{
    VulkanRenderer::VulkanRenderer(const Fractals::Core::Interfaces::SharedILogger& logger, const Fractals::Infrastructure::SharedJsonSerializer& jsonSerializer, const Fractals::Infrastructure::SharedConverter& converter)
        : _instance(nullptr), _surface(nullptr), _physicalDevice(nullptr), _logicalDevice(nullptr), _graphicQueue(nullptr), _presentationQueue(nullptr), _logger(logger), _jsonSerializer(jsonSerializer), _converter(converter) {
    }

    SharedVulkan VulkanRenderer::Create(const Fractals::Core::Interfaces::SharedILogger& logger,
        const Fractals::Infrastructure::SharedJsonSerializer& jsonSerializer,
        const Fractals::Infrastructure::SharedConverter& converter,
        const SharedGlfwWindow window)
	{
        const auto ret = MAKE_SHARED_VULKAN_RENDERER(logger, jsonSerializer, converter);
		ret->setupInstance(window->GetRequiredExtensionsToAttachVulkanRenderer());
        ret->_surface = window->CreateVulkanSurface(ret->_instance);
        ret->setupPhysicalDevice();
        ret->setupLogicalDeviceAndQueues();
		logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "renderer created"));

		return ret;
	}

	VulkanRenderer::~VulkanRenderer()
	{
        if (_logicalDevice != nullptr)
        {
            vkDestroyDevice(_logicalDevice, nullptr);
            _logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "logical device destroyed"));
        }

		if (_surface != nullptr)
		{
			vkDestroySurfaceKHR(_instance, _surface, nullptr);
			_logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "surface destroyed"));
		}

		if (_instance != nullptr)
		{
			vkDestroyInstance(_instance, nullptr);
            _logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "instance destroyed"));
		}

        _logger->Info(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "renderer destroyed"));
	}

    void VulkanRenderer::setupInstance(const SharedVector<const char*> extensions)
    {
        // Layers
#ifdef _DEBUG
        const SharedVector<const char*> layers = MAKE_SHARED_VECTOR(const char*)(std::initializer_list<const char*>{ "VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_api_dump" });
#else
        const SharedVector<const char*> layers = MAKE_SHARED_VECTOR(const char*)();
#endif // DEBUG
		const auto layersJson = _jsonSerializer->ToJson(layers);
		_logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "layers", *layersJson));

        // Extensions
        const auto extensionsJson = _jsonSerializer->ToJson(extensions);
		_logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "extensions", *extensionsJson));

        // Create instance
        VkInstance instance;

		constexpr VkApplicationInfo applicationInfo = {
			VK_STRUCTURE_TYPE_APPLICATION_INFO, nullptr, "Fractals", VK_MAKE_VERSION(1, 0, 0),
            "Fractals", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 4, 309)
        };

        const VkInstanceCreateInfo createInfo = {
			VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, &applicationInfo, layers->size(),
            layers->data(), extensions->size(), extensions->data()
        };

        const auto result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "create instance error", *_jsonSerializer->WrapInQuotes(_converter->ToString(result))));

        _instance = instance;
        _logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "instance setuped"));
    }

    void VulkanRenderer::setupPhysicalDevice()
    {
		uint32_t physicalDeviceCount;
		auto result = vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, nullptr);
        if (result != VK_SUCCESS)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "get physical device count error", *_jsonSerializer->WrapInQuotes(_converter->ToString(result))));
        if (physicalDeviceCount < 1)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "physical devices not found"));

        SharedVector<VkPhysicalDevice> physicalDevices = MAKE_SHARED_VECTOR(VkPhysicalDevice)(physicalDeviceCount);
		result = vkEnumeratePhysicalDevices(_instance, &physicalDeviceCount, physicalDevices->data());
        if (result != VK_SUCCESS)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "get physical devices error", *_jsonSerializer->WrapInQuotes(_converter->ToString(result))));

        const Shared<VkPhysicalDeviceProperties> properties = MAKE_SHARED(VkPhysicalDeviceProperties)();
        for (const auto& physicalDevice : *physicalDevices)
        {
            vkGetPhysicalDeviceProperties(physicalDevice, properties.get());
            if (properties->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                continue;

			
			const auto propertiesJson = _jsonSerializer->ToJson(properties);
			_logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "physical device properties", *propertiesJson));

			_physicalDevice = physicalDevice;
			_logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "physical device setuped"));

			return;
        }

        throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "discrete gpu not found"));
    }

	void VulkanRenderer::setupLogicalDeviceAndQueues()
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, nullptr);

        if (queueFamilyCount < 1)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "queue families not found"));
        
		SharedVector<VkQueueFamilyProperties> queueFamilies = MAKE_SHARED_VECTOR(VkQueueFamilyProperties)(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(_physicalDevice, &queueFamilyCount, queueFamilies->data());
        

        uint32_t graphicQueueFamilyIndex;
        bool graphicQueueFamilyIndexIsFound(false);

        uint32_t presentationQueueFamilyIndex;
        bool presentationQueueFamilyIndexIsFound(false);
		
		std::unordered_map<uint32_t, uint32_t> queueCountsByFamilyIndexes;

        for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < queueFamilyCount; queueFamilyIndex++)
        {
            const auto& queueFamily = (*queueFamilies)[queueFamilyIndex];

            if (!graphicQueueFamilyIndexIsFound && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                const auto queueFamilyPropertiesJson = _jsonSerializer->ToJson(queueFamily);
				_logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "graphic queue family properties", *queueFamilyPropertiesJson));
            
				graphicQueueFamilyIndex = queueFamilyIndex;
				graphicQueueFamilyIndexIsFound = true;
				
				queueCountsByFamilyIndexes.insert({ queueFamilyIndex, queueFamily.queueCount });

				if (presentationQueueFamilyIndexIsFound)
					break;
			}

			VkBool32 presentationSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(_physicalDevice, queueFamilyIndex, _surface, &presentationSupport);
			if (!presentationQueueFamilyIndexIsFound && presentationSupport)
			{
				const auto queueFamilyPropertiesJson = _jsonSerializer->ToJson(queueFamily);
				_logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "presentation queue family properties", *queueFamilyPropertiesJson));

				presentationQueueFamilyIndex = queueFamilyIndex;
				presentationQueueFamilyIndexIsFound = true;

				queueCountsByFamilyIndexes.insert({ queueFamilyIndex, queueFamily.queueCount });

				if (graphicQueueFamilyIndexIsFound)
					break;
			}
        }
        if (!graphicQueueFamilyIndexIsFound)
            throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "graphic queue family not found"));
		if (!presentationQueueFamilyIndexIsFound)
			throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "presentation queue family not found"));


		VkDevice logicalDevice;

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		const auto queuePriority = 1.0f;
		for (const auto& [queueFamilyIndex, queueCount] : queueCountsByFamilyIndexes) {
			const VkDeviceQueueCreateInfo queueCreateInfo =
			{
				VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				nullptr,
				0,
				queueFamilyIndex,
				queueCount,
				&queuePriority
			};
			queueCreateInfos.push_back(queueCreateInfo);
		}

		const Shared<VkPhysicalDeviceFeatures> features = MAKE_SHARED(VkPhysicalDeviceFeatures)();
		vkGetPhysicalDeviceFeatures(_physicalDevice, features.get());
		const auto featuresJson = _jsonSerializer->ToJson(features);
		_logger->Trace(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "physical device features", *featuresJson));

		const VkDeviceCreateInfo logicalDeviceCreateInfo =
		{
			VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			nullptr,
			0,
			static_cast<uint32_t>(queueCreateInfos.size()),
			queueCreateInfos.data(),
			0,
			nullptr,
			0,
			nullptr,
			features.get()
		};

		const auto result = vkCreateDevice(_physicalDevice, &logicalDeviceCreateInfo, nullptr, &logicalDevice);
		if (result != VK_SUCCESS)
			throw Fractals::Core::Exceptions::Critical::Create(CREATE_LOG_MESSAGE_WITH_PAYLOAD("vulkan", "create logical device error", *_jsonSerializer->WrapInQuotes(_converter->ToString(result))));


		VkQueue graphicQueue;
		vkGetDeviceQueue(logicalDevice, graphicQueueFamilyIndex, 0, &graphicQueue);

		VkQueue presentationQueue;
		vkGetDeviceQueue(logicalDevice, presentationQueueFamilyIndex, 0, &presentationQueue);


		_logicalDevice = logicalDevice;
		_graphicQueue = graphicQueue;
		_presentationQueue = presentationQueue;
		_logger->Debug(CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("vulkan", "logical device and queues setuped"));
	}
}