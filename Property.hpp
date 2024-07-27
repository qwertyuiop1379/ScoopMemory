#pragma once

namespace Scoop::Memory
{
    template <typename T> class Property
    {
        private:
            T *obj = nullptr;

        public:
            Property() = default;
            ~Property()
            {
                if (obj != nullptr)
                    obj->Release();
            }

            // Access

            T *Get() const { return this->obj; }
            T *operator->() const { return this->obj; }
            operator T *() const { return this->obj; }

            // Assignment

            void Assign(T *newObj)
            {
                if (this->obj != nullptr)
                    this->obj->Release();

                if (newObj != nullptr)
                    newObj->Retain();

                this->obj = newObj;
            }

            T *operator=(T *newObj)
            {
                this->Assign(newObj);
                return newObj;
            }

            // Allocate new object

            void AllocateNew()
            {
                T *newObj = new T();
                this->Assign(newObj);
                newObj->Release();
            }
    };
}