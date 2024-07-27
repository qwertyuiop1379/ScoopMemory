#pragma once

#include <map>

namespace Scoop::Memory
{
    class Dictionary : public Object
    {
        private:
            std::map<String *, Object *> map;

        public:
            Dictionary() = default;
            ~Dictionary();

            // Copy

            void Copy(const Dictionary *other);

            // Clear dictionary
            
            void Clear();

            // Get all keys

            void GetAllKeys(Array *keys) const;

            // Check if dictionary contains a key

            bool Contains(const char *key) const;
            bool Contains(const String *key) const;

            // Assign entry

            void SetObject(const char *key, Object *value);
            void SetObject(String *key, Object *value);

            // Retrieve entry

            Object *GetObject(const char *key) const;
            Object *GetObject(const String *key) const;

            Object *GetObjectIfPresent(const char *key) const;
            Object *GetObjectIfPresent(const String *key) const;

            template <class T> T *GetObject(const char *key) const
            {
                Object *object = this->GetObject(key);
                return static_cast<T *>(object);
            }

            template <class T> T *GetObject(const String *key) const
            {
                Object *object = this->GetObject(key);
                return static_cast<T *>(object);
            }

            template <class T> T *GetObjectIfPresent(const char *key) const
            {
                Object *object = this->GetObjectIfPresent(key);
                return static_cast<T *>(object);
            }

            template <class T> T *GetObjectIfPresent(const String *key) const
            {
                Object *object = this->GetObjectIfPresent(key);
                return static_cast<T *>(object);
            }

            // Remove entry

            void Remove(const char *key);
            void Remove(String *key);
    };
}