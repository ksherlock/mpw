#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <Files.h>

/*
Duplicate       # duplicate files and directories
Duplicate [-y | -n | -c] [-p] [-d | -r] name... target  > progress
    -y                      # overwrite target files (avoids dialog)
    -n                      # don't overwrite target files (avoids dialog)
    -c                      # cancel if conflict occurs (avoids dialog)
    -p                      # write progress information to diagnostics
    -d                      # duplicate data fork only
    -r                      # duplicate resource fork only
    -rs                     # resolve leaf aliases in the source path(s)
    -rt                     # resolve leaf aliases in the target path
    -f                      # preserve Finder icon locations

*/
// todo -- support src1 src2 ... dest/


char *c2p(const char *cp)
{
	int length;
	char *p;

	if (!cp) return NULL;
	length = strlen(cp);
	if (length > 255)
	{
		fprintf(stderr, "Error: Pathname is too long.\n");
		exit(1);
		return NULL;
	}

	p = malloc(length + 2); // + 2 for \0 and length.
	if (!p)
	{
		fprintf(stderr, "Error: unable to allocate memory.\n");
		exit(1);
		return NULL;
	}

	p[0] = length;
	memcpy(p + 1, cp, length + 1);
	return p;
}


void help(void)
{
	fprintf(stdout, "Usage: Duplicate [-y | -n | -c] [-p] [-d |-r] source destination\n");
}

int getopt(int *opts, int argc, char **argv)
{
	int i = 0;

	for (i = 1; i < argc; ++i)
	{
		char *str = argv[i];
		char c = str[0];

		if (c != '-') return i;
		++str;

		// -- to terminate
		if (str[0] == '-' && str[1] == 0)
			return i + 1;

		// skip -rt, -rs
		if (str[0] == 'r' && str[1] == 's' && str[2] == 0)
			continue;

		if (str[0] == 'r' && str[1] == 't' && str[2] == 0)
			continue;

		for (; *str; ++str)
		{
			c = *str;
			switch(c)
			{
				case 'r':
				case 'd':
					opts['r' - 'a'] = 0;
					opts['d' - 'a'] = 0;
					opts[c - 'a'] = 1;
					break;
				case 'y':
				case 'n':
				case 'c':
					opts['y' - 'a'] = 0;
					opts['n' - 'a'] = 0;
					opts['c' - 'a'] = 0;
					opts[c - 'a'] = 1;
					break;				
				case 'p':
					opts[c - 'a'] = 1;
					break;
				case 'h':
					help();
					exit(0);
					break;
				default:
					fprintf(stderr, "Duplicate - Invalid flag: \"%c\"\n", c);
					exit(1);
					break;

			}
		}
	}
	return i;
}

int copyFork(const char *src, const char *dest, unsigned fork)
{
	static char buffer[4096];

	int rfd, wfd;

	int rv;

	fork = fork ? O_RSRC : 0;

	rfd = open(src, O_RDONLY | O_BINARY | fork);
	if (rfd < 0)
	{
		fprintf(stderr, "Error opening %s: %s\n", src, strerror(errno));
		return -1;
	}

	// no 3rd parameter to open.
	wfd = open(dest, O_WRONLY | O_BINARY | O_CREAT |O_TRUNC| fork);
	if (wfd < 0)
	{
		fprintf(stderr, "Error opening %s: %s\n", dest, strerror(errno));
		close(rfd);
		return -1;
	}

	rv = -1;
	for (;;)
	{
		ssize_t rsize;
		ssize_t wsize;

		rsize = read(rfd, buffer, sizeof(buffer));
		if (rsize == 0)
		{
			rv = 0;
			break;
		}
		if (rsize < 0)
		{
			if (errno == EINTR) continue;
			fprintf(stderr, "Error reading %s: %s\n", src, strerror(errno));
			break;
		}

		wsize = write(wfd, buffer, rsize);
		if (wsize != rsize)
		{
			fprintf(stderr, "Error writing %s: %s\n", dest, strerror(errno));
			break;
		}
	}

	close(rfd);
	close(wfd);
	return rv;
}

