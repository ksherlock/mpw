#ifndef __winclude_unistd_h__
#define __winclude_unistd_h__

#include <io.h>

#define ftruncate(a,b) chsize(a,b)

typedef long ssize_t;

#endif
