#include "Critical.h"
#include "../Typedefs.h"

namespace Fractals::Core::Exceptions
{
    Critical::Critical(const SharedString& message) : _message(message) {}

    SharedCritical Critical::Create(const SharedString& message)
    {
        return MAKE_SHARED_CRITICAL(message);
    }

    const char* Critical::what() const noexcept
    {
        return _message->c_str();
    }
}