int copyFinderInfo(const char *src, const char *dest)
{
	FInfo finderInfo;
	OSErr status;

	char *psrc;
	char *pdest;

	psrc = c2p(src);
	pdest = c2p(dest);
	if (!psrc || !pdest) return -1;

	// getfinfo/setfinfo seem to have bugs.

	memset(&finderInfo, 0, sizeof(finderInfo));

	status = GetFInfo((unsigned char *)psrc, 0, &finderInfo);
	if (status == 0)
	{
		status = SetFInfo((unsigned char *)pdest, 0, &finderInfo);
	}
	free(psrc);
	free(pdest);

	if (status) return -1;

	return 0;
}

int createFile(const char *src, const char *dest)
{
	FInfo finderInfo;
	OSErr status;

	char *psrc;
	char *pdest;

	psrc = c2p(src);
	pdest = c2p(dest);
	if (!psrc || !pdest) return -1;

	memset(&finderInfo, 0, sizeof(finderInfo));

	status = GetFInfo((unsigned char *)psrc, 0, &finderInfo);

	status = Create((unsigned char *)pdest, 0, finderInfo.fdCreator, finderInfo.fdType);
	free(psrc);
	free(pdest);

	if (status) return -1;
	return 0;
}

// -1 - error.
// 0 - no file
// 1 - regular file
// 2 - directory.
int mode(const char *path)
{
	char *pname;
	CInfoPBRec rec;
	OSErr status;

	memset(&rec, 0, sizeof(rec));

	pname = c2p(path);
	if (!pname) return -1;

	rec.hFileInfo.ioNamePtr = (unsigned char *)pname;
	status = PBGetCatInfo(&rec, false);
	free(pname);

	if (status) return 0;
	if (rec.hFileInfo.ioFlAttrib & kioFlAttribDirMask)
		return 2;

	return 1;
}

int main(int argc, char **argv)
{
	int opts[26];
	int optind;
	int ok;
	char *src;
	char *dest;
	int m;

	memset(opts, 0, sizeof(opts));

	opts['n' - 'a'] = 1;
	optind = getopt(opts, argc, argv);
	argc -= optind;
	argv += optind;

	if (argc != 2)
	{
		help();
		exit(1);
	}

	src = argv[0];
	dest = argv[1];
	
	// 1. check if src exists
	// 2. check if dest exists

	// 3. copy data fork unless -r
	// 4. copy resource fork unless -d
	// 5. copy finder info.

	ok = 0;
	// -n - do not overwrite
	// -c - cancel if conflict

	m = mode(dest);
	if (m == 2)
	{
		fprintf(stderr, "Error: directory destination is not yet supported.\n");
		exit(1);
	}
	if (m == 0 && opts['r' - 'a'])
	{
		// workaround to create the file if 
		// only copying the resource fork.

		// TODO -- call Create(name, 0, creator, filetype)

		if (opts['p' - 'a'])
			printf("Creating file %s\n", dest);
		ok = createFile(src, dest);
		if (ok < 0)
		{
			fprintf(stderr, "Error creating %s\n", dest);
			exit(1);
		}
	}

	if (m == 1)
	{
		// todo -- should this check at the file level or at the fork level?
		// seems to check at the file level.

		// file exists.
		if (opts['c' - 'a'] || opts['n' - 'a'])
		{
			if (opts['p' - 'a'])
			{
				printf("File exists - nothing done.\n");
			}
			exit(0);
		}
	}


	if (opts['r' - 'a'] == 0)
	{
		if (opts['p' - 'a'])
			printf("Copying Data Fork.\n");
		ok = copyFork(src, dest, 0);
	}

	if (opts['d' - 'a'] == 0)
	{
		if (opts['p' - 'a'])
			printf("Copying Resource Fork.\n");
		ok = copyFork(src, dest, 1);
	}

	return ok;
}