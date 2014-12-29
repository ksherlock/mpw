#ifndef __debug_template_h__
#define __debug_template_h__

#include <unordered_map>
#include <string>

namespace Debug {


	enum {

		// 0x8000 = pointer
		// 0x1000 = unsigned
		// 0x0f00 = size
		// 0-255 - type

		kSInt64 = 0x0800,
		kUInt64 = 0x1800,

		kSInt32 = 0x0400,
		kUInt32 = 0x1400,

		kSInt16 = 0x0200,
		kUInt16 = 0x1200,

		kSInt8 = 0x0100,
		kUInt8 = 0x1100,

		kStruct = 0x0000,
		kStructPtr = 0x8000,

		// ptrs are a special case where size = 0
		//kVoid = 0x0001,
		kVoidPtr = 0x8001,

		// these exist for display purposes.
		kCStringPtr = 0x8002,
		kPStringPtr = 0x8003,

		kOSType = 0x00404,
		kBoolean = 0x0105,
		kOSErr = 0x0206,
		kHandle = 0x04007,

	};

	inline unsigned MakePtr(unsigned type) { return type | 0x8000; }
	inline unsigned MakeType(unsigned size, bool sign, bool ptr) {
		unsigned rv = size << 8;
		if (!sign) rv |= 0x0100;
		if (ptr) rv |= 0x8000;
		return rv;
	}

	enum {
		kDisplayNative = 0,
		kDisplayStringPtr, // p-string 
		kDisplayCStringPtr, // c-string
		kDisplayOSType, // four-cc
		kDisplayBoolean, // unsigned char, display true/false
		kDisplayOSErr,
	};


#if 0

	struct Type {
		enum {
			kSimpleType,
			kStructType,
			kPointerType,
		};
		uint16_t tag;
		uint16_t size; 
	};

	// handles [signed/unsigned] type, type[], *type, and *type[]
	struct SimpleType : public Type {
		unsigned rank:16; // int == int[1]
		unsigned display:14; // 

		unsigned sign:1;
		unsigned pointer:1;

		SimpleType() {
			tag = kSimpleType;
			size = 0;
			rank = 0;
			display = 0;
			sign = 0;
			pointer = 0;
		}
	};

	struct PointerType: public Type {
		Type *type;
	};

	struct StructType : public Type {
		unsigned total_size;
		FieldEntry *firstField;
	};

#endif

	struct FieldEntry;
	typedef FieldEntry *Template;

	struct FieldEntry {
		std::string *name;
		unsigned type;
		unsigned count;
		Template tmpl;
		FieldEntry *next;
		unsigned struct_size; // only populated for head entry.
	};

	struct TemplateParseInfo {
		std::unordered_map<std::string, Template> *templates;
		std::unordered_map<std::string, unsigned> *types;
		int LineNumber;
	};


	void CreateTypedef(const std::string *name, int type, TemplateParseInfo *);
	void CreateTemplate(const std::string *name, FieldEntry *firstField, TemplateParseInfo *);


	bool LoadTemplateFile(const std::string &filename, std::unordered_map<std::string, Template> &);


	void ApplyTemplate(uint32_t address, FieldEntry *e, unsigned indent = 0);



}
#endif
