#pragma once

using size_t = decltype(sizeof(1));

namespace Scoop::Memory
{
    class String : public Object
    {
        private:
            char *data = nullptr;
            void Resize(size_t length);

        public:
            String();
            ~String();

            // Length

            size_t Length() const;

            // Data access

            char &GetCharacter(size_t index);
            const char *CString() const;

            // Value validation

            bool Empty() const;
            static bool IsNullOrEmpty(const String *string);

            // Copy

            void Copy(const String *string);

            // Substring

            explicit String(const String *string, size_t startIndex = 0, size_t length = 0);
            explicit String(const char *string, size_t startIndex = 0, size_t length = 0);

            // Assignment

            void Assign(const String *source, size_t startIndex = 0, size_t length = 0);
            void Assign(const char *source, size_t startIndex = 0, size_t length = 0);

            // Format assignment

            void AssignFormat(const char *format, ...);

            // Assign from file

            void AssignFromFile(const String *path);
            void AssignFromFile(const char *path);

            // String comparison

            bool IsEqual(const String *string) const;
            bool IsEqual(const char *string) const;
            int Compare(const String *string, size_t maxLength = 0) const;
            int Compare(const char *string, size_t maxLength = 0) const;

            // Affix testing

            bool StartsWith(const String *string) const;
            bool StartsWith(const char *string) const;
            bool EndsWith(const String *string) const;
            bool EndsWith(const char *string) const;

            // Clear string

            void Clear();

            // Append to string

            void Append(char c);
            void Append(const String *string, size_t count = 0);
            void Append(const char *string, size_t count = 0);
            void AppendFormat(const char *format, ...);

            // Insert into string
            
            void Insert(const String *string, size_t index, size_t count = 0);
            void Insert(const char *string, size_t index, size_t count = 0);

            // Case conversion

            void ConvertToUppercase();
            void ConvertToLowercase();
    };
}