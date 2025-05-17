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

    SharedString JsonSerializer::ToJson(const SharedVector<const char*> strs)
    {
        const int strsLastIndex = strs->size() - 1;
        
        SharedString ret = MAKE_SHARED_STRING("[");

        if (strsLastIndex >= 0)
		{
			for (auto i = 0; i < strsLastIndex; i++)
				ret->append(std::format(R"("{}",)", (*strs)[i]));

			ret->append(std::format(R"("{}")", (*strs)[strsLastIndex]));
		}

        ret->append("]");

        return ret;
    }

    SharedString JsonSerializer::ToJson(const SharedString str)
    {
        return MAKE_SHARED_STRING(std::format(R"("{}")", *str));
    }
}