#include "Critical.h"
#include <string>
#include <type_traits>

namespace Fractals::Core::Exceptions
{
    Critical::Critical(std::string message) : _message(std::move(message)) {}

    const char* Critical::what() const noexcept
    {
        return _message.c_str();
    }
}