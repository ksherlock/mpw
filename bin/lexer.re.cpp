
#include <string>
#include <algorithm>
#include <numeric>

#include <cstdint>
#include <stdlib.h>

#include "debugger.h"
#include "parser.h"

// re2c -b -i


//extern "C" {

	void *ParseAlloc(void *(*mallocProc)(size_t));
	void ParseFree(void *p, void (*freeProc)(void*));
	void Parse(void *yyp, int yymajor, uint32_t yyminor, Debug::Command *command);
	void ParseTrace(FILE *TraceFILE, char *zTracePrompt);
//}

	// p / print expression
	// hd / hexdump expression [:expression]
	// stack ?
	// brk expression
	// tbrk expression 
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
}



namespace Debug {
bool ParseLine(const char *iter, Command *command)
{
	void *parser;

	parser = ParseAlloc(malloc);

	//ParseTrace(stdout, "--> ");
	command->action = cmdNull;
	for (;;)
	{
		const char *begin = iter;
		const char *marker;

		/*!re2c
		   re2c:define:YYCURSOR = iter;
		   re2c:define:YYMARKER = marker;
		   re2c:define:YYCTYPE = char;
		   re2c:yyfill:enable = 0;

		[ \t\f\r\n]+ {
			// white space
			continue;
		}

		'>=' { Parse(parser, tkGTEQ, 0, command); continue; }
		'>>' { Parse(parser, tkGTGT, 0, command); continue; }
		'<=' { Parse(parser, tkLTEQ, 0, command); continue; }
		'<<' { Parse(parser, tkLTLT, 0, command); continue; }
		'!=' { Parse(parser, tkBANGEQ, 0, command); continue; }
		'==' { Parse(parser, tkEQEQ, 0, command); continue; }
		'||' { Parse(parser, tkPIPEPIPE, 0, command); continue; }
		'&&' { Parse(parser, tkAMPAMP, 0, command); continue; }


		'(' { Parse(parser, tkLPAREN, 0, command); continue; }
		')' { Parse(parser, tkRPAREN, 0, command); continue; }
		'=' { Parse(parser, tkEQ, 0, command); continue; }
		'+' { Parse(parser, tkPLUS, 0, command); continue; }
		'-' { Parse(parser, tkMINUS, 0, command); continue; }
		'*' { Parse(parser, tkSTAR, 0, command); continue; }
		'/' { Parse(parser, tkSLASH, 0, command); continue; }
		'%' { Parse(parser, tkPERCENT, 0, command); continue; }
		'~' { Parse(parser, tkTILDE, 0, command); continue; }
		'!' { Parse(parser, tkBANG, 0, command); continue; }
		'^' { Parse(parser, tkCARET, 0, command); continue; }
		'&' { Parse(parser, tkAMP, 0, command); continue; }
		'|' { Parse(parser, tkPIPE, 0, command); continue; }
		'<' { Parse(parser, tkLT, 0, command); continue; }
		'>' { Parse(parser, tkGT, 0, command); continue; }

		':' { Parse(parser, tkCOLON, 0, command); continue; }
		'@' { Parse(parser, tkAT, 0, command); continue; }

		[0-9]+ {
			// integer
			uint32_t data;

			data = std::accumulate(begin, iter, 0,
				[](uint32_t value, char c){
					return (value * 10) + c - '0';
			});

			Parse(parser, tkINTEGER, data, command);
			continue;
		}

		'$' [0-9A-Fa-f]+ {
			// hex number
			uint32_t data;

			data = std::accumulate(begin + 1, iter, 0,
				[](uint32_t value, char c){
					return (value << 4) + tox(c);
				}
			);
			Parse(parser, tkINTEGER, data, command);
			continue;
		}

		'0x' [0-9A-Fa-f]+ {
			// hex number
			uint32_t data;

			data = std::accumulate(begin + 2, iter, 0,
				[](uint32_t value, char c){
					return (value << 4) + tox(c);
				}
			);
			Parse(parser, tkINTEGER, data, command);
			continue;
		}

		['] [^']{1,4} ['] {
			// 4 cc code
			uint32_t data;

			data = std::accumulate(begin + 1, iter - 1, 0,
				[](uint32_t value, char c)
				{
					return (value << 8) + (unsigned)c;
				}
			);
			Parse(parser, tkINTEGER, data, command);
			continue;
		}


		'd' [0-7] {
			// data register
			uint32_t data = begin[1] - '0';
			Parse(parser, tkDREGISTER, data, command);
			continue;
		}

		'a' [0-7] {
			// address register
			uint32_t data = begin[1] - '0';
			Parse(parser, tkAREGISTER, data, command);
			continue;
		}

		'pc' {
			// program counter...
			Parse(parser, tkXREGISTER, 0, command);
			continue;
		}

		'csr' {
			// condition status register.
			Parse(parser, tkXREGISTER, 1, command);
			continue;
		}

		'sp' {
			// stack pointer aka a7
			Parse(parser, tkAREGISTER, 7, command);
			continue;
		}

		'fp' {
			// frame pointer aka a6
			Parse(parser, tkAREGISTER, 6, command);
			continue;
		}


		'c' | 'continue' {
			Parse(parser, tkCONTINUE, 0, command);
			continue;
		}

		'hd' | 'dump' {
			Parse(parser, tkDUMP, 0, command);
			continue;
		}

		'h' | 'help' {
			Parse(parser, tkHELP, 0, command);
			continue;
		}

		'l' | 'list' {
			Parse(parser, tkLIST, 0, command);
			continue;
		}

		'n' | 'next' {
			Parse(parser, tkNEXT, 0, command);
			continue;
		}

		's' | 'step' {
			Parse(parser, tkNEXT, 0, command);
			continue;
		}

		'b' | 'brk' | 'break' {
			Parse(parser, tkBREAK, 0, command);
			continue;
		}

		'tbrk' | 'tbreak' | 'toolbreak' {
			Parse(parser, tkTBREAK, 0, command);
			continue;	
		}

		'mbrk' | 'mbreak' | 'rwbrk' | 'rwbreak' {
			Parse(parser, tkRWBREAK, 0, command);
			continue;
		}

		'rbrk' | 'rbreak' {
			Parse(parser, tkRBREAK, 0, command);
			continue;
		}

		'wbrk' | 'wbreak' {
			Parse(parser, tkWBREAK, 0, command);
			continue;
		}


		'g' | 'go' {
			Parse(parser, tkCONTINUE, 0, command);
			continue;
		}

		'p' | 'print' {
			Parse(parser, tkPRINT, 0, command);
			continue;
		}
	
		'r' | 'run' {
			Parse(parser, tkCONTINUE, 0, command);
			continue;
		}


		';h' | ';hd' | ';hexdump' {
			Parse(parser, tkSEMIH, 0, command);
			continue;
		}

		';i' | ';info' {
			Parse(parser, tkSEMII, 0, command);
			continue;
		}

		';l' | ';list' {
			Parse(parser, tkSEMIL, 0, command);
			continue;
		}



		[_A-Za-z][_A-Za-z0-9] + {
			// identifier. lookup global address, tool number, etc.
			fprintf(stderr, "illegal identifier: `%.*s`\n", (int)(iter - begin), begin);
			return false;
		}


		[\x00] {
			// eol.
			Parse(parser, tkEOL, 0, command);
			break;
		}

		[^] {
			fprintf(stderr, "illegal character: `%c`\n", *begin);
			ParseFree(parser, free);
			return false;
		}

		*/

	}

	Parse(parser, 0, 0, command);
	ParseFree(parser, free);

	if (!command->valid)
		fprintf(stderr,"I don't understand.\n");

	return command->valid;
}



} // namespace Debugger

