#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Fractals::Infrastructure
{
    class JsonSerializer
    {
    public:
        // Constructors
        JsonSerializer() = delete;

        // Copy constructors
        JsonSerializer(const JsonSerializer&) = delete;

        JsonSerializer(JsonSerializer&&) = delete;

        // Operators
        JsonSerializer& operator=(const JsonSerializer&) = delete;

        JsonSerializer& operator=(JsonSerializer&&) = delete;

        // Destructors
        ~JsonSerializer() = delete;

        // Methods
        static std::string ToJson(const std::vector<VkPhysicalDevice>&,
            void (*&)(const VkPhysicalDevice&, VkPhysicalDeviceProperties&));
    };
}