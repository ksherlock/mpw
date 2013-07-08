
#include <unordered_map>
#include <string>
#include <cstdio>

#include "debugger.h"

%%{
	
	machine traplist;

	action error {
		fprintf(stderr, "Invalid line: %d %.*s", line, (int)length, cp);
		fprintf(stderr, "%s - %x\n", name.c_str(), trap);
	}

	main :=
		[ \t]*
		[_A-Za-z][_A-Za-z0-9]* @{
			name.push_back(fc);
		}
		[ \t]*
		'='
		[ \t]*
		('0x' | '$')
		[0-9A-Fa-f]+ @{ 
			trap = (trap << 4) + digittoint(fc);
		}
		[ \t\r\n]*

		%eof {
			// final state
			map.emplace(name, trap);
		}
		@eof(error)
		$err(error)
		
		;

}%%

namespace Debug {
std::unordered_map<std::string, uint16_t> LoadTrapFile(const std::string &path)
{
%% write data nofinal;

	FILE *fp;

	std::unordered_map<std::string, uint16_t> map;

	fp = fopen(path.c_str(), "r");
	if (!fp)
	{
		fprintf(stderr, "Unable to open file %s\n", path.c_str());
		return map;
	}

	int line = 0;
	for(;;)
	{
		char *cp;

		size_t length;
		cp = fgetln(fp, &length);
		if (!cp) break;
		++line;

		while (isspace(*cp) && length)
		{
			++cp;
			length--;
		}
		if (!length) continue;
		if (*cp == '#') continue;

		char *p = cp;
		char *pe = p + length;
		char *eof = p + length;
		int cs;

		std::string name;
		uint16_t trap = 0;

		%% write init;
		/* ... */
		%% write exec;
	}

	fclose(fp);
	return map;
}

}
