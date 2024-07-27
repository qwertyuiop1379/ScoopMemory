#include "Tests.hpp"

#include <cstdio>

#define BEGIN_TEST(section) size_t pass = 0, fail = 0; const char *_section = section; printf("Beginning tests for %s...\n", _section);
#define TEST(test, condition, failMessage) if (condition) { pass++; } else { printf("[%s] Fail: %s\n", test, failMessage); fail++; }
#define END_TEST { printf("All tests complete for %s. Passed %zu/%zu tests.\n", _section, pass, pass + fail); }

namespace Scoop::Memory::TestScoopMemory
{
    void TestObject()
    {
        class TestObject : public Object
        {
            public:
                bool &didDelete;
                TestObject(bool &didDelete) : didDelete(didDelete) { }
                ~TestObject() { this->didDelete = true; }
        };

        bool didDelete = false;
        TestObject *obj = new TestObject(didDelete);

        BEGIN_TEST("Object");

        TEST("Object::GetReferenceCount", obj->GetReferenceCount() == 1, "reference count must be 1 upon allocation.");

        obj->Retain();
        TEST("Object::Retain", obj->GetReferenceCount() == 2, "reference count did not increase.");

        obj->Release();
        TEST("Object::Release", obj->GetReferenceCount() == 1, "reference count did not decrease.");

        TEST("Object::~Object", didDelete == false, "object deleted prematurely.");
        obj->Release();
        TEST("Object::~Object", didDelete, "object did not delete upon final release.");

        END_TEST;
    }

    void TestString()
    {
        BEGIN_TEST("String");

        String *string = new String();
        TEST("String::CString", string->CString() != nullptr, "value is nullptr.");
        TEST("String::String", strlen(string->CString()) == 0, "string must be empty upon allocation.");
        TEST("String::Length", string->Length() == 0, "length must be 0 upon allocation.");
        TEST("String::Empty", string->Empty(), "string is empty.")
        
        TEST("String::IsEmptyOrNull", String::IsNullOrEmpty(nullptr), "string is null.");
        TEST("String::IsEmptyOrNull", String::IsNullOrEmpty(string), "string is empty.");

        string->Assign("hello");
        TEST("String::Assign", strcmp(string->CString(), "hello") == 0, "string was not assigned correct value.");
        TEST("String::Length", string->Length() == strlen("hello"), "reported length is incorrect.");
        TEST("String::GetCharacter", string->GetCharacter(0) == 'h' && string->GetCharacter(1) == 'e' && string->GetCharacter(2) == 'l', "incorrect character returned.");
        TEST("String::Empty", string->Empty() == false, "string is not empty.");

        String *otherString = new String();
        otherString->Copy(string);
        TEST("String::Copy", strcmp(string->CString(), otherString->CString()) == 0, "strings are not equivalent.");
        TEST("String::IsEqual", string->IsEqual(otherString), "strings are equivalent.");
        TEST("String::Compare", string->Compare(otherString) == 0, "strings are equivalent.");
        
        otherString->Assign("world");
        TEST("String::IsEqual", string->IsEqual(otherString) == false, "strings are not equivalent.");
        TEST("String::Compare", string->Compare(otherString) != 0, "strings are not equivalent.");

        otherString->Assign("help");
        TEST("String::Compare", string->Compare(otherString) == strcmp(string->CString(), otherString->CString()), "reported different value than strcmp.");

        otherString->Clear();
        TEST("String::Clear", otherString->Empty(), "string was not cleared.");

        otherString->Release();
        otherString = new String(string);
        TEST("String::String", string->IsEqual(otherString), "string was not copied.");

        otherString->Release();
        otherString = new String(string, 2);
        TEST("String::String", otherString->IsEqual("llo"), "substring was not properly extracted.");

        otherString->Release();
        otherString = new String(string, 2, 2);
        TEST("String::String", otherString->IsEqual("ll"), "substring was not properly extracted.");

        otherString->AssignFormat("hello %d, %s", 1, "world");
        TEST("String::AssignFormat", otherString->IsEqual("hello 1, world"), "string was not properly formatted.");
        TEST("String::StartsWith", otherString->StartsWith("hello"), "string starts with 'hello'.");
        TEST("String::StartsWith", !otherString->StartsWith("hi"), "string does not start with 'hi'.");
        TEST("String::EndsWith", otherString->EndsWith("world"), "string ends with 'world'.");
        TEST("String::EndsWith", !otherString->EndsWith("planet"), "string does not end with 'planet'.");

        otherString->Append('s');
        TEST("String::Append", otherString->EndsWith("worlds"), "did not append character.");
        TEST("String::Append", !otherString->IsEqual("worlds"), "string was overwritten instead of appended.");
        otherString->Append(string);
        TEST("String::Append", otherString->EndsWith(string), "did not append string.");
        otherString->Append("123456", 5);
        TEST("String::Append", otherString->EndsWith("12345"), "appended too many characters.");
        otherString->AppendFormat("a %d, b %s", 3, "hi");
        TEST("String::AppendFormat", otherString->EndsWith("a 3, b hi"), "did not append format.");

        string->ConvertToUppercase();
        TEST("String::ConvertToUppercase", string->IsEqual("HELLO"), "did not convert to uppercase.");
        string->ConvertToLowercase();
        TEST("String::ConvertToLowercase", string->IsEqual("hello"), "did not convert to lowercase.");

        string->Release();
        otherString->Release();

        END_TEST;
    }

