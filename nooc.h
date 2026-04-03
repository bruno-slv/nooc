#ifndef _NOOC_H_
#define _NOOC_H_

#include <stdlib.h>

// Method Signature

#define SIG_METHOD(ret, name, method, ...) \
    ret name##_##method(name *self, ##__VA_ARGS__)

#define DECL_METHOD(ret, name, method, ...) \
    SIG_METHOD(ret, name, method, ##__VA_ARGS__);

// Apply Methods and Attributes

#define APPLY_METHODS_ON_CLASS(ret, name, method, ...) \
    ret (*method)(name * self, ##__VA_ARGS__);

#define APPLY_ATTRIBUTES_ON_CLASS(type, name) type name;

#define APPLY_OWN_ATTRIBUTES_ON_CONSTRUCTOR(type, name) _obj->name = p.name;
#define APPLY_PARENT_ATTRIBUTES_ON_CONSTRUCTOR(type, name) _obj->_super.name = p.name;

#define IMPL_METHOD(type, name, method, ...) \
    _##name##_vtable.method = name##_##method;

// Global Definitions

#define _GLOBAL_CLASS(name)                          \
    typedef struct name##VTable name##VTable;        \
    typedef struct name name;                        \
    struct name                                      \
    {                                                \
        name##_ATTRIBUTES(APPLY_ATTRIBUTES_ON_CLASS) \
    };                                               \
    struct name##VTable                              \
    {                                                \
        name##_METHODS(APPLY_METHODS_ON_CLASS)       \
    };                                               \
    name##_METHODS(DECL_METHOD) extern name##VTable _##name##_vtable;

#define _GLOBAL_VTABLE(name)            \
    name##VTable _##name##_vtable;      \
    static void _##name##_init_vtable() \
    {                                   \
        name##_METHODS(IMPL_METHOD)     \
    }

// Root Object

#define MEMORY_METHODS(type, X) \
    X(void, type, init)         \
    X(void, type, destructor)

#define Object_ATTRIBUTES(X) \
    X(ObjectVTable *, vptr)

#define Object_METHODS(X) \
    MEMORY_METHODS(Object, X)

_GLOBAL_CLASS(Object)

// Inherited Object Definitions

// Constructor

#define DEF_CONSTRUCTOR(name, parent)                                       \
    name *name##_new(name##ConstructorAttributes p)                         \
    {                                                                       \
        static int _vtable_inited = 0;                                      \
        if (!_vtable_inited)                                                \
        {                                                                   \
            _##name##_init_vtable();                                        \
            _vtable_inited = 1;                                             \
        }                                                                   \
                                                                            \
        name *_obj = (name *)calloc(1, sizeof(name));                       \
        if (!_obj)                                                          \
            return NULL;                                                    \
                                                                            \
        parent##_init(&_obj->_super);                                        \
        parent##_ATTRIBUTES(APPLY_PARENT_ATTRIBUTES_ON_CONSTRUCTOR)         \
            name##_init(_obj);                                              \
        name##_ATTRIBUTES(APPLY_OWN_ATTRIBUTES_ON_CONSTRUCTOR) return _obj; \
    }

#define CONSTRUCTOR(name, ...) \
    name##_new((name##ConstructorAttributes){__VA_ARGS__});

// Class

#define CLASS(name, parent)                                                 \
    typedef struct name##VTable name##VTable;                               \
    typedef struct name name;                                               \
    typedef struct name##ConstructorAttributes name##ConstructorAttributes; \
    struct name                                                             \
    {                                                                       \
        parent _super;                                                      \
        name##_ATTRIBUTES(APPLY_ATTRIBUTES_ON_CLASS)                        \
    };                                                                      \
    struct name##ConstructorAttributes                                      \
    {                                                                       \
        parent##_ATTRIBUTES(APPLY_ATTRIBUTES_ON_CLASS)                      \
            name##_ATTRIBUTES(APPLY_ATTRIBUTES_ON_CLASS)                    \
    };                                                                      \
    struct name##VTable                                                     \
    {                                                                       \
        parent##VTable _super;                                              \
        name##_METHODS(APPLY_METHODS_ON_CLASS)                              \
    };                                                                      \
    name##_METHODS(DECL_METHOD)                                             \
        name *                                                              \
        name##_new(name##ConstructorAttributes p);                          \
    extern name##VTable _##name##_vtable;

#define VTABLE(name, parent)                              \
    name##VTable _##name##_vtable;                        \
    static void _##name##_init_vtable()                   \
    {                                                     \
        name##_METHODS(IMPL_METHOD)                       \
            _##name##_vtable._super = _##parent##_vtable; \
    }

#define CALL(obj, type, method, ...) \
    (((type##VTable *)((Object *)(obj))->vptr)->method((type *)(obj), ##__VA_ARGS__))

#endif // _NOOC_H_
