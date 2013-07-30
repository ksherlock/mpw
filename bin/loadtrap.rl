/*
 * Copyright (c) 2013, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <unordered_map>
#include <string>
#include <cstdio>

%%{
	
	machine lexer;

	action error {
		fprintf(stderr, "Invalid line: %d %.*s", line, (int)length, cp);
		fprintf(stderr, "%s - %x\n", name.c_str(), trap);
	}

	action addx {
		trap = (trap << 4) + digittoint(fc);
	}

	action emplace {

		auto iter = map.find(name);

		if (iter == map.end())
		{
			map[std::move(name)] = trap;
		}
		else
		{
			if (iter->second != trap)
			{
				fprintf(stderr, "Warning: redefining %s ($%04x -> $%04x)\n", 
					name.c_str(),
					iter->second, 
					trap
				);
				iter->second = trap;
			}
		}

	}

	ws = [ \t];


	value = 
		'0x' >{trap = 0; } xdigit+ @addx
		|
		'$' >{trap = 0; } xdigit+ @addx
		# todo -- add identifiers?
		;


	line :=
		[_A-Za-z][_A-Za-z0-9]* @{
			name.push_back(fc);
		}

		ws*
		'='
		ws*
		value
		'\n' @emplace
	;

	comment := any* ${ fbreak; };

	main := |*
		ws; # leading space
		'\n'; # blank line.
		'#' => { fgoto comment; };
		[A-Za-z_] => { fhold; fgoto line; };
		*|;

}%%

namespace Debug {
std::unordered_map<std::string, uint32_t> LoadTrapFile(const std::string &path)
{
%% write data;

	FILE *fp;

	std::unordered_map<std::string, uint32_t> map;

	fp = fopen(path.c_str(), "r");
	if (!fp)
	{
		fprintf(stderr, "Unable to open file %s\n", path.c_str());
		return map;
	}

	// todo -- consider using the debugger parser?  That would allow
	// full mathematical expressions.

	int line = 0;
	for(;;)
	{
		char *cp;

		size_t length;
		cp = fgetln(fp, &length);
		if (!cp) break;
		++line;

		#if 0
		while (isspace(*cp) && length)
		{
			++cp;
			length--;
		}
		if (!length) continue;
		#endif

		while (length && isspace(cp[length - 1]))
			length--;
		if (!length) continue;

		std::string buffer(cp, cp + length);
		buffer.push_back('\n');

		const char *p = buffer.c_str();
		const char *pe = p + buffer.length();
		//const char *eof = pe;
		const char *ts;
		const char *te;

		int cs;
		int act;

		std::string name;
		uint32_t trap = 0;

		%% write init;
		/* ... */
		%% write exec;

		if (cs == lexer_error)
		{
			fprintf(stderr, "Bad line: %.*s\n", (int)length, cp);
		}
	}

	fclose(fp);
	return map;
}

}


#ifdef TEST

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string f(argv[i]);

		auto map = Debug::LoadTrapFile(f);

		for(const auto kv  : map)
		{
			printf("%s -> %04x\n", kv.first.c_str(), kv.second);
		}
	}

	return 0;
}

#endif

