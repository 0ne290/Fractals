#include "../Core/Exceptions/Critical.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Infrastructure/Logger.h"
#include "../Infrastructure/Vulkan.h"
#include <spdlog/common.h>
#include <memory>
#include "../Core/Typedefs.h"
#include "../Infrastructure/Converter.h"
#include "../Infrastructure/JsonSerializer.h"

int main()
{
    const Fractals::Core::Interfaces::SharedILogger logger =
        Fractals::Infrastructure::Logger::Create(spdlog::level::trace);

    try
    {
        const auto converter = Fractals::Infrastructure::Converter::Create(logger);
        const auto jsonSerializer = Fractals::Infrastructure::JsonSerializer::Create(converter);

        const auto vulkan = Fractals::Infrastructure::Vulkan::Create(logger, jsonSerializer);
        vulkan->LogPhysicalDevices();

        return 0;
    }
    catch (const Fractals::Core::Exceptions::SharedCritical e)
    {
        logger->Critical(MAKE_SHARED_STRING(e->what()));

        return 1;
    }
}