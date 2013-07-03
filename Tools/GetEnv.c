/*
 *
 * GetEnv variable
 * read an mpw variable.
 * eg:  CLibraries=`mpw GetEnv CLibraries`
 * (makefile) CLibraries = $(shell mpw GetEnv CLibraries)
 * flags to do name = value?
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  char *value;
  char *name;

  if (argc != 2)
  {
    return 1;
  }

  name = argv[1];
  value = getenv(name);
  // value == null if not defined.
  if (value) puts(value);
  return 0;
}
