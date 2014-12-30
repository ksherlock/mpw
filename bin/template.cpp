#include "template.h"
#include "debugger.h"
#include "debugger_internal.h"

#include <toolbox/toolbox.h>


namespace Debug {

	using namespace Debug::Internal;

	namespace {


		unsigned CalcOneSize(FieldEntry *e)
		{
			unsigned s = (e->type & 0x0f00) >> 8;

			if (!s) {
				// struct or pointer...
				if (e->type & 0x8000) s = 4;
				else if (e->tmpl) s = e->tmpl->struct_size;
			}

			if (e->count) s *= e->count;

			return s;
		}

		unsigned CalcSize(FieldEntry *e)
		{
			unsigned size = 0;

			while (e)
			{
				size += CalcOneSize(e);
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


	}

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


	void PrettyPrint(uint32_t value, unsigned type)
	{

		switch (type)
		{
			case kOSType:
				// print 4-cc code
				fputc(' ', stdout);
				fputs(ToolBox::TypeToString(value).c_str(), stdout);
				return;

			case kOSErr:
				// print value + short name
				{
					printf(" %6d", (int16_t)value);
					auto iter = ErrorTableInvert.find(value);
					if (iter != ErrorTableInvert.end()) printf(" %s", iter->second.c_str());
				}
				return;

			case kPStringPtr:
				// read the string...
				return;

			case kCStringPtr:
				// read the string...
				return;

			case kBoolean:
				fputc(' ', stdout);
				fputs(value ? "true" : "false", stdout);
				return;

			case kHandle:
				// print address, size, locked stats.
				return;

		}

		// int/signed int - print base 10.

	}


	void ApplyTemplate(uint32_t address, FieldEntry *e, unsigned indent)
	{
		unsigned offset = 0;

		if (!e) return;

		for( ; e ; e = e->next)
		{

			bool nonl = false;
			unsigned count = e->count;
			unsigned type = e->type;
			unsigned s = (type & 0x0f00) >> 8;

			for (unsigned i = 0; i < indent; ++i) fputc('>',stdout);
			fputs(e->name->c_str(),stdout);
			for(unsigned i = indent + e->name->length(); i < 40; ++i) fputc(' ',stdout);

			// todo -- support arrays
			// todo -- pretty print values (boolean, oserr, ostype, etc.)
			switch(s)
			{
				case 1:
					{
						uint8_t value = ReadByte(address + offset);
						printf("      %02x", value);
						PrettyPrint(value, type);
						break; 
					}

				case 2:
					{
						uint16_t value = ReadWord(address + offset);
						printf("    %04x", value);
						PrettyPrint(value, type);
						break; 
					}

				case 4:
					{
						uint32_t value = ReadLong(address + offset);
						printf("%08x", value);
						PrettyPrint(value, type);
						break; 
					}

				case 0:
					// either a pointer or a struct
					if (type & 0x8000) { 
						// pointer.
						uint32_t value = ReadLong(address + offset);
						printf("%08x", value);
						PrettyPrint(value, type);
						break;
					}
					if (type == 0) {
						// struct ... recurse.
						fputc('\n', stdout);
						nonl = true;
						ApplyTemplate(address + offset, e->tmpl, indent + 1);
						break;
					}
			}

			if (!nonl) fputc('\n', stdout);
			offset += CalcOneSize(e);

		}


	}

}
