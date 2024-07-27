#include "Object.hpp"

namespace Scoop::Memory
{
    unsigned int Object::GetReferenceCount() const
    { return this->referenceCount; }
    
    void Object::Retain()
    { this->referenceCount++; }

    void Object::Release()
    {
        if (--this->referenceCount == 0)
            delete this;
    }
}