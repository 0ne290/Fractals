#pragma once
#include "../Core/Interfaces/ILogger.h"
#include "../Core/Typedefs.h"
#include <spdlog/logger.h>
#include <spdlog/common.h>
#include <memory>

using std::shared_ptr;

namespace Fractals::Infrastructure
{
    using SharedSpdLogger = std::shared_ptr<spdlog::logger>;

    class Logger;
    using SharedLogger = std::shared_ptr<Logger>;
    #define MAKE_SHARED_LOGGER std::make_shared<Logger>

    class Logger : public Fractals::Core::Interfaces::ILogger
    {
    public:
        // Constructors
        Logger() = delete;

        Logger(const SharedSpdLogger&);

        // Copy constructors
        Logger(const Logger&) = delete;

        Logger(Logger&&) = delete;

        // Operators
        Logger& operator=(const Logger&) = delete;

        Logger& operator=(Logger&&) = delete;

        // Destructors
        ~Logger();

        // Methods
        static SharedLogger Create(const spdlog::level::level_enum);

        void Trace(const SharedString) const override;

        void Debug(const SharedString) const override;

        void Info(const SharedString) const override;

        void Warn(const SharedString) const override;

        void Error(const SharedString) const override;

        void Critical(const SharedString) const override;

    private:
        // Fields
        const SharedSpdLogger _logger;
    };
}