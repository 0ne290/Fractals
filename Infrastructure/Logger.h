#pragma once
#include "../Core/Interfaces/ILogger.h"
#include <memory>
#include <string>
#include <spdlog/logger.h>
#include <spdlog/common.h>

using std::shared_ptr;

namespace Fractals::Infrastructure
{
    class Logger : public Fractals::Core::Interfaces::ILogger
    {
    public:
        // Constructors
        Logger() = delete;

        Logger(const std::shared_ptr<spdlog::logger>&);

        // Copy constructors
        Logger(const Logger&) = delete;

        Logger(Logger&&) = delete;

        // Operators
        Logger& operator=(const Logger&) = delete;

        Logger& operator=(Logger&&) = delete;

        // Destructors
        ~Logger();

        // Methods
        static shared_ptr<Logger> Create(const spdlog::level::level_enum&);

        void Trace(const string&) const override;

        void Info(const string&) const override;

        void Warn(const string&) const override;

        void Critical(const string&) const override;

    private:
        // Fields
        const shared_ptr<spdlog::logger> _logger;
    };
}