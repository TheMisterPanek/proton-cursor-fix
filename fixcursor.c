#define _GNU_SOURCE
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <dlfcn.h>

#define MAXB 256
#define MAXD 16
static Cursor blank[MAXB];
static int nblank;
static Display *dpys[MAXD];
static Cursor arrows[MAXD];
static int ndpy;

Cursor XCreatePixmapCursor(Display *d, Pixmap src, Pixmap mask, XColor *fg, XColor *bg,
                           unsigned int x, unsigned int y)
{
    static Cursor (*real)(Display *, Pixmap, Pixmap, XColor *, XColor *, unsigned int, unsigned int);
    if (!real) real = dlsym(RTLD_NEXT, "XCreatePixmapCursor");
    Cursor c = real(d, src, mask, fg, bg, x, y);
    if (nblank < MAXB) blank[nblank++] = c;
    return c;
}

static int is_blank(Cursor c)
{
    for (int k = 0; k < nblank; k++) if (blank[k] == c) return 1;
    return 0;
}

static Cursor arrow_for(Display *d)
{
    for (int k = 0; k < ndpy; k++) if (dpys[k] == d) return arrows[k];
    if (ndpy >= MAXD) return None;
    dpys[ndpy] = d;
    arrows[ndpy] = XCreateFontCursor(d, XC_left_ptr);
    return arrows[ndpy++];
}

int XDefineCursor(Display *d, Window w, Cursor c)
{
    static int (*real)(Display *, Window, Cursor);
    if (!real) real = dlsym(RTLD_NEXT, "XDefineCursor");
    if (c != None && !is_blank(c)) {
        Cursor a = arrow_for(d);
        if (a != None) c = a;
    }
    return real(d, w, c);
}
