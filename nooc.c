#include <nooc.h>

DEF_VTABLE(Object)

// Every class should implement its own destructor and init

SIG_METHOD(void, Object, init)
{
    static int _done = 0;
    if (!_done)
    {
        _Object_init_vtable();
        _done = 1;
    }
    self->vptr = &_Object_vtable;
}

SIG_METHOD(void, Object, destructor)
{
    free(self);
}
