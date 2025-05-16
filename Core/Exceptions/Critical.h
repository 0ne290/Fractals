#pragma once

#include <exception>
#include "../Typedefs.h"
#include <memory>

namespace Fractals::Core::Exceptions
{
    class Critical;
    using SharedCritical = std::shared_ptr<Critical>;
    #define MAKE_SHARED_CRITICAL std::make_shared<Critical>

    class Critical final : public std::exception
    {
    public:
        // Constructors
        Critical() = delete;

        explicit Critical(const SharedString&);

        // Copy constructors
        Critical(const Critical&) = delete;

        Critical(Critical&&) = delete;

        // Operators
        Critical& operator=(const Critical&) = delete;

        Critical& operator=(Critical&&) = delete;

        // Destructors
        ~Critical() override = default;

        // Methods
        static SharedCritical Create(const SharedString&);

        const char* what() const noexcept override;

    private:
        // Fields
        const SharedString _message;
    };
}