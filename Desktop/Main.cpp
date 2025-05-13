#include "../Core/Exceptions/Critical.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Infrastructure/Logger.h"
#include "../Infrastructure/Vulkan.h"
#include <spdlog/common.h>
#include <memory>
#include "../Core/Typedefs.h"

int main()
{
    const Fractals::Core::Interfaces::SharedILogger logger =
        Fractals::Infrastructure::Logger::Create(spdlog::level::trace);

    try
    {
        const auto vulkan = Fractals::Infrastructure::Vulkan::Create(logger);
        vulkan->LogPhysicalDevices();

        return 0;
    }
    catch (const Fractals::Core::Exceptions::SharedCritical e)
    {
        logger->Critical(MAKE_SHARED_STRING(e->what()));

        return 1;
    }
}