
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <Finder.h>
#include <Files.h>

#include "SetFile-flags.h"



int tox(char c)
{
	c |= 0x20;
	if (c >='0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return 0;
}

int hex(const char *in, char *out, int length)
{
	int i;
	for (i = 0; i < length; ++i)
	{
		int tmp = 0;
		char c;

		c = *in++;
		if (!isxdigit(c)) return -1;
		tmp |= tox(c) << 4;

		c = *in++;
		if (!isxdigit(c)) return -1;
		tmp |= tox(c);

		*out++ = tmp;
	}	
	return 0;
}

// convert the file/creators...
// format:
// 0x \xdigit{8}
// $ \xdigit{8}
// 4-cc code
int checkcode(const char *in, char *out)
{
	int length;

	length = strlen(in);

	if (length == 4)
	{
		// 4 cc code.
		int i;
		for (i = 0; i < 4; ++i)
			out[i] = in[i];
		return 0;
	}

	if (length == 9 && in[0] == '$')
		return hex(in + 1, out, 4);

	if (length ==10 && in[0] == '0' && in[1] == 'x')
		return hex(in + 2, out, 4);

	return -1;
}

int main(int argc, char **argv)
{

	FInfo newFI;
	int optind;
	int ok;
	int i;


	optind = FlagsParse(argc, argv);

	argc -= optind;
	argv += optind;

	if (argc == 0)
	{
		FlagsHelp();
		return 0;
	}


	memset(&newFI, 0, sizeof(newFI));

	if (!flags._t && !flags._c) return 0;

	if (flags._t)
	{
		ok = checkcode(flags._t, (char *)&newFI.fdType);
		if (ok < 0)
		{
			fprintf(stderr, "SetFile: invalid file type: `%s`\n", flags._t);
			exit(1);
		}
	}

	if (flags._c)
	{
		ok = checkcode(flags._c, (char *)&newFI.fdCreator);
		if (ok < 0)
		{
			fprintf(stderr, "SetFile: invalid creator type: `%s`\n", flags._c);
			exit(1);
		}
	}

	for (i = 0; i < argc; ++i)
	{
		FInfo fi;
		char buffer[256];
		char *cp;
		int l;

		cp = argv[i];
		l = strlen(cp);
		if (l > 255)
		{
			fprintf(stderr, "SetFile: %s: file name too long.\n", cp);
			continue;
		}

		buffer[0] = l;
		memcpy(buffer + 1, cp, l);

		memset(&fi, 0, sizeof(fi));

		ok = GetFInfo((unsigned char *)buffer, 0, &fi);
		
		if (flags._t) fi.fdType = newFI.fdType;
		if (flags._c) fi.fdCreator = newFI.fdCreator;

		ok = SetFInfo((unsigned char *)buffer, 0, &fi);
		if (ok != 0)
		{
			fprintf(stderr, "SetFile: %s: unable to set finder info: %d\n", cp, ok);
		}
	}

	exit(0);
	return 0;
}