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


/*
 *
 * file                -> file
 * :directory:file     -> directory/file
 * volume:directory    -> /volume/directory
 * ::                  -> ../
 * :::                 -> ../../
 */

%%{
	
	machine m;

	main := |*


		':' {2,} {

			// :: = ..
			// ::: = ../..
			// etc
			unsigned count = te - ts;

			if (ts != begin)
			{
				rv.push_back('/');
			}
			for (unsigned i = 1; i < count; ++i)
			{
				rv.append("../");
			}
		};

		':' {
			if (ts != begin)
				rv.push_back('/');
		};


		any {
			rv.push_back(*ts);
		};


	*|;

}%%

#include <string>
#include "mpw.h"

namespace {

	%%write data;

}

namespace MPW {
	
	std::string ExpandPathVariables(std::string);

	std::string ExpandPath(std::string s)
	{
		std::string rv;


		// 1. expand any variables.

#ifndef TESTING
		if (s.find('{') != s.npos)
		{
			s = ExpandPathVariables(s);
		}
#endif
		// 2. does it even need it?
		unsigned sep = 0;
		for (char c : s)
		{
			if (c == ':' || c == '/')
			{
				sep = c;
				break;
			}
		}

		if (!sep) return s;

		// convert volume:path to /volume/path
		// leading / manually placed.
		if (sep == ':' && s.front() != ':')
			rv.push_back('/');

		auto begin = s.c_str();
		auto p = begin;
		auto pe = p + s.length();
		auto eof = pe;

		const char *ts;
		const char *te;

		int cs;
		int act;

		%%write init;

		%%write exec;

		return rv;
	}


}

#if TESTING

#include <cstdio>
int main(int argc, char **argv)
{
	
	for (int i = 1; i < argc; ++i)
	{
		std::string s(argv[i]);
		printf("%s  ->  ", s.c_str());
		s = MPW::ExpandPath(s);
		printf("%s\n", s.c_str());
	}
	
	return 0;
}

#endif