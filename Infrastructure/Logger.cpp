#include "Logger.h"
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include "../Core/Interfaces/ILogger.h"
#include "../Core/Typedefs.h"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <thread>

namespace Fractals::Infrastructure
{
	Logger::~Logger()
	{
		const auto spdlog_tp = spdlog::thread_pool();

		_logger->flush();
		while (spdlog_tp->queue_size() != 0)
			std::this_thread::yield();

		_logger->set_pattern(R"({"time":"%Y-%m-%d %H:%M:%S.%e","level":"%l","process":%P,"thread":%t,"message":%v})");
		_logger->info(*CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("logger", "logger destroyed"));
		_logger->flush();
		while (spdlog_tp->queue_size() != 0)
			std::this_thread::yield();

		_logger->set_pattern("]}");
		_logger->info("");

		spdlog::shutdown();
	}

	SharedLogger Logger::Create(const spdlog::level::level_enum logLevel)
	{
		const auto logger = spdlog::rotating_logger_mt<spdlog::async_factory>("logger", "log.json",
			1'073'741'824, 2);
		logger->set_level(logLevel);
		const auto spdlog_tp = spdlog::thread_pool();

		logger->set_pattern(R"({"log":[)");
		logger->info("");
		logger->flush();
		while (spdlog_tp->queue_size() != 0)
			std::this_thread::yield();

		logger->set_pattern(R"({"time":"%Y-%m-%d %H:%M:%S.%e","level":"%l","process":%P,"thread":%t,"message":%v},)");
		logger->info(*CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD("logger", "logger created"));
		logger->flush();
		while (spdlog_tp->queue_size() != 0)
			std::this_thread::yield();

		return MAKE_SHARED_LOGGER(logger);
	}

	Logger::Logger(const SharedSpdLogger& logger) : _logger(logger) {}

	void Logger::Trace(const SharedString message) const
	{
		_logger->trace(*message);
	}

	void Logger::Info(const SharedString message) const
	{
		_logger->info(*message);
	}

	void Logger::Warn(const SharedString message) const
	{
		_logger->warn(*message);
	}

	void Logger::Critical(const SharedString message) const
	{
		_logger->critical(*message);
	}
}