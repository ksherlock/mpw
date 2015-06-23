
/*
 * ragel code to extract commands from mpw make output
 */

#include <vector>
#include <unistd.h>
#include <string>
#include <cstring>
#include <cerrno>

#include <stdlib.h>
#include <sysexits.h>

 /*

PascalIIGS IRModule.p
asmiigs TheHeader.aii

reziigs IRModule.r -o IRModule

LinkIIGS TheHeader.aii.obj IRModule.p.obj ?
	-lib "{PIIGSLibraries}"PLib ?
	-o IRModule ?
	-t $BC -at $4001
	DuplicateIIgs -y -m IRModule :

*/

%%{
	machine make;
	alphtype unsigned char;

	main := |*

	0xb6 '\n' => {
		// line continuation marker.
		// terminate the current token?
	};


	'\n' => {
		// terminate the command.
		if (!token.empty()) {
			argv.push_back(strdup(token.c_str()));
			token.clear();		
		}
		if (argv.size() > 1) {
			int ok = launch_command(argv);
			if (ok != 0) {
				fprintf(stderr, "command failed with exit code %d\n", ok);
				return ok;
			}
		}

		for (auto cp : argv) { free(cp); }
		argv.clear();
		argv.push_back(strdup("mpw"));
	};

	[ \t]+ => {
		// token separator.
		if (!token.empty()) {
			argv.push_back(strdup(token.c_str()));
			token.clear();
		}
	};

	'"{' [A-Za-z][A-Za-z0-9]* '}"' => {
		// environment variable.  remove quotes.
		token.append(ts + 1, te - 1);
	};

	['] [^']* ['] => {
		// quoted string -- remove quotes.
		token.append(ts + 1, te - 1);
	};

	["] [^"]* ["] => {
		// quoted string -- remove quotes.
		token.append(ts + 1, te - 1);
	};



	any => {
		token.push_back(*ts);
	};

	*|;
}%%

namespace {

	%% write data nofinal;
	
}

extern int launch_command(std::vector<char *> &argv);

int parse_makefile(int fd) {
	
	std::string token;
	std::vector<char *> argv;

	unsigned char buffer[4096];

	const unsigned char *ts;
	const unsigned char *te;
	const unsigned char *p;
	const unsigned char *pe;
	const unsigned char *eof;

	int cs, act;

	unsigned offset = 0;


	%% write init;

	// strdup so it can be free() later.
	argv.push_back(strdup("mpw"));


	bool done = false;
	while (!done) {
		ssize_t s;
		s = read(fd, buffer + offset, sizeof(buffer) - offset);
		if (s < 0) {
			if (errno == EINTR) continue;
			perror("read: ");
			exit(EX_OSERR);
		}

		p = buffer + offset;
		pe = p + s;
		if (s == 0) {
			done = true;
			eof = pe;
		}

		%% write exec;

		if (cs == make_error) {
			fprintf(stderr, "lexer error\n");
			exit(EX_DATAERR);
		}
		// if inside a token, shift data around.

		if (ts == 0) {
			offset = 0;
		} else {
			offset = pe - ts;
			memmove(buffer, ts, offset);

			te = buffer + (te - ts);
			ts = buffer;

			if (offset == sizeof(buffer)) {
				fprintf(stderr, "buffer exceeded\n");
				exit(EX_SOFTWARE);
			}

		}


	}
	// any remaining argv?
	if (argv.size() > 1) {
		fprintf(stderr, "warning: unterminated line\n");

		int ok = launch_command(argv);
		if (ok != 0) {
			fprintf(stderr, "command failed with exit code %d\n", ok);
			return ok;
		}
	}

	for (auto cp : argv) { free(cp); }
	argv.clear();

	return 0;
}
