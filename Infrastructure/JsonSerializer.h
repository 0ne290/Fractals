#pragma once
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include "Converter.h"
#include <memory>

namespace Fractals::Infrastructure
{
    class JsonSerializer;
    using SharedJsonSerializer = std::shared_ptr<JsonSerializer>;
    #define MAKE_SHARED_JSON_SERIALIZER std::make_shared<JsonSerializer>

    class JsonSerializer
    {
    public:
        // Constructors
        JsonSerializer() = delete;

        JsonSerializer(const Fractals::Infrastructure::SharedConverter&);

        // Copy constructors
        JsonSerializer(const JsonSerializer&) = delete;

        JsonSerializer(JsonSerializer&&) = delete;

        // Operators
        JsonSerializer& operator=(const JsonSerializer&) = delete;

        JsonSerializer& operator=(JsonSerializer&&) = delete;

        // Destructors
        ~JsonSerializer() = default;

        // Methods
        static SharedJsonSerializer Create(const Fractals::Infrastructure::SharedConverter&);

        SharedString ToJson(const SharedVector<VkPhysicalDevice>,
            void (*const getProperties)(const VkPhysicalDevice, VkPhysicalDeviceProperties*));

        // Methods
        SharedString ToJson(const SharedVector<VkLayerProperties>);

        SharedString ToJson(const SharedVector<VkExtensionProperties>);

    private:
        // Fields
        const Fractals::Infrastructure::SharedConverter _converter;
    };
}