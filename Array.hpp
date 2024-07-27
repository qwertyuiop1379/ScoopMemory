#pragma once

#include <vector>

namespace Scoop::Memory
{
    class Array : public Object
    {
        private:
            std::vector<Object *> objects;

        public:
            Array() = default;
            ~Array();

            // Copy other array

            void Copy(const Array *array);

            // Object count

            size_t Count() const;

            // Clear array

            void Clear();

            // Indexing

            Object *ObjectAtIndex(size_t index) const;
            template <class T> T *ObjectAtIndex(size_t index) const
            {
                Object *object = this->ObjectAtIndex(index);
                return static_cast<T *>(object);
            }

            // Check if array contains object

            bool Contains(Object *obj, size_t *index = nullptr) const;

            // Add objects

            void AddObject(Object *obj);
            void AddObjects(const Array *objects);

            // Remove objects

            void RemoveObjectAtIndex(size_t i);
            void RemoveObject(Object *obj);
            void RemoveObjects(const Array *objects);
    };
}