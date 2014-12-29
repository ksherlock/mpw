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
#include <algorithm>
#include <numeric>
#include <memory>
#include <deque>

#include <cstdint>
#include <stdlib.h>

#include "debugger.h"
#include "parser.h"


	void *ParseAlloc(void *(*mallocProc)(size_t));
	void ParseFree(void *p, void (*freeProc)(void*));
	void Parse(void *yyp, int yymajor, Debug::Token yyminor, Debug::Command *command);
	void ParseTrace(FILE *TraceFILE, char *zTracePrompt);


	// for the common case....
	void Parse(void *yyp, int yymajor, uint32_t yyminor, Debug::Command *command)
	{
		Parse(yyp, yymajor, Debug::Token::Make(yyminor), command);
	}


namespace {

	int tox(char c)
	{
		c |= 0x20; // lowercase it.
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		return 0;
	}

	uint32_t scan10(const char *begin, const char *end)
	{
		return std::accumulate(begin, end, 0, 
			[](uint32_t value, char c){
				return value * 10 + c - '0';
			});
	}

	uint32_t scan16(const char *begin, const char *end)
	{
		return std::accumulate(begin, end, 0, 
			[](uint32_t value, char c){
				return (value << 4) + tox(c);
			});
	}

	uint32_t scan2(const char *begin, const char *end)
	{
		return std::accumulate(begin, end, 0, 
			[](uint32_t value, char c){
				return (value << 1) + (c & 0x01);
			});
	}

	uint32_t scancc(const char *begin, const char *end)
	{
		return std::accumulate(begin, end, 0,
			[](uint32_t value, char c)
			{
				return (value << 8) + (unsigned)c;
			}
		);

	}
}




