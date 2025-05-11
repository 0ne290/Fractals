#include "../Core/Exceptions/Critical.h"

int main()
{
    //const auto logger = logging::LoggerCreator::create(spdlog::level::trace);

    try
    {
        //const auto vulkanFacadeCreator = vulkan::VulkanFacadeCreator(logger);

        //const auto vulkanFacade = vulkanFacadeCreator.create();
        //vulkanFacade->logPhysicalDevices();

        return 0;
    }
    catch (const Fractals::Core::Exceptions::Critical&)
    {
        //logger->instance->critical(ex.what());

        return 1;
    }
}