
#ifdef __ORCAC__
#pragma optimize 79
#pragma noroot
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "SetFile-flags.h"

void FlagsHelp(void)
{
  fputs("SetFile [options] file...\n", stdout);
  fputs("-c creator   set the creator\n", stdout);
  fputs("-t type      set the file type\n", stdout);
  fputs("\n", stdout);
  exit(0);
}

struct Flags flags;
int FlagsParse(int argc, char **argv)
{
  char *cp;
  char c;
  int i;
  int j;
    
  memset(&flags, 0, sizeof(flags));

  for (i = 1; i < argc; ++i)
  {    
    cp = argv[i];
    c = cp[0];
      
    if (c != '-')
      return i;
      
    // -- = end of options.
    if (cp[1] == '-' && cp[2] == 0)
      return i + 1;
   
    // now scan all the flags in the string...
    for (j = 1; ; ++j)
    {
      int skip = 0;
          
      c = cp[j];
      if (c == 0) break;
          
      switch (c)
      {
      case 'h':
        FlagsHelp();
        break;
      case 'c':
        // -xarg or -x arg
        skip = 1;
        if (cp[j + 1])
        {
          flags._c = cp + j + 1;
        }
        else
        {
          if (++i >= argc)
          {
            fputs("option requires an argument -- c\n", stderr); 
            return -1;
          }
          flags._c = argv[i];
        }
        break;
      case 't':
        // -xarg or -x arg
        skip = 1;
        if (cp[j + 1])
        {
          flags._t = cp + j + 1;
        }
        else
        {
          if (++i >= argc)
          {
            fputs("option requires an argument -- t\n", stderr); 
            return -1;
          }
          flags._t = argv[i];
        }
        break;

      default:
        fprintf(stderr, "illegal option -- %c\n", c);
        return -1; 
      }
            
      if (skip) break;
    }    
  }

  return i;
}
