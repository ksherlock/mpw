#ifndef __winclude_unistd_h__
#define __winclude_unistd_h__

#include <io.h>

#define ftruncate(a,b) chsize(a,b)


#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#endif
