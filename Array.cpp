#include "Array.hpp"

namespace Scoop::Memory
{
    Array::~Array()
    { this->Clear(); }

    // Copy other array

    void Array::Copy(const Array *array)
    {
        this->Clear();
        
        this->objects = array->objects;
        for (Object *object : this->objects)
            object->Retain();
    }

    // Object count

    size_t Array::Count() const
    { return this->objects.size(); }

    void Array::Clear()
    {
        for (Object *obj : this->objects)
            obj->Release();

        objects.clear();
    }

    // Indexing

    Object *Array::ObjectAtIndex(size_t index) const
    {
        if (index >= this->objects.size())
            throw Error::IndexError("Array", "ObjectAtIndex", index, this->objects.size());
            
        return this->objects[index];
    }

    // Check if array contains object
    
    bool Array::Contains(Object *obj, size_t *index) const
    {
        size_t size = this->objects.size();
        
        for (size_t i = 0; i < size; i++)
        {
            if (this->objects[i] == obj)
            {
                if (index != nullptr)
                    *index = i;

                return true;
            }
        }

        return false;
    }

    // Add objects

    void Array::AddObject(Object *obj)
    {
        if (this->Contains(obj))
            return;

        obj->Retain();
        this->objects.push_back(obj);
    }

    void Array::AddObjects(const Array *objects)
    {
        size_t size = objects->objects.size();
        for (size_t i = 0; i < size; i++)
            this->AddObject(objects->objects[i]);
    }

    // Remove objects

    void Array::RemoveObjectAtIndex(size_t i)
    {
        if (i >= this->objects.size())
            throw Error::IndexError("Array", "RemoveObjectAtIndex", i, this->objects.size());

        this->objects[i]->Release();
        this->objects.erase(this->objects.begin() + i);
    }

    void Array::RemoveObject(Object *obj)
    {
        size_t size = this->objects.size();
        for (size_t i = 0; i < size; i++)
        {
            Object *test = this->objects[i];
            if (test == obj)
            {
                test->Release();
                this->objects.erase(this->objects.begin() + i);
                break;
            }
        }
    }

    void Array::RemoveObjects(const Array *objects)
    {
        size_t size = objects->objects.size();
        for (size_t i = 0; i < size; i++)
            this->RemoveObject(objects->objects[i]);
    }
}