#pragma once

#include <stdexcept>

namespace Scoop::Memory::Error
{
    template <typename ... Args>
    static std::runtime_error Create(const char *className, const char *methodName, const char *format, Args ... args)
    {
        String *str = new String();
        str->AssignFormat("[%s::%s] ", className, methodName);
        str->AppendFormat(format, args ...);

        std::runtime_error error(str->CString());
        str->Release();

        return error;
    }

    std::runtime_error NullError(const char *className, const char *methodName, const char *variableName);
    std::runtime_error EmptyError(const char *className, const char *methodName, const char *variableName);
    std::runtime_error IndexError(const char *className, const char *methodName, size_t index, size_t size);
}