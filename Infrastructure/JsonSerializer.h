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

        SharedString ToJson(const SharedVector<const char*>);

        SharedString WrapInQuotes(const SharedString);

        SharedString ToJson(const Shared<VkPhysicalDeviceProperties>);

        SharedString ToJson(const Shared<VkPhysicalDeviceFeatures>);

        SharedString ToJson(const VkQueueFamilyProperties&);

        SharedString ToJson(const VkExtent3D&);

    private:
        // Fields
        const Fractals::Infrastructure::SharedConverter _converter;
    };
}