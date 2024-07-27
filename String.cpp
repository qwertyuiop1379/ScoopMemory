#include "String.hpp"

#include <cstdlib>
#include <cstdio>

namespace Scoop::Memory
{
    // Constructor

    String::String()
    { this->Assign(""); }

    // Destructor

    String::~String()
    {
        if (this->data != nullptr)
            free(this->data);
    };

    // Resize

    void String::Resize(size_t length)
    {
        if (this->data == nullptr)
            this->data = (char *)malloc(length + 1);
        else
            this->data = (char *)realloc(this->data, length + 1);
    }

    // Copy

    void String::Copy(const String *string)
    { this->Assign(string); }

    // Length

    size_t String::Length() const
    { return strlen(this->data); }

    // Data access

    const char *String::CString() const
    { return this->data; }

    char &String::GetCharacter(size_t index)
    {
        size_t length = strlen(this->data);
        if (index >= length)
            throw Error::IndexError("String", "GetCharacter", index, length);

        return this->data[index];
    }

    // Value validation

    bool String::Empty() const
    { return strlen(this->data) == 0; }

    bool String::IsNullOrEmpty(const String *string)
    { return string == nullptr || string->Empty(); }

    // Substring

    String::String(const String *str, size_t startIndex, size_t length)
    { this->Assign(str->data, startIndex, length); }
    String::String(const char *str, size_t startIndex, size_t length)
    { this->Assign(str, startIndex, length); }

    // Assignment

    void String::Assign(const String *source, size_t startIndex, size_t length)
    { this->Assign(source->data, startIndex, length); }
    
    void String::Assign(const char *source, size_t startIndex, size_t length)
    {
        if (length == 0)
            length = strlen(source);

        size_t sourceLength = strlen(source + startIndex);
        if (sourceLength > length)
            sourceLength = length;

        this->Resize(sourceLength);

        memcpy(this->data, source + startIndex, sourceLength + 1);
        this->data[sourceLength] = '\0';
    }

    // Format assigment

    void String::AssignFormat(const char *format, ...)
    {
        int final_n, n = ((int)strlen(format)) * 2;
        std::unique_ptr<char[]> formatted;
        va_list ap;

        while (true)
        {
            formatted.reset(new char[n]);
            strcpy(&formatted[0], format);

            va_start(ap, format);
            final_n = vsnprintf(&formatted[0], n, format, ap);
            va_end(ap);

            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }

        this->Assign(formatted.get());
    }

    // Assign from file

    void String::AssignFromFile(const String *path)
    { this->AssignFromFile(path->data); }

    void String::AssignFromFile(const char *path)
    {
        FILE *file = fopen(path, "r");

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        this->Resize(size);

        fseek(file, 0, SEEK_SET);
        fread(this->data, 1, size, file);
        fclose(file);

        this->data[size] = '\0';
    }

    // String comparison

    bool String::IsEqual(const String *string) const
    {
        if (this == string)
            return true;
        return this->IsEqual(string->data);
    }

    bool String::IsEqual(const char *string) const
    {
        size_t length = strlen(this->data);
        if (length != strlen(string))
            return false;

        return memcmp(this->data, string, length) == 0;
    }

    int String::Compare(const String *string, size_t maxLength) const
    { return this->Compare(string->data, maxLength); }

    int String::Compare(const char *string, size_t maxLength) const
    {
        if (string == nullptr)
            throw Error::NullError("String", "Compare", "string");
        if (maxLength == 0)
            maxLength = strlen(this->data) + 1;
        return strncmp(this->data, string, maxLength);
    }

    // Affix testing

    bool String::StartsWith(const String *str) const
    { return this->Compare(str, strlen(str->data)) == 0; }

    bool String::StartsWith(const char *str) const
    { return this->Compare(str, strlen(str)) == 0; }

    bool String::EndsWith(const String *str) const
    { return this->EndsWith(str->data); }

    bool String::EndsWith(const char *str) const
    {
        if (str == nullptr)
            throw Error::NullError("String", "EndsWith", "string");

        size_t length = strlen(str);
        if (length == 0)
            throw Error::Create("String", "EndsWith", "Specified string is empty.");

        if (length >= strlen(this->data))
            return false;
            
        return strncmp(this->data + strlen(this->data) - length, str, length) == 0;
    }

    // Clear string

    void String::Clear()
    { this->Assign(""); }

    // Append to string

    void String::Append(char c)
    { this->Insert(&c, strlen(this->data), 1); }
    void String::Append(const String *str, size_t count)
    { this->Insert(str->data, strlen(this->data), count); }
    void String::Append(const char *str, size_t count)
    { this->Insert(str, strlen(this->data), count); }

    void String::AppendFormat(const char *format, ...)
    {
        int final_n, n = ((int)strlen(format)) * 2;
        std::unique_ptr<char[]> formatted;
        va_list ap;

        while (true)
        {
            formatted.reset(new char[n]);
            strcpy(&formatted[0], format);

            va_start(ap, format);
            final_n = vsnprintf(&formatted[0], n, format, ap);
            va_end(ap);

            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }

        this->Append(formatted.get());
    }

    // Insert into string
    
    void String::Insert(const String *str, size_t index, size_t count)
    { this->Insert(str->data, index, count); }

    void String::Insert(const char *str, size_t index, size_t count)
    {
        if (count == 0)
            count = strlen(str);
            
        size_t length = strlen(this->data);
        if (index > length)
            throw Error::IndexError("String", "Insert", index, length);

        length += count;

        char *buffer = (char *)malloc(length + count + 1);
        memcpy(buffer, this->data, index);
        memcpy(buffer + index, str, count);
        strcpy(buffer + index + count, this->data + index);
        
        this->Assign(buffer);
        free(buffer);
    }

    // Case conversion

    void String::ConvertToUppercase()
    {
        size_t length = this->Length();
        for (size_t i = 0; i < length; i++)
        {
            char &c = this->GetCharacter(i);
            if (c >= 'a' && c <= 'z')
                c -= 0x20;
        }
    }

    void String::ConvertToLowercase()
    {
        size_t length = this->Length();
        for (size_t i = 0; i < length; i++)
        {
            char &c = this->GetCharacter(i);
            if (c >= 'A' && c <= 'Z')
                c += 0x20;
        }
    }
}