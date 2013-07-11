
#ifndef __flags_h__
#define __flags_h__

typedef struct Flags {
    char *_c;
    char *_t;


} Flags;


extern struct Flags flags;

int FlagsParse(int argc, char **argv);

void FlagsHelp(void);

#endif

