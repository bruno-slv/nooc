# libnooc

> I did it all for the nooc-ie

New Object Oriented C is a library designed to implement object oriented programming in C, in some "C minus minus" fashion.

## Usage

To create a new classs:

### ExampleClass.h

```C
#ifndef _EXAMPLE_CLASS_H_
#define _EXAMPLE_CLASS_H_

#include <nooc.h>

#define ExampleClass_ATTRIBUTES(X) \
    X(int, example_attribute)

#define ExampleClass_METHODS(X)          \
    MEMORY_METHODS(ExampleClass, X)      \
    X(int, ExampleClass, example_method)

CLASS(ExampleClass, Object)

#endif // _EXAMPLE_CLASS_H_
```

### ExampleClass.c

```C
#include <ExampleClass.h>

VTABLE(ExampleClass, Object)
DEF_CONSTRUCTOR(ExampleClass, Object)

SIG_METHOD(void, ExampleClass, init)
{
    Object_init(&self->_super);
    self->_super.vptr = (ObjectVTable *)&_Animal_vtable;
}

SIG_METHOD(void, ExampleClass, destructor)
{
    Object_destructor(&self->_super);
}

SIG_METHOD(void, ExampleClass, example_method)
{
    return self->example_attribute;
}
```

In this simple example, a class named ```ExampleClass``` is defined. This class inherits everything from the default global class ```Object```.

In the header, the attributes and methods for this class are declared through X-Macro syntax for easier readability. After the attributes and methods are declared, the class main structure is declared usign the macro ```CLASS(name, parent)```. All classes should use the ```MEMORY_METHODS(name, X)``` when declaring methods, for init and destructor declaration.

In the source file, the class VTable and constructor are implemented before any other methods. Then, the memory methods are implemented. After all the default methods and functions are implemented, then the programmer is free to add the class specific methods using the ```SIG_METHOD(type, name, method, ...)``` macro.

**Important**: The class methods and attributes X-Macros must follow the convention ```${ClassName}_${METHODS|ATTRIBUTES}``` to macro compatibility.

Then, in another file the class is used:

```C
#include <ExampleClass.h>

// Construct new object
ExampleClass *obj = CONSTRUCTOR(
    ExampleClass,
    .example_attribute = 1
);

// Access attribute
printf("%d\n", obj->example_attribute); // 1

// Run method
CALL(obj, ExampleClass, example_method)

// Destructor
CALL(obj, ExampleClass, destructor)
```

It's also possible to use objects allocated on stack with:

```C
// Construc Object
ExampleClass *obj = CONSTRUCTOR_STACK(
    ExampleClass,
    .example_attribute = 1
);

// Run method
CALL(&obj, ExampleClass, example_method)
```

While using stack objects, the destructor should not be called.

## Installation

CMake minimum 3.10 version is required.

```bash
git clone https://github.com/bruno-slv/nooc.git
cd nooc
mkdir build && cd build
cmake --build .
cmake --install . /usr/local
```

## Limitations

Currently, the lib doesn't support multi inheritance, encapsulation and is very manual when handling memory with dynamic allocated objects.
