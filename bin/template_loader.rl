
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
#include "intern.h"

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

void *TemplateParseAlloc(void *(*mallocProc)(size_t));
void TemplateParseFree(void *p, void (*freeProc)(void*));

void TemplateParse(void *yyp, int yymajor, void *yyminor, Debug::TemplateParseInfo *);


void TemplateParse(void *yyp, int yymajor, int yyminor, Debug::TemplateParseInfo *info)
{
	TemplateParse(yyp, yymajor, &yyminor, info);
}

void TemplateParse(void *yyp, int yymajor, const std::string *yyminor, Debug::TemplateParseInfo *info)
{
	TemplateParse(yyp, yymajor, (void *)yyminor, info);
}

#define TemplateParse(a,b,c) TemplateParse(a,b,c, &info)

%%{
	machine lexer;

	# this exits with cs == lexer_en_error.
	error := any* ${ fbreak; };

	block_comment := |*
		[\n\r] { info.LineNumber++; };
		'*/' { fgoto main; };
		any ;
	*|; 

	main := |*

		[\n\r] { info.LineNumber++; };
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

		'int64_t' { TemplateParse(parser, tkTYPECODE, kSInt64); };
		'uint64_t' { TemplateParse(parser, tkTYPECODE, kUInt64); };

		'int32_t' { TemplateParse(parser, tkTYPECODE, kSInt32); };
		'uint32_t' { TemplateParse(parser, tkTYPECODE, kUInt32); };

		'int16_t' { TemplateParse(parser, tkTYPECODE, kSInt16); };
		'uint16_t' { TemplateParse(parser, tkTYPECODE, kUInt16); };

		'int8_t' { TemplateParse(parser, tkTYPECODE, kSInt8); };
		'uint8_t' { TemplateParse(parser, tkTYPECODE, kUInt8); };


		'StringPtr' { TemplateParse(parser, tkTYPECODE, kPStringPtr); };
		'CStringPtr' { TemplateParse(parser, tkTYPECODE, kCStringPtr); };
		'Ptr' { TemplateParse(parser, tkTYPECODE, kVoidPtr); };
		'OSType' { TemplateParse(parser, tkTYPECODE, kOSType); };
		'OSErr' { TemplateParse(parser, tkTYPECODE, kOSErr); };
		'Boolean' { TemplateParse(parser, tkTYPECODE, kBoolean); };
		'Handle' { TemplateParse(parser, tkTYPECODE, kHandle); };


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

			const std::string *name = Intern::String(ts, te);
			bool ok = false;

			if (!ok) {
				auto iter = Types.find(*name);
				if (iter != Types.end())
				{
					TemplateParse(parser, tkTYPECODE, iter->second);
					ok = true;
				}
			}
			if (!ok) {
				auto iter = Templates.find(*name);
				if (iter != Templates.end())
				{
					TemplateParse(parser, tkTEMPLATE, iter->second);
					ok = true;
				}
			}
			if (!ok)
			{
				TemplateParse(parser, tkIDENTIFIER, name);
			}
		};

	*|;

}%%

namespace Debug {


void CreateTypedef(const std::string *name, int type, TemplateParseInfo *info)
{
	// check if it's an existing typedef...

	auto &Templates = *info->templates;
	auto &Types = *info->types;


	auto iter = Types.find(*name);
	if (iter != Types.end())
	{
		if (iter->second == type) return; // ok, just a duplicate.
		fprintf(stderr, "Template Error: line %d - redefining %s\n", 
			info->LineNumber, name->c_str());

		return;
	}

	if (Templates.find(*name) != Templates.end())
	{
		fprintf(stderr, "Template Error: line %d - redefining %s\n", 
			info->LineNumber, name->c_str());

		return;	
	}

	Types.emplace(std::make_pair(*name, type)); 
}

unsigned CalcSize(FieldEntry *e)
{
	unsigned size = 0;

	while (e)
	{
		unsigned s = (e->type & 0x0f00) >> 8;

		if (!s) {
			// struct or pointer...
			if (e->type & 0x8000) s = 4;
			else if (e->tmpl) s = e->tmpl->struct_size;
		}

		if (e->count != 0) s *= e->count;

		size += s;
		e = e->next;
	}
	return size;
}

FieldEntry *Reverse(FieldEntry *e)
{
	if (!e) return e;

	// reverse the order...
	FieldEntry *prev;
	FieldEntry *next;

	prev = nullptr;
	for(;;)
	{
		next = e->next;
		e->next = prev;

		prev = e;
		e = next;
		if (!e) return prev;
	}
}


void CreateTemplate(const std::string *name, FieldEntry *firstField, TemplateParseInfo *info)
{
	auto &Templates = *info->templates;
	auto &Types = *info->types;

	// check if it exists...

	if (Templates.find(*name) != Templates.end())
	{
		fprintf(stderr, "Template Error: line %d - redefining %s\n", 
			info->LineNumber, name->c_str());
		return;	
	}

	if (Types.find(*name) != Types.end())
	{
		fprintf(stderr, "Template Error: line %d - redefining %s\n", 
			info->LineNumber, name->c_str());
		return;	
	}

	firstField = Reverse(firstField);
	firstField->struct_size = CalcSize(firstField);

	Templates.emplace(std::make_pair(*name, firstField));
}


bool LoadTemplateFile(const std::string &filename, std::unordered_map<std::string, Template> &Templates)
{
	%% write data;

	void *parser;

	int fd;
	struct stat st;
	char *buffer;

	std::unordered_map<std::string, unsigned> Types;
	TemplateParseInfo info;

	info.LineNumber = 1;
	info.templates = &Templates;
	info.types = &Types;

	// simple types are handled via the lexer so no need to populate them here.



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
			// ?
			//TemplateParse(parser, tkEOF, 0);
			break;
		}
	}

	TemplateParse(parser, 0, 0);
	TemplateParseFree(parser, free);

	munmap(buffer, st.st_size);

	return true;
}

}