    void TestProperty()
    {
        class TestClass
        {
            public:
                Property<Object> prop;
                TestClass() = default;
        };


        BEGIN_TEST("Property");

        TestClass testClass;
        Property<Object> &prop = testClass.prop;
        TEST("Property::Property", prop.Get() == nullptr, "object must be nullptr upon allocation.");

        Object *obj = new Object();
        prop.Assign(obj);
        TEST("Property::Assign", prop.Get() == obj, "object was not assigned.");
        TEST("Property::Assign", obj->GetReferenceCount() == 2, "object was not retained.");

        Object *otherObj = new Object();
        prop.Assign(otherObj);
        TEST("Property::Assign", obj->GetReferenceCount() == 1, "previous object was not released.");
        TEST("Property::Assign", otherObj->GetReferenceCount() == 2, "new object was not retained.");

        prop = obj;
        TEST("Property::operator=", prop.Get() == obj, "object was not assigned.");

        obj->Release();
        otherObj->Release();

        TestClass *newTestClass = new TestClass();
        Property<Object> &newProp = newTestClass->prop;
        newProp.AllocateNew();
        TEST("Property::AllocateNew", newProp.Get() != nullptr, "object was not allocated.");
        TEST("Property::AllocateNew", newProp->GetReferenceCount() == 1, "incorrect reference count; leaked memory.");

        Object *propObj = newProp.Get();
        propObj->Retain();

        delete newTestClass;
        TEST("Property::~Property", propObj->GetReferenceCount() == 1, "did not release object.");

        propObj->Release();

        END_TEST;
    }

