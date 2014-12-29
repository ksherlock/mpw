#ifndef __debug_template_h__
#define __debug_template_h__

#include <unordered_map>

namespace Debug {



	enum {
		kUnknown = 0,
		kStringPtr, // p-string 
		kCStringPtr, // c-string
		kPtr,
		kOSType, // four-cc
		kBoolean, // unsigned char, display true/false
	};


	struct Template;
	struct FieldEntry;

	struct FieldEntry {
		std::string *name;
		unsigned type;
		unsigned count;
		Template *tmpl;
		FieldEntry *next;
	};

	struct Template {
		unsigned type; // 0 for structs, < 256 for types.

		FieldEntry *firstField;
		unsigned size;
	};

	void CreateTypedef(const std::string *name, int type);
	void CreateTemplate(const std::string *name, FieldEntry *firstField);


	extern std::unordered_map<std::string, Template *> Templates;
	extern int TemplateLine;
}
#endif
