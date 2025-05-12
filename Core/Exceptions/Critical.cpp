#include "Critical.h"
#include <string>

namespace Fractals::Core::Exceptions
{
    Critical::Critical(const string& message) : _message(message) {}

    const char* Critical::what() const noexcept
    {
        return _message.c_str();
    }
}