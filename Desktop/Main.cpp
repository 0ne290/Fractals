#include "../Core/Exceptions/Critical.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Infrastructure/Logger.h"
#include "../Infrastructure/Vulkan.h"
#include <spdlog/common.h>
#include <memory>

int main()
{
    const shared_ptr<Fractals::Core::Interfaces::ILogger> logger =
        Fractals::Infrastructure::Logger::Create(spdlog::level::trace);

    try
    {
        const auto vulkan = Fractals::Infrastructure::Vulkan::Create(logger);

        vulkan->LogPhysicalDevices();

        return 0;
    }
    catch (const Fractals::Core::Exceptions::Critical& e)
    {
        logger->Critical(e.what());

        return 1;
    }
}