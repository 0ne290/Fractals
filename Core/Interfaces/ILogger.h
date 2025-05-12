#pragma once
#include <string>
#include <format>

#define CREATE_LOG_MESSAGE_WITHOUT_PAYLOAD(tag, desc) std::format(\
    R"({{"header":{{"tag":"{}","desc":"{}","source":"{}"}},"payload":null}})", \
    tag, \
    desc, \
    std::format("{} -> {} : {}", __FUNCTION__, __FILE__, __LINE__))
#define CREATE_LOG_MESSAGE_WITH_PAYLOAD(tag, desc, payload) std::format(\
    R"({{"header":{{"tag":"{}","desc":"{}","source":"{}"}},"payload":{}}})", \
    tag, \
    desc, \
    std::format("{} -> {} : {}", __FUNCTION__, __FILE__, __LINE__), \
    payload)

using std::string;

namespace Fractals::Core::Interfaces
{
	class ILogger
	{
	public:
        virtual void Trace(const string&) const = 0;

		virtual void Info(const string&) const = 0;

        virtual void Warn(const string&) const = 0;

		virtual void Critical(const string&) const = 0;
	};
}