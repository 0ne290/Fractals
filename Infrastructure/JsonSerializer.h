#pragma once
#include "../Core/Typedefs.h"
#include <vulkan/vulkan_core.h>
#include "Converter.h"
#include <memory>

// Required GLFW
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

        SharedString ToJson(const SharedString);

    private:
        // Fields
        const Fractals::Infrastructure::SharedConverter _converter;
    };
}