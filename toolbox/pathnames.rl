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


#include <string>

#include "toolbox.h"

#ifndef TESTING
#include <mpw/mpw.h>
#endif


namespace {
	

	/*
	 * To Mac:
	 * file                -> file
	 * :directory:file     -> directory/file
	 * volume:directory    -> /volume/directory
	 * ::                  -> ../
	 * :::                 -> ../../
	 *
	 *
	 * To Unix:
	 * file                -> file
	 * directory/file      -> :directory:file
	 * /volume/file        -> volume:file
	 *
	 */


	%%{
		machine mac;

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

		write data;
	}%%

	%%{
		machine unix;

		main := |*

			'/' + {
				// collapse multiple /s to a single ':'
				if (ts != begin)
				{
					rv.push_back(':');
					slash = true;
				}
			};

			any {
				rv.push_back(*ts);
			};

		*|;

		write data;
	}%%

}

namespace ToolBox
{

	std::string MacToUnix(std::string path)
	{

		// todo -- Dev:Null -> lowercase it?

		#ifndef TESTING
		if (path.find_first_of("${") != path.npos)
		{
			path = MPW::ExpandVariables(path);
		}
		#endif

		int sep = 0;
		bool colon = false;

		for (char c : path)
		{
			switch (c)
			{
			case ':':
				colon = true;
			case '/':
				if (!sep) sep = c;
				break;

			}
			if (colon) break;
		}

		// no colon - no problem.
		if (!colon) return path;


		const char *p = path.c_str();
		const char *pe = p + path.length();
		const char *eof = pe;
		const char *ts;
		const char *te;

		const char *begin = p;
		int cs;
		int act;

		std::string rv;

		// volume:directory -> /volume
		if (sep == ':' && path.front() != ':')
			rv.push_back('/');

		%%{
			machine mac;

			write init;
			write exec;
		}%%


		return rv;
	}


	std::string UnixToMac(std::string path)
	{
		// /volume/directory -> volume:directory
		// // -> /


		bool slash = false;
		bool sep = 0;

		for (char c : path)
		{
			switch(c)
			{
			case '/':
				slash = true;
			case ':':
				if (!sep) sep = c;
				break;
			}
			if (slash) break;
		}

		if (!slash) return path;

		const char *p = path.c_str();
		const char *pe = p + path.length();
		const char *eof = pe;
		const char *ts;
		const char *te;

		const char *begin = p;
		int act;
		int cs;

		std::string rv;


		slash = false;
		// path/file -> :path:file
		if (path.front() != '/')
		{
			rv.push_back(':');
			slash = true;
		}
		%%{
			machine unix;

			write init;
			write exec;
		}%%

		// special check  /dir -> dir:
		if (path.front() == '/' && !slash) rv.push_back(':');

		return rv;
	}


}


#ifdef TESTING

#include <cstdio>

int main(int argc, char **argv)
{
	
	for (int i = 1; i < argc; ++i)
	{
		std::string s(argv[i]);
		std::string a = ToolBox::MacToUnix(s);
		std::string b = ToolBox::UnixToMac(s);

		if (s != a)
			printf("ToUnix: %s -> %s\n", s.c_str(), a.c_str());

		if (s != b)
			printf("ToMac:  %s -> %s\n", s.c_str(), b.c_str());


	}

	return 0;
}

#endif