%%{
	machine lexer;

	# this exits with cs == lexer_en_error.
	error := any* ${ fbreak; };

	# identifiers.
	ident := |*

		[ \t\r\n]+;

		[_A-Za-z][_A-Za-z0-9]* {

			std::unique_ptr<std::string> sp(new std::string(ts, te));

			Parse(parser, tkIDENTIFIER, Token::Make(sp.get(), 0), command);
			Strings.push_back(std::move(sp));
		};

	*|;

	# semi-colon commands.
	semi := |*

		[ \t\r\n]+;

		'h'i | 'hd'i | 'hexdump'i {
			Parse(parser, tkSEMIH, 0, command);
		};

		'i'i | 'info'i {
			Parse(parser, tkSEMII, 0, command);
		};

		'l'i | 'list'i {
			Parse(parser, tkSEMIL, 0, command);
		};

		'date'i {
			Parse(parser, tkSEMIDATE, 0, command);
		};

		'error'i {
			Parse(parser, tkSEMIERROR, 0, command);
		};

		't'i {
			Parse(parser, tkSEMIT, 0, command);
			fgoto ident;
		};

	*|;

	main := |*

		[ \t\r\n]+;

		'>=' { Parse(parser, tkGTEQ, 0, command); };
		'>>' { Parse(parser, tkGTGT, 0, command); };
		'<=' { Parse(parser, tkLTEQ, 0, command); };
		'<<' { Parse(parser, tkLTLT, 0, command); };
		'!=' { Parse(parser, tkBANGEQ, 0, command); };
		'==' { Parse(parser, tkEQEQ, 0, command); };
		'||' { Parse(parser, tkPIPEPIPE, 0, command); };
		'&&' { Parse(parser, tkAMPAMP, 0, command); };


		'(' { Parse(parser, tkLPAREN, 0, command); };
		')' { Parse(parser, tkRPAREN, 0, command); };
		'=' { Parse(parser, tkEQ, 0, command); };
		'+' { Parse(parser, tkPLUS, 0, command); };
		'-' { Parse(parser, tkMINUS, 0, command); };
		'*' { Parse(parser, tkSTAR, 0, command); };
		'/' { Parse(parser, tkSLASH, 0, command); };
		'%' { Parse(parser, tkPERCENT, 0, command); };
		'~' { Parse(parser, tkTILDE, 0, command); };
		'!' { Parse(parser, tkBANG, 0, command); };
		'^' { Parse(parser, tkCARET, 0, command); };
		'&' { Parse(parser, tkAMP, 0, command); };
		'|' { Parse(parser, tkPIPE, 0, command); };
		'<' { Parse(parser, tkLT, 0, command); };
		'>' { Parse(parser, tkGT, 0, command); };

		':' { Parse(parser, tkCOLON, 0, command); };

		'@' { Parse(parser, tkAT, 0, command); };

		';' {
			Parse(parser, tkSEMI, 0, command);
			fgoto semi;
		};


		'$' xdigit + {
			uint32_t value = scan16(ts + 1, te);
			Parse(parser, tkINTEGER, value, command);
		};

		'0x'i xdigit+ {
			// hexadecimal
			uint32_t value = scan16(ts + 2, te);
			Parse(parser, tkINTEGER, value, command);
		};

		'0b'i [01]+ {
			// binary
			uint32_t value = scan2(ts + 2, te);
			Parse(parser, tkINTEGER, value, command);
		};

		digit+ {
			uint32_t value = scan10(ts, te);
			Parse(parser, tkINTEGER, value, command);
		};

		['] [^']{1,4} ['] {
			// 4 cc code

			uint32_t value = scancc(ts + 1, te - 1);
			Parse(parser, tkINTEGER, value, command);

		};

		'd'i [0-7] {
			// data register
			uint32_t data = ts[1] - '0';
			Parse(parser, tkDREGISTER, data, command);
		};

		'a'i [0-7] {
			// address register
			uint32_t data = ts[1] - '0';
			Parse(parser, tkAREGISTER, data, command);
		};

		'pc'i {
			// program counter...
			Parse(parser, tkXREGISTER, 0, command);
		};

		'csr'i {
			// condition status register.
			Parse(parser, tkXREGISTER, 1, command);
		};

		'sp'i {
			// stack pointer aka a7
			Parse(parser, tkAREGISTER, 7, command);
		};

		'fp'i {
			// frame pointer aka a6
			Parse(parser, tkAREGISTER, 6, command);
		};

		# commands...

		'bt'i | 'backtrace'i {
			Parse(parser, tkBACKTRACE, 0, command);
		};

		'c'i | 'continue'i {
			Parse(parser, tkCONTINUE, 0, command);
		};

		'hd'i | 'dump'i {
			Parse(parser, tkDUMP, 0, command);
		};

		'h'i | 'help'i {
			Parse(parser, tkHELP, 0, command);
		};

		'l'i | 'list'i {
			Parse(parser, tkLIST, 0, command);
		};

		'n'i | 'next'i {
			Parse(parser, tkNEXT, 0, command);
		};

		's'i | 'step'i {
			Parse(parser, tkNEXT, 0, command);
		};

		'b'i | 'brk'i | 'break'i {
			Parse(parser, tkBREAK, 0, command);
		};

		'tbrk'i | 'tbreak'i | 'toolbreak'i {
			Parse(parser, tkTBREAK, 0, command);
		};

		'mbrk'i | 'mbreak'i | 'rwbrk'i | 'rwbreak'i {
			Parse(parser, tkRWBREAK, 0, command);
		};

		'rbrk'i | 'rbreak'i {
			Parse(parser, tkRBREAK, 0, command);
		};

		'wbrk'i | 'wbreak'i {
			Parse(parser, tkWBREAK, 0, command);
		};


		'g'i | 'go'i {
			Parse(parser, tkCONTINUE, 0, command);
		};

		'p'i | 'print'i {
			Parse(parser, tkPRINT, 0, command);
		};
	
		'r'i | 'run'i {
			Parse(parser, tkCONTINUE, 0, command);
		};


		# generic identifier
		# since % is a valid character, should drop %/modulo operator.
		[%_A-Za-z][%_.A-Za-z0-9]* {

			std::unique_ptr<std::string> sp(new std::string(ts, te));

			Parse(parser, tkIDENTIFIER, Token::Make(sp.get(), 0), command);
			Strings.push_back(std::move(sp));
		};


		# end of the line...
		#0 {
		#	Parse(parser, tkEOL, 0, command);
		#};

		#any {
		#	fprintf(stderr, "illegal character: `%c`\n", *ts);
		#	fbreak;
		#};


	*|;
}%%

namespace Debug {

bool ParseLine(const char *iter, Command *command)
{
	%% write data;

	void *parser;

	// string table to avoid memory leaks in parser.
	std::deque<std::unique_ptr<std::string>> Strings;


	parser = ParseAlloc(malloc);

	//ParseTrace(stdout, "--> ");
	command->action = cmdNull;

	int length = strlen(iter);
	const char *p = iter;
	const char *pe = iter + length;
	const char *eof = pe;
	const char *ts;
	const char *te;
	int cs, act;

	for(;;)
	{

		%% write init;
		%% write exec;

		if (cs == lexer_error)
		{
			fprintf(stderr, "illegal character: `%c'\n", *p);
			ParseFree(parser, free);
			return false;
		}
		if (cs == lexer_en_error)
		{
			ParseFree(parser, free);
			return false;
		}
		if (p == pe)
		{
			Parse(parser, tkEOL, 0, command);
			break;
		}
	}

	Parse(parser, 0, 0, command);
	ParseFree(parser, free);

	if (!command->valid)
		fprintf(stderr,"I don't understand.\n");

	return command->valid;

}

} // namespace