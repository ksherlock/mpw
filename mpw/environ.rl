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
 
%%{
	
	machine lexer;


	ws = [ \t];

	word = [A-Za-z0-9_];

	action emplace {
		//printf("emplacing %s\n", name.c_str());
		// trim any whitespace.
		//while (value.length() && isspace(value.back()))
		//	value.pop_back();
		// ?= only sets if not already set.

		auto iter = env.find(name);
		if (iter == env.end())
		{
			env[std::move(name)] = std::move(value);
		}
		else
		{
			if (overwrite)
				iter->second = std::move(value);
		}	
	}

	value := |*

		'$' word+  => {
			std::string name(ts + 1, te);
			auto iter = env.find(name);
			if (iter != env.end())
				value.append(iter->second);
		};
		
		'${' word+ '}'  => {
			std::string name(ts + 2, te - 1);
			auto iter = env.find(name);
			if (iter != env.end())
				value.append(iter->second);
		};

		'$$' => {
			value.push_back('$');
		};

		'\n' => emplace;

		any - '\n' => {
			value.push_back(fc);
		} ;


		*|;


	# exit w/ cs = lexer_en_comment.

	comment := any* ${ fbreak; };

	assignment := 
		word+ ${ name.push_back(fc); }
		ws*
		(
			'?=' ${ overwrite = false; }
			|
			'=' ${ overwrite = true; }
		)
		ws*
		# ws does not include '\n', so that will be handled
		# as a value.
		(any - ws)? ${ fhold; fgoto value; }
	;

	main := |*
		ws; # leading space

		'\n'; # blank line.

		'#' => { fgoto comment; };

		word => { fhold; fgoto assignment; };
	*|;
}%%

#include <string>
#include <unordered_map>
#include <cstdio>
#include <cctype>




namespace MPW {

void LoadEnvironment(std::string &envfile, std::unordered_map<std::string, std::string> &env)
{
	%% write data;

	FILE *fp;


	fp = fopen(envfile.c_str(), "r");

	if (!fp) return;

	/*
	 * fgetln (for something more portable, try getline)
	 * may or may not include the trailing \r. at the eof.
	 * To compensate, trim all trailing whitespace and run it
	 * a second time (with eof=pe) on a string of "\n"
	 *
	 */

	for(;;)
	{
		const char *line;
		size_t length;

		line = fgetln(fp, &length);
		if (!line) break; // eof or error.

		// strip any trailing space.
		while (length && isspace(line[length - 1]))
			length--;

		if (!length) continue;

		std::string name;
		std::string value;

		std::string buffer(line, line + length);
		buffer.push_back('\n');

		const char *p = buffer.c_str();
		const char *pe = p + buffer.length();
		const char *eof = pe;
		const char *te;
		const char *ts;
		bool overwrite = true;

		int cs, act;

		%%write init;

		%%write exec;

		if (cs == lexer_error)
		{
			fprintf(stderr, "Bad environment: %.*s\n", (int)length, line);
			continue;
		}
	}

	fclose(fp);
}

} // namespace

#ifdef TEST
// clang++ -DTEST -std=c++11 -stdlib=libc++ environ.cpp
int main(int argc, char **argv)
{

	for (int i = 1; i < argc; ++i)
	{
		std::unordered_map<std::string, std::string> env;
		std::string f(argv[i]);

		MPW::LoadEnvironment(f, env);

		for (const auto &kv : env)
		{
			printf("%s -> %s\n", kv.first.c_str(), kv.second.c_str());
		} 
	}

	return 0;
}


#endif