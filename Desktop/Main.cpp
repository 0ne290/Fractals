#include "../Core/Exceptions/Critical.h"
#include "../Core/Interfaces/ILogger.h"
#include "../Infrastructure/Converter.h"
#include "../Infrastructure/JsonSerializer.h"
#include "../Infrastructure/Logger.h"
#include "../Infrastructure/Window.h"
#include "../Infrastructure/VulkanRenderer.h"

int main()
{
    const Fractals::Core::Interfaces::SharedILogger logger =
        Fractals::Infrastructure::Logger::Create(spdlog::level::trace);
    const auto converter = Fractals::Infrastructure::Converter::Create(logger);
	const auto jsonSerializer = Fractals::Infrastructure::JsonSerializer::Create(converter);

    try
    {
        const auto window = Fractals::Infrastructure::Window::Create(logger, 800, 600);
        const auto renderer = Fractals::Infrastructure::VulkanRenderer::Create(logger, jsonSerializer, converter, window->GetHwnd());

        window->Loop();

        return 0;
    }
    catch (const Fractals::Core::Exceptions::SharedCritical e)
    {
        logger->Critical(MAKE_SHARED_STRING(e->what()));

        return 1;
    }
	catch (const std::exception& e)
	{
		logger->Critical(CREATE_LOG_MESSAGE_WITH_PAYLOAD("main", "uncaught exception", *jsonSerializer->ToJson(MAKE_SHARED_STRING(e.what()))));

		return 2;
	}
}