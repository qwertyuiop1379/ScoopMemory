#pragma once

namespace Scoop::Memory
{
    class Object
    {
        private:
            unsigned int referenceCount;

        public:
            Object() : referenceCount(1) { }
            virtual ~Object() = default;

            // Reference counting

            unsigned int GetReferenceCount() const;

            void Retain();
            void Release();

            // Prohibit copying

            Object(const Object &) = delete;
            void operator=(const Object &) = delete;

            // Prohibit moving

            Object(Object &&) = delete;
            void operator=(Object &&) = delete;
    };
}