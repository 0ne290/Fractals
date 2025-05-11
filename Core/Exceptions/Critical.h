#pragma once

#include <exception>
#include <string>

namespace Fractals::Core::Exceptions
{
    class Critical final : public std::exception
    {
    public:

        // Constructors
        Critical() = delete;

        explicit Critical(std::string message);

        // Copy constructors
        Critical(const Critical&) = delete;

        Critical(Critical&&) = delete;

        // Operators
        Critical& operator=(const Critical&) = delete;

        Critical& operator=(Critical&&) = delete;

        // Destructors
        //~Critical() override = delete;

        // Methods
        [[nodiscard]] const char* what() const noexcept override;

    private:

        // Fields
        const std::string _message;
    };
}