    void TestArray()
    {
        BEGIN_TEST("Array");

        Array *array = new Array();

        TEST("Array::Array", array->Count() == 0, "object count must be 0 upon allocation.");

        Object *obj = new Object();
        array->AddObject(obj);
        TEST("Array::AddObject", obj->GetReferenceCount() == 2, "array did not retain object.");
        TEST("Array::AddObject", array->Count() == 1, "array count did not increase with added object.");

        array->RemoveObject(obj);
        TEST("Array::RemoveObject", obj->GetReferenceCount() == 1, "array did not release object.");
        TEST("Array::RemoveObject", array->Count() == 0, "object count did not decrease with removed object.");

        array->AddObject(obj);
        array->Clear();
        TEST("Array::Clear", array->Count() == 0, "array did not remove object.");
        TEST("Array::Clear", obj->GetReferenceCount() == 1, "array did not release object.");

        array->AddObject(obj);
        TEST("Array::ObjectAtIndex", obj == array->ObjectAtIndex(0), "array did not return the proper object.");

        Object *otherObj = new Object();
        array->AddObject(otherObj);
        TEST("Array::Count", array->Count() == 2, "array count did not increase with added object.");
        TEST("Array::ObjectAtIndex<T>", otherObj == array->ObjectAtIndex<Object>(1), "array did not return the proper object.");

        array->RemoveObject(otherObj);
        TEST("Array::Contains", array->Contains(obj), "array did not report containing object.");
        TEST("Array::Contains", !array->Contains(otherObj), "array reported containing object.");

        array->AddObject(otherObj);
        array->RemoveObjectAtIndex(1);
        TEST("Array::RemoveObjectAtIndex", !array->Contains(otherObj), "array did not remove the proper object.");
        
        array->AddObject(otherObj);
        array->RemoveObjectAtIndex(0);
        TEST("Array::RemoveObjectAtIndex", !array->Contains(obj), "array did not remove the proper object.");

        array->AddObject(obj);

        Array *otherArray = new Array();
        otherArray->Copy(array);

        TEST("Array::Copy", otherArray->Contains(obj) && otherArray->Contains(otherObj), "array did not copy contents.");

        array->Clear();

        Object *thirdObject = new Object();
        array->AddObject(thirdObject);
        array->AddObjects(otherArray);
        TEST("Array::AddObjects", array->Contains(obj) && array->Contains(otherObj) && array->Contains(thirdObject), "did not add specified objects.");

        array->RemoveObjects(otherArray);
        TEST("Array::RemoveObjects", !array->Contains(obj) && !array->Contains(otherArray) && array->Contains(thirdObject), "did not remove specified objects.");

        otherArray->Release();

        obj->Release();
        otherObj->Release();
        thirdObject->Release();

        END_TEST;
    }

    void TestDictionary()
    {
        BEGIN_TEST("Dictionary");

        Dictionary *dict = new Dictionary();
        String *str = new String();

        dict->SetObject("first", str);
        TEST("Dictionary::SetObject", str->GetReferenceCount() == 2, "did not retain object.");
        TEST("Dictionary::GetObject", dict->GetObject("first") == str, "did not return correct object.");
        TEST("Dictionary::GetObject<T>", dict->GetObject<String>("first") == str, "did not return correct object.");

        Array *keys = new Array();
        dict->GetAllKeys(keys);
        TEST("Dictionary::GetAllKeys", keys->Count() == 1, "incorrect key count.");
        TEST("Dictionary::GetAllKeys", keys->ObjectAtIndex<String>(0)->IsEqual("first"), "wrong key returned.");

        Object *obj = new Object();
        dict->SetObject("second", obj);
        dict->GetAllKeys(keys);
        TEST("Dictionary::GetAllKeys", keys->Count() == 2, "incorrect key count.");

        dict->Clear();
        dict->GetAllKeys(keys);
        TEST("Dictionary::Clear", keys->Count() == 0, "dictionary did not clear.");

        dict->SetObject("a", str);
        dict->SetObject("b", obj);

        Dictionary *otherDict = new Dictionary();
        otherDict->Copy(dict);
        otherDict->GetAllKeys(keys);
        TEST("Dictionary::Copy", keys->Count() == 2, "incorrect key count.");
        TEST("Dictionary::Copy", otherDict->GetObject("a") == str, "did not copy first object.");
        TEST("Dictionary::Copy", otherDict->GetObject("b") == obj, "did not copy second object.");
        keys->Release();
        otherDict->Release();

        TEST("Dictionary::Contains", dict->Contains("a"), "dict does contain the key.");
        TEST("Dictionary::Contains", dict->Contains("b"), "dict does contain the key.");
        TEST("Dictionary::Contains", !dict->Contains("c"), "dict does not contain the key.");

        dict->Remove("a");
        TEST("Dictionary::Remove", !dict->Contains("a"), "did not remove key-value pair.");
        TEST("Dictionary::Remove", str->GetReferenceCount() == 1, "did not release object.");

        dict->Release();
        str->Release();
        obj->Release();

        END_TEST;
    }

    void TestAll()
    {
        TestObject();
        TestString();
        TestProperty();
        TestArray();
        TestDictionary();
    }
}