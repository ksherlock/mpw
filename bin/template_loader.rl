
#include <string>
#include <unordered_map>
#include <algorithm>
#include <numeric>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "template_parser.h"
#include "template.h"

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



	unsigned int DJBHash(const char* begin, const char *end)
	{
		unsigned int hash = 5381;
		unsigned int i    = 0;

		for(const char *iter = begin; iter != end; ++iter)
		{   
			hash = ((hash << 5) + hash) + (*iter);
		}   

		return hash;
	}

	std::unordered_multimap<unsigned, std::string *> InternTable;

	const std::string *InternString(const char *begin, const char *end)
	{
		bool found = false;
		unsigned hash = DJBHash(begin, end);
		size_t length = end - begin;

		auto range = InternTable.equal_range(hash);
		auto iter = range.first;
		auto endit = range.second;

		for( ; iter != endit; ++iter)
		{
			// hash matches, make sure the string does.
			const std::string *s = iter->second;

			if (s->length() == length && std::memcmp(s->data(), begin, length) == 0)
				return s;
		}

		// insert it.
		std::string *s = new std::string(begin, end);
		InternTable.emplace(std::make_pair(hash, s));
		return s;
	}

}

void TemplateParse(void *yyp, int yymajor, void *yyminor);
void *TemplateParseAlloc(void *(*mallocProc)(size_t));
void TemplateParseFree(void *p, void (*freeProc)(void*));

void TemplateParse(void *yyp, int yymajor, int yyminor)
{
	TemplateParse(yyp, yymajor, &yyminor);
}

void TemplateParse(void *yyp, int yymajor, const std::string *yyminor)
{
	TemplateParse(yyp, yymajor, (void *)yyminor);
}

%%{
	machine lexer;

	# this exits with cs == lexer_en_error.
	error := any* ${ fbreak; };

	block_comment := |*
		[\n\r] { TemplateLine++; };
		'*/' { fgoto main; };
		any ;
	*|; 

	main := |*

		[\n\r] { TemplateLine++; };
		[ \t]+;

		'//' [^\r\n]* ;
		'/*' { fgoto block_comment; };

		';' { TemplateParse(parser, tkSEMI, 0); };
		'{' { TemplateParse(parser, tkLBRACE, 0); };
		'}' { TemplateParse(parser, tkRBRACE, 0); };
		'[' { TemplateParse(parser, tkLBRACKET, 0); };
		']' { TemplateParse(parser, tkRBRACKET, 0); };
		'*' { TemplateParse(parser, tkSTAR, 0); };

		'struct' { TemplateParse(parser, tkSTRUCT, 0); };
		'typedef' { TemplateParse(parser, tkTYPEDEF, 0); };


		'int' { TemplateParse(parser, tkINT, 0); };
		'long' { TemplateParse(parser, tkLONG, 0); };
		'short' { TemplateParse(parser, tkSHORT, 0); };
		'volatile' { TemplateParse(parser, tkVOLATILE, 0); };
		#'const' { TemplateParse(parser, tkCONST, 0); };
		'char' { TemplateParse(parser, tkCHAR, 0); };
		#'bool' { TemplateParse(parser, tkBOOL, 0); };
		'void' { TemplateParse(parser, tkVOID, 0); };

		'signed' { TemplateParse(parser, tkSIGNED, 0); };
		'unsigned' { TemplateParse(parser, tkUNSIGNED, 0); };

		'int64_t' { TemplateParse(parser, tkTYPECODE, 'q'); };
		'uint64_t' { TemplateParse(parser, tkTYPECODE, 'Q'); };

		'int32_t' { TemplateParse(parser, tkTYPECODE, 'l'); };
		'uint32_t' { TemplateParse(parser, tkTYPECODE, 'L'); };

		'int16_t' { TemplateParse(parser, tkTYPECODE, 's'); };
		'uint16_t' { TemplateParse(parser, tkTYPECODE, 'S'); };

		'int8_t' { TemplateParse(parser, tkTYPECODE, 'c'); };
		'uint8_t' { TemplateParse(parser, tkTYPECODE, 'C'); };


		'StringPtr' { TemplateParse(parser, tkTYPECODE, kStringPtr); };
		'CStringPtr' { TemplateParse(parser, tkTYPECODE, kCStringPtr); };
		'Ptr' { TemplateParse(parser, tkTYPECODE, kPtr); };
		'OSType' { TemplateParse(parser, tkTYPECODE, kOSType); };
		'Boolean' { TemplateParse(parser, tkTYPECODE, kBoolean); };


		# numbers.  negative numbers are not allowed.

		'0x'i xdigit+ {
			// hexadecimal
			uint32_t value = scan16(ts + 2, te);
			TemplateParse(parser, tkINTEGER, value);
		};

		digit+ {
			uint32_t value = scan10(ts, te);
			TemplateParse(parser, tkINTEGER, value);
		};

		# identifier ... but also need to check if it's a type.
		[A-Za-z_][A-Za-z0-9_]+ {

			// intern the string.

			const std::string *name = InternString(ts, te);

			auto iter = Templates.find(*name);
			if (iter != Templates.end())
			{
				unsigned type = iter->second->type;
				if (type) TemplateParse(parser, tkTYPECODE, type);
				else TemplateParse(parser, tkTEMPLATE, iter->second);
			}
			else
			{
				TemplateParse(parser, tkIDENTIFIER, name);
			}
		};

	*|;

}%%

namespace Debug {

std::unordered_map<std::string, Template *> Templates;

void CreateTypedef(const std::string *name, int type)
{
}
void CreateTemplate(const std::string *name, FieldEntry *firstField)
{
}

int TemplateLine;

bool ParseTemplates(const std::string &filename)
{
	%% write data;

	void *parser;

	int fd;
	struct stat st;
	char *buffer;

	if (stat(filename.c_str(), &st) < 0) return false;
	if (st.st_size == 0) return false;

	fd = open(filename.c_str(), O_RDONLY);
	if (fd < 0) {
		perror("Error opening template file: ");
		return false;
	}

	buffer = (char *)mmap(nullptr, st.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
	if (buffer == MAP_FAILED) {
		perror("Error mapping template file: ");
		close(fd);
		return false;
	}
	close(fd);




	parser = TemplateParseAlloc(malloc);


	TemplateLine = 1;

	const char *p = buffer;
	const char *pe = buffer + st.st_size;
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
			TemplateParseFree(parser, free);
			munmap(buffer, st.st_size);
			return false;
		}
		if (cs == lexer_en_error)
		{
			TemplateParseFree(parser, free);
			munmap(buffer, st.st_size);
			return false;
		}
		if (p == pe)
		{
			TemplateParse(parser, tkEOF, 0);
			break;
		}
	}

	TemplateParse(parser, 0, 0);
	TemplateParseFree(parser, free);

	munmap(buffer, st.st_size);

	return true;
}

}