#pragma once
#include <memory>
#include <cstdint>
#include <GLFW/glfw3.h>
#include "../Core/Interfaces/ILogger.h"

// GLFW for Vulkan
namespace Fractals::Infrastructure
{
    class Window;
    using SharedWindow = std::shared_ptr<Window>;
    #define MAKE_SHARED_WINDOW std::make_shared<Window>

    class Window
    {
    public:
        // Constructors
        Window() = delete;

        Window(const Fractals::Core::Interfaces::SharedILogger&, GLFWwindow*&);

        // Copy constructors
        Window(const Window&) = delete;

        Window(Window&&) = delete;

        // Operators
        Window& operator=(const Window&) = delete;

        Window& operator=(Window&&) = delete;

        // Destructors
        ~Window();

        // Methods
        static SharedWindow Create(const Fractals::Core::Interfaces::SharedILogger&, const uint32_t&, const uint32_t&);

        void Loop();

    private:
        // Fields
        GLFWwindow* _window;

        const Fractals::Core::Interfaces::SharedILogger _logger;
    };
}