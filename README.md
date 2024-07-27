# ScoopMemory

ScoopMemory is a C++ memory-management library that uses reference-counting in order to efficiently control object lifetimes. It is loosely based on Objective-C.

# Remarks

- `Object` instances and subclasses are only intended for heap-allocation. Stack allocation is not intended and may result in undefined behavior.
- Inheritance from `Object` must be `public` in order to properly manage memory.
- This library does not automatically manage reference-counts. The programmer must properly retain and release objects in order to avoid memory leaks.

# Provided Classes

ScoopMemory provides the following classes, documented below:
- Object
- Property
- String
- Array
- Dictionary

# Custom Classes

To implement custom classes that use reference-counting, simply inherit from `Object`:
```c++
class SomeComplexObject : public Object
{ ... };
```

# Errors
The namespace `Scoop::Memory::Error` provides various functions that return an `std::runtime_error`.

### Public Methods
```c++
std::runtime_error Create(const char *className, const char *methodName, const char *format, ...) // returns a runtime_error with the format "[class::method] format..."

std::runtime_error NullError(const char *className, const char *methodName, const char *variableName) // returns a runtime_error detailing an unexpectedly null variable
std::runtime_error EmptyError(const char *className, const char *methodName, const char *variableName) // returns a runtime_error detailing an unexpectedly empty variable
std::runtime_error IndexError(const char *className, const char *methodName, size_t index, size_t size) // returns a runtime_error detailing an out-of-range indexing operation
```

### Example Usage
```c++
if (someVariable == nullptr)
    throw Error::NullError("Class", "Method", "someVariable");
else if (somethingElseGoesWrong)
    throw Error::Create("Class", "Method", "Something went wrong %d times.", 3);
```

# Tests
Tests are performed within `Scoop::Memory::TestScoopMemory`; Running the function `TestAll` will provide a detailed report:

```
Beginning tests for Object...
All tests complete for Object. Passed 5/5 tests.
Beginning tests for String...
All tests complete for String. Passed 32/32 tests.
Beginning tests for Property...
All tests complete for Property. Passed 9/9 tests.
Beginning tests for Array...
All tests complete for Array. Passed 17/17 tests.
Beginning tests for Dictionary...
All tests complete for Dictionary. Passed 15/15 tests.
```
- All provided classes pass all test cases

# Object
### Remarks
- Utilizes reference-counting to remain in memory until no longer needed.
- Upon allocation, objects are initialzed with a reference count of 1.
- Objects will remain in memory until their reference count reaches 0.
- Objects must be heap-allocated and cannot be copied/moved/assigned.
- Reference count is read-only.

### Constructor
```c++
Object() // initializes reference count to 1
```

### Public Methods
```c++
void GetReferenceCount() // get reference count

void Retain() // increase reference count
void Release() // decrease reference count
```

### Example Usage
```c++
Object *obj = new Object(); // reference count == 1
...
obj->Release(); // reference count == 0, object is deleted
```

# Property\<class T>
### Remarks
- Does not inherit from `Object`.
- Intended for use as a data-member.
- Not intended for use as a pointer.
- Template parameter `T` for object type.
- Contains `T *` data-member as the "held object".
- Retains and releases reference as needed.

### Constructor
```c++
Property() // initializes held object to nullptr
```

### Destructor
```c++
~Property() // if held object is not nullptr, releases held object
```

### Public Methods
```c++
T *Get() const // returns held object
T *operator->() const // returns held object
operator T *() const // returns held object

void Assign(T *newObject) // if held object is not nullptr, held object is released; new held object is retained
T *operator=(T *newObject) // calls Assign(newObject) and returns newObject

void AllocateNew() // allocates a new T and assigns it to the held object
```
### Example Usage
```c++
class SomeClass : public Object
{
    private:
        Property<String> name;

    public:
        SomeClass(String *name)
        {
            this->name = name; // name is assigned and retained
        }

        ~SomeClass() = default; // name is released upon destruction of the property
};
```

# String
### Remarks
- Inherits from `Object`.
- Manages a heap-allocated, null-terminated `char *`.
- All methods that accept a `String` as a parameter have overloads to accept a `const char *`.
### Constructor
```c++
String() // initialize an empty string

String(const String *str, size_t startIndex, size_t length = 0) // initialize with contents of str, starting at startIndex, copying length characters -- if length == 0, the entire string is copied
```

