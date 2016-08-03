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

#include <map>
#include <string>
#include <cstdio>

#include <sys/types.h>
#include <limits.h>

namespace _loadtrap_rl {
#if __APPLE__ && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
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
#if __APPLE__ && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
        return getdelim(lineptr, n, '\n', stream);
#else
        return ::getline(lineptr, n, stream);
#endif
    } 
}
namespace {
	// private...
	%%{
		
		machine lexer;

		action addx {
			value = (value << 4) + digittoint(fc);
		}

		ws = [ \t];

		value = 
			'0x' xdigit+ @addx
			|
			'$' xdigit+ @addx
			|
			'-'? @{ negative = true; }
			digit+ @{value = value * 10 + (fc - '0'); }
			|
			[A-Za-z_] @{ stringValue.push_back(fc); }
			[A-Za-z0-9_]+ @{ stringValue.push_back(fc); }
			;


		name = 	
			[A-Za-z0-9_]+ @{
				name.push_back(fc);
			}
		;

		main := 

			name
			ws*
			'='
			ws*
			value
		;

		write data;
	}%%



	bool ParseLine(const char *p, const char *pe, std::map<std::string, uint16_t> &map)
	{

		// trap = number
		// or trap = [previously defined trap]

		std::string name;
		std::string stringValue;
		uint32_t value;
		bool negative = false;

		value = 0;
		const char *eof = pe;
		int cs;

		%%write init;
		%%write exec;

		if (cs < %%{ write first_final; }%% )
		{
			return false;
		}
		if (negative) value = (-value) & 0xffff;

		// name lookup
		if (!stringValue.empty())
		{
			auto iter = map.find(stringValue);
			if (iter == map.end())
			{
				fprintf(stderr, "Undefined trap: %s\n", stringValue.c_str());
				return false;
			}
			value = iter->second;
		}

		#if 0
		// if loading globals, wouldn't need to do this...
		if (value > 0xafff || value < 0xa000)
		{
			fprintf(stderr, "Invalid trap number: $%04x\n", value);
			return false;
		}
#endif

		map.emplace(name, (uint16_t)value);


		return true;
	}



}

namespace Debug {

	void LoadTrapFile(const std::string &path, std::map<std::string, uint16_t> &map)
	{
		FILE *fp;

		fp = fopen(path.c_str(), "r");
		if (!fp)
		{
			fprintf(stderr, "Unable to open trap file %s\n", path.c_str());
			return;
		}


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

			length = _loadtrap_rl::getline(&lineBuffer, &lineSize, fp);
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

			if (!ParseLine(line, line + length, map))
			{
				fprintf(stderr, "Error in trap definition: %s\n", line);
			}
		}

		fclose(fp);
	}




}


#ifdef TEST

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string f(argv[i]);
		std::map<std::string, uint16_t> map;
		Debug::LoadTrapFile(f, map);

		for(const auto kv  : map)
		{
			printf("%s -> %04x\n", kv.first.c_str(), kv.second);
		}
	}

	return 0;
}

#endif
