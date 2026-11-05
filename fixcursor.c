#define _GNU_SOURCE
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <dlfcn.h>

#define FAKE_CURSOR ((Cursor)1)

Cursor XRenderCreateCursor(Display *d, Picture p, unsigned int x, unsigned int y)
{
    return FAKE_CURSOR;
}

int XDefineCursor(Display *d, Window w, Cursor c)
{
    static int (*real)(Display *, Window, Cursor);
    if (!real) real = dlsym(RTLD_NEXT, "XDefineCursor");
    if (c == FAKE_CURSOR) c = None;
    return real(d, w, c);
}

int XFreeCursor(Display *d, Cursor c)
{
    static int (*real)(Display *, Cursor);
    if (!real) real = dlsym(RTLD_NEXT, "XFreeCursor");
    if (c == FAKE_CURSOR) return 0;
    return real(d, c);
}
