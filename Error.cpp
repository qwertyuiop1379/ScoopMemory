#include "Error.hpp"

namespace Scoop::Memory
{
    std::runtime_error Error::NullError(const char *className, const char *methodName, const char *variableName)
    { return Error::Create(className, methodName, "Variable '%s' may not be null.", variableName); }

    std::runtime_error Error::EmptyError(const char *className, const char *methodName, const char *variableName)
    { return Error::Create(className, methodName, "Variable '%s' may not be empty.", variableName); }

    std::runtime_error Error::IndexError(const char *className, const char *methodName, size_t index, size_t size)
    { return Error::Create(className, methodName, "Specified index (%zu) exceeds size (%zu).", index, size); }
}