### Public Methods
```c++
size_t Length() // get length of string, excluding the null-terminator

char &GetCharacter(size_t index) // retrieve reference to the character at a specified index, excluding the null-terminator

const char *CString() const // retrieve internal char *

bool Empty() const // returns true if length is 0, excluding the null-terminator
static bool IsNullOrEmpty(const String *str) // returns true if str is nullptr or str is empty, otherwise false

void Copy(const String *str) // copy contents of str

void Assign(const String *str, size_t startIndex = 0) // assign to contents of str, starting at startIndex
void Assign(const String *source, size_t startIndex, size_t length) // assign to contents of str, starting at startIndex, copying length characters -- if length is 0, the entire string is copied
void AssignFormat(const char *format, ...) // assign to contents of formatted string
void AssignFromFile(const char *path) // assign to contents of file

bool IsEqual(const String *str) const // returns true if equal to str
int Compare(const char *str, size_t maxLength = 0) const // returns 0 if equal to str, otherwise returns the difference between the first non-matching character

bool StartsWith(const String *str) const // returns true if starts with str, otherwise false
bool EndsWith(const String *str) const // returns true if ends with str, otherwise false

void Append(char c) // append `c` to end
void Append(const String *str, size_t count = 0) // append str to end, copying count characters -- if count is 0, the entire string is copied

void Insert(const String *st, size_t index, size_t count = 0) // insert str at index, copying count characters -- if count is 0, the entire string is copied

void ConvertToUppercase() // convert to upper-case letters
void ConvertToLowercase() // convert to lower-case letters
```

### Example Usage
```c++
String *str = new String("hello, ");
str->Append("world.");

printf("%s\n", str->CString());

str->Release();
```

# Array
### Remarks
- Inherits from `Object`.
- Stores a list of values.
- Values must inherit from `Object`.
- Retains and releases stored objects.
- Does not track object type; this is up to the programmer.

### Constructor
```c++
Array() // initialize an empty array
```

### Destructor
```c++
~Array() // release all references
```

### Public Methods
```c++
size_t Count() const // retreive the number of stored objects

void Copy(const Array *array) // copy the contents of another array

void Clear() // clear array; release all references

Object *ObjectAtIndex(size_t index) const // retrieve the object stored at the specified index
template <class T> T *ObjectAtIndex(size_t index) const // retrieve the object stored at the specified index and cast it to the template parameter

bool Contains(Object *obj, size_t *index = nullptr) const // returns true if object is stored within array, otherwise returns false; if index pointer is not nullptr and object is contained within array, index is set to the index of the object

void AddObject(Object *obj) // add object to array; retain object
void AddObjects(const Array *objects) // add objects to array; retain objects

void RemoveObjectAtIndex(size_t i) // removes the object at the specified index; releases reference
void RemoveObject(Object *obj) // removes the object from the array; releases reference
void RemoveObjects(const Array *objects) // remove the objects from the array; releases references
```

### Example Usage
```c++
Array *array = new Array();

String *str = new String("hello");
array->AddObject(str);
str->Release(); // str remains in memory because it is retained by the array

...

size_t count = array->Count();
for (size_t i = 0; i < count; i++)
{
    String *str = array->ObjectAtIndex<String>(i);
    printf("%s\n", str->CString());
}

array->Release(); // array is deleted, and now str's reference count is 0, so it is also deleted
```

# Dictionary
### Remarks
- Inherits from `Object`.
- Holds a list of key-value pairs.
- Keys are `String` objects.
- Values must inherit from `Object`.
- Retains and releases keys and values.
- Does not track object type; this is up to the programmer.
- All methods that accept a `String` as a parameter have overloads to accept a `const char *`.

### Constructor
```c++
Dictionary() // initialize an empty dictionary
```

### Destructor
```c++
~Dictionary() // release all references
```

### Public Methods
```c++
void Copy(const Dictionary *dictionary) // copies contents of another dictionary

void Clear(); // clear dictionary; release all references

bool Contains(const String *key) const // returns true if key is present in dictionary, otherwise returns false
void GetAllKeys(Array *keys) const // assigns keys array to a list of all keys

void SetObject(String *key, Object *value) // assigns key-value pair in dictionary; retains key and object

Object *GetObject(const String *key) const // returns the object mapped to the specified key; if the key is not present in the dictionary, an error is thrown
template <class T> *GetObject(const String *key) const // calls GetObject and returns the pointer casted to the template parameter

Object *GetObjectIfPresent(const String *key) const // returns the object mapped to the specified key; if the key is not present in the dictionary, returns nullptr
template <class T> *GetObjectIfPresent(const String *key) const // calls GetObjectIfPresent and returns the pointer casted to the template parameter

void Remove(String *key) // removes the specified key-value pair from the dictionary; releases the reference to the key and object
```

### Example Usage
```c++
Dictionary *dict = new Dictionary();

...

Array *keys = new Array();
size_t count = keys->Count();

for (size_t i = 0; i < count; i++)
{
    String *key = keys->ObjectAtIndex<String>(i);
    String *value = dict->GetObject<String>(key);
    printf("Key: '%s', Value: '%s'\n", key->CString(), value->CString());
}

keys->Release();
dict->Release();
```