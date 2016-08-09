/*
 * Copyright (c) 2013, 2016, Kelvin W Sherlock
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

machine load_traps;
alphtype unsigned char;

action reset {
	negative = false;
	value = 0;
	key.clear();
	scratch.clear();
}

action commit {
	bool ok = true;

	if (!scratch.empty()) {
		auto iter = table.find(scratch);
		if (iter == table.end()) {
			fprintf(stderr, "Undefined reference: %s\n", scratch.c_str());
			ok = false;
		}
		else value = iter->second;
	}
	if (negative) value = -value;

	if (ok) table.emplace(std::move(key), value);
}

eol = [\r\n] ${ line++; };
ws = [ \t];

error := [^\r\n]* eol $reset ${ fnext main; };

comment = '#' [^\r\n]*;


word = [A-Za-z_][A-Za-z_0-9]*;
key = word ${ key.push_back(fc); };

base16 =
	  [0-9]    ${value = (value << 4) + fc - '0'; }
	| [A-Fa-f] ${value = (value << 4) + (fc | 0x20) - 'a' + 10; }
	;

base10 = [0-9]+ ${ value = (value * 10) + fc - '0'; };

decnumber = ( '-' ${ negative = true; } )?  base10+;
hexnumber = ('$' | '0x') base16+;

number = decnumber | hexnumber;

value = number | word ${ scratch.push_back(fc); };


line =
	ws* ( key ws* '=' ws* value %commit %reset ws*)? comment? eol
	;

 main := line*
 	$err{
 		fprintf(stderr, "Unexpected character '%c' on line %d\n", fc, line);
 		fnext error;
 	};

}%%;


namespace {
	%% write data;
}

#include <string>
#include <map>
#include <stdexcept>

#include <cstdint>
#include <cxx/mapped_file.h>

namespace Debug {

	void LoadTrapFile(const std::string &path, std::map<std::string, uint16_t> &table) {
		int cs;
		const unsigned char *p;
		const unsigned char *pe;
		const unsigned char *eof;
		int line = 1;

		int value = 0;
		bool negative = false;
		std::string key;
		std::string scratch;

		mapped_file mf;
		try {
			mf.open(path, mapped_file::readonly);
		} catch (const std::exception &ex) {
			//fprintf(stderr, "### %s: %s\n", path.c_str(), ex.what());
			return;
		}

		%% write init;
		p = mf.cbegin();
		pe = mf.cend();
		eof = nullptr;

		for (int i = 0; i < 2; ++i) {
			%% write exec;

			p = (const unsigned char *)"\n";
			pe = eof = p+1;
		}
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

		for(const auto &kv  : map)
		{
			printf("%s -> %04x\n", kv.first.c_str(), kv.second);
		}
	}

	return 0;
}

#endif