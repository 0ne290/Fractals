#pragma once
#include "../Typedefs.h"
#include <memory>
#include <format>

namespace Fractals::Core::Interfaces::InnerUtils
{
    SharedString InvertSlash(const char*);
}

#define CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD(tag, desc) MAKE_SHARED_STRING(std::format(\
    R"({{"header":{{"tag":"{}","desc":"{}","source":"{}"}},"payload":null}})", \
    tag, \
    desc, \
    std::format("{} -> {} : {}", __FUNCTION__, *Fractals::Core::Interfaces::InnerUtils::InvertSlash(__FILE__), \
    __LINE__)))
#define CREATE_LOG_MESSAGE_WITH_PAYLOAD(tag, desc, payload) MAKE_SHARED_STRING(std::format(\
    R"({{"header":{{"tag":"{}","desc":"{}","source":"{}"}},"payload":{}}})", \
    tag, \
    desc, \
    std::format("{} -> {} : {}", __FUNCTION__, *Fractals::Core::Interfaces::InnerUtils::InvertSlash(__FILE__), \
    __LINE__), payload))

namespace Fractals::Core::Interfaces
{
    class ILogger;
    using SharedILogger = std::shared_ptr<ILogger>;
    #define MAKE_SHARED_ILOGGER std::make_shared<ILogger>

	class ILogger
	{
	public:
        virtual void Trace(const SharedString) const = 0;

		virtual void Info(const SharedString) const = 0;

        virtual void Warn(const SharedString) const = 0;

        virtual void Critical(const SharedString) const = 0;
	};
}