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
#include <unordered_map>
#include <cstdio>
#include <vector>

#include <sys/types.h>
#include <limits.h>

namespace _env_rl {
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
    #define _GETDELIM_GROWBY 128 /* amount to grow line buffer by */
    #define _GETDELIM_MINLEN 4 /* minimum line buffer size */
     
    ssize_t getdelim(char ** lineptr, size_t * n, int delimiter, FILE * stream) {
        char *buf, *pos;
        int c;
        ssize_t bytes;
         
        if (lineptr == NULL || n == NULL) {
            errno = EINVAL;
            return -1;
        }
        if (stream == NULL) {
            errno = EBADF;
            return -1;
        }
         
        /* resize (or allocate) the line buffer if necessary */
        buf = *lineptr;
        if (buf == NULL || *n < _GETDELIM_MINLEN) {
            buf = (char*)realloc(*lineptr, _GETDELIM_GROWBY);
            if (buf == NULL) {
                /* ENOMEM */
                return -1;
            }
            *n = _GETDELIM_GROWBY;
            *lineptr = buf;
        }
         
        /* read characters until delimiter is found, end of file is reached, or an
        error occurs. */
        bytes = 0;
        pos = buf;
        while ((c = getc(stream)) != EOF) {
            if (bytes + 1 >= SSIZE_MAX) {
                errno = EOVERFLOW;
                return -1;
            }
            bytes++;
            if (bytes >= *n - 1) {
                buf = (char*)realloc(*lineptr, *n + _GETDELIM_GROWBY);
                if (buf == NULL) {
                    /* ENOMEM */
                    return -1;
                }
                *n += _GETDELIM_GROWBY;
                pos = buf + bytes - 1;
                *lineptr = buf;
            }
             
            *pos++ = (char) c;
            if (c == delimiter) {
                break;
            }
        }
         
        if (ferror(stream) || (feof(stream) && (bytes == 0))) {
            /* EOF, or an error from getc(). */
            return -1;
        }
         
        *pos = '\0';
        return bytes;
    }
#endif
     
     
    ssize_t getline(char ** lineptr, size_t * n, FILE * stream) {
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
        return getdelim(lineptr, n, '\n', stream);
#else
        return ::getline(lineptr, n, stream);
#endif
    } 
}

namespace MPW
{
	extern std::unordered_map<std::string, std::string> Environment;

	std::string ExpandVariables(const std::string &s, bool pathname = false);
}

namespace {


	%%{
		machine name;

		main :=

			[A-Za-z0-9_]+ @{
				name.push_back(fc);
			}
		;

		write data;
	}%%

	%%{
		machine assign;

		ws = [ \t\r\n];
		main :=

			ws*
			(
				'=' @{
					op = 1;
				}
				|
				'+=' @{
					op = 2;
				}
				|
				'?=' @{
					op = 3;
				}
			)
			ws*
		;

		write data;
	}%%

	%%{
		machine variables;	

		coalesce_colon := |*

			':' {
				if (rv.length() && rv.back() != ':')
					rv.push_back(':');

				fgoto main;
			};
			any {
				fhold;
				fgoto main;
			};
		*|;

		main := |*

			'{' [A-Za-z0-9_]+ '}' {

				std::string name(ts + 1, te - 1);
				auto iter = Environment.find(name);
				if (iter != Environment.end())
					rv.append(iter->second);

				fgoto coalesce_colon;
			};

			# backwards compatibility.
			'${' [A-Za-z0-9_]+ '}' {

				if (pathname) {
					rv.append(ts, te);
				} else {
					std::string name(ts + 2, te - 1);
					auto iter = Environment.find(name);
					if (iter != Environment.end())
						rv.append(iter->second);

					fgoto coalesce_colon;
				}
			};

			# backwards compatibility.
			# lcc generates temporary files named $xxx$
			# so don't replace in pathnames.
			'$' [A-Za-z0-9_]+ {
				if (pathname) {
					rv.append(ts, te);
				} else {
					std::string name(ts + 1, te);
					auto iter = Environment.find(name);
					if (iter != Environment.end())
						rv.append(iter->second);

					fgoto coalesce_colon;
				}
			};

			any {
				rv.push_back(*ts);
			};
		*|;

		write data;
	}%%

