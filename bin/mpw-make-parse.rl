
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

	comment := |*
		'\n' => { fhold; fgoto main; };
		any => {};
	*|;

	main := |*

	0xb6 '\n' => {
		// line continuation marker.
		// terminate the current token?
	};


	'\n' => {
		// terminate the command.
		if (!token.empty()) {
			argv.push_back(std::move(token));
			token.clear();		
		}
		if (!argv.empty()) {
			int ok = launch_command(argv);
			if (ok != 0) {
				fprintf(stderr, "command failed with exit code %d\n", ok);
				return ok;
			}
		}

		argv.clear();
	};

	[ \t]+ => {
		// token separator.
		if (!token.empty()) {
			argv.push_back(std::move(token));
			token.clear();
		}
	};

	'"{' [A-Za-z][A-Za-z0-9]* '}"' => {
		// environment variable.  remove quotes.
		// also expand if possible.
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

	'#' => {
		// comment to eol.
		fgoto comment;
	};

	any => {
		token.push_back(*ts);
	};

	*|;
}%%

namespace {

	%% write data nofinal;
	
}

//extern int launch_command(std::vector<char *> &argv);
extern int launch_command(const std::vector<std::string> &argv);

int parse_makefile(int fd) {
	
	std::string token;
	std::vector<std::string> argv;

	unsigned char buffer[4096];

	const unsigned char *ts;
	const unsigned char *te;
	const unsigned char *p;
	const unsigned char *pe;
	const unsigned char *eof;

	int cs, act;

	unsigned offset = 0;


	%% write init;


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
	if (!argv.empty()) {
		fprintf(stderr, "warning: unterminated line\n");

		int ok = launch_command(argv);
		if (ok != 0) {
			fprintf(stderr, "command failed with exit code %d\n", ok);
			return ok;
		}
	}

	return 0;
}