	// split out since goto names conflict.
	const char* GetName(const char *p, const char *pe, std::string &name)
	{
		const char *eof = pe;
		int cs;

		%%{
			machine name;
			write init;
			write exec;	
		}%%

		return p;
	}

	const char *GetAssignment(const char *p, const char *pe, unsigned &op)
	{
		const char *eof = pe;
		int cs;

		%%{
			machine assign;
			write init;
			write exec;	
		}%%

		return p;
	}

	bool ParseLine(const char *p, const char *pe)
	{
		/*
		 * name = value
		 * name += value
		 * name ?= value
		 */

		std::string name;
		unsigned op = 0;

		const char *begin = p;

		// 1. get the name
		p = GetName(p, pe, name);
		if (!name.length())
		{
		#ifdef TESTING
			fprintf(stderr, "No Name\n");
		#endif
			return false;
		}

		// 2. get the op
		p = GetAssignment(p, pe, op);
		if (!op)
		{
			fprintf(stderr, "No assignment\n");
			return false;
		}

		std::string value(p, pe);
		value = MPW::ExpandVariables(value);

		auto iter = MPW::Environment.find(name);
		if (iter == MPW::Environment.end())
		{
			MPW::Environment.emplace(std::move(name), std::move(value));
		}
		else
		{
			switch(op)
			{
			case 1: // =
				iter->second = std::move(value);
				break;
			case 2: // +=
				iter->second += value;
				break;
			case 3: // ?=
				break;

			}
		}
		return true;
	}

}

namespace MPW {


	std::string GetEnv(const std::string &key)
	{
		auto iter = Environment.find(key);
		if (iter == Environment.end()) return "";
		return iter->second;
	}

	std::string ExpandVariables(const std::string &s, bool pathname)
	{
		if (s.find_first_of("{$") == s.npos) return s;

		std::string rv;
		const char *p = s.c_str();
		const char *pe = p + s.length();
		const char *eof = pe;
		const char *te;
		const char *ts;

		int cs;
		int act;

		%%{
			machine variables;

			write init;
			write exec;
		}%%

		return rv;
	}

	void EnvLoadArray(const std::vector<std::string> &data)
	{
		for (const auto &s : data)
		{
			const char *begin = s.c_str();
			const char *end = begin + s.length();
			if (!s.length()) continue;
			if (!ParseLine(begin, end))
			{
				fprintf(stderr, "Error in variable: %s\n", s.c_str());
			}
		}
	}


	void EnvLoadFile(const std::string &envfile)
	{
		
		FILE *fp;


		fp = fopen(envfile.c_str(), "r");

		if (!fp) return;


		/*
		 * getline(3) is 2008 posix. it allocates (and resizes as appropriate)
		 * the buffer.
		 *
		 */
		char *lineBuffer = NULL;
		size_t lineSize = 0;

		for(;;)
		{
			char *line;
			ssize_t length;

			length = _env_rl::getline(&lineBuffer, &lineSize, fp);
			if (!length) continue; //?
			if (length < 0) break; // eof or error.

			line = lineBuffer;

			// skip any leading space.
			while (length && isspace(*line))
			{
				++line;
				--length;
			}
			if (!length) continue;

			// comments
			if (*line == '#') continue;


			// strip any trailing space.
			// (will be \n terminated unless there was no \n)
			while (length && isspace(line[length - 1]))
			{
				line[--length] = 0;
			}
			if (!length) continue;

			if (!ParseLine(line, line + length))
			{
				fprintf(stderr, "Error in variable: %s\n", line);
			}
		}

		fclose(fp);
	}

} // namespace

#ifdef TESTING

namespace MPW
{
	std::unordered_map<std::string, std::string> Environment;
}


int main(int argc, char **argv)
{

		
	for (int i = 1; i < argc; ++i)
	{
		char *cp = argv[i];
		int len = strlen(cp);

		if (!ParseLine(cp, cp + len))
		{
			fprintf(stderr, "Error: %s\n", cp);
		}
	}

	MPW::EnvLoadFile("/Users/kelvin/mpw/Environment.text");	

	for (auto kv : MPW::Environment)
	{
		printf("%s --> %s\n", kv.first.c_str(), kv.second.c_str());
	}

	return 0;
}

#endif
