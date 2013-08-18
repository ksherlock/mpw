#ifndef __debugger_h__
#define __debugger_h__


#include <cstdint>
#include <unordered_map>
#include <functional>
#include <string>

// Debugger is a function in MacTypes.h
namespace Debug {


enum {
	cmdNull,
	cmdContinue,
	cmdStep,
	cmdRun,
};

enum {
	// subtypes
	// ; commands
	kHexdump = 1,
	kInfo,
	kList
};

struct Command {
	bool valid;
	int action;
	uint32_t argc;
	uint32_t argv[10];
};

struct Token {
	// constructor is not allowable because
	// this is a union in the parser.
#if 0
	Token():
	intValue(0), stringValue(0), subtype(0)
	{}

	Token(uint32_t i) : 
	intValue(i), subtype(0)
	{}

	Token(std::string *s, unsigned st  = 0) : 
	intValue(0), stringValue(s), subtype(st)
	{}
#endif

	static Token Make()
	{
		Token t = {0, 0, 0};
		return t;
	}

	static Token Make(uint32_t i)
	{
		Token t = { i, 0, 0};
		return t;
	}

	static Token Make(std::string *s, uint32_t st)
	{
		Token t = { 0, s, st};
		return t;
	}

	uint32_t intValue;
	std::string *stringValue;
	unsigned subtype;


#if 0
	Token& operator=(uint32_t rhs)
	{
		intValue = rhs;
		stringValue = 0;
		subtype = 0;
		return *this;
	}
#endif
	
	operator uint32_t() const
	{
		return intValue;
	}
};



bool ParseLine(const char *iter, Command *command);

std::unordered_map<std::string, uint32_t> LoadTrapFile(const std::string &path);

uint32_t VariableGet(const std::string &);
void VariableSet(const std::string &name, uint32_t value);

void Shell();
void Help();

uint32_t ReadLong(uint32_t);
uint16_t ReadWord(uint32_t);
uint8_t ReadByte(uint32_t);

void Print(uint32_t value);
void PrintRegisters();

void Dump(uint32_t address, int count = 256);
void List(uint32_t address, int count = 20);
void List(uint32_t pc, uint32_t endpc);

void SetARegister(unsigned reg, uint32_t value);
void SetDRegister(unsigned reg, uint32_t value);
void SetXRegister(unsigned reg, uint32_t value);

void ToolBreak(int32_t tool);
void ToolBreak();

void Break(int32_t address);
void Break();

void ReadBreak();
void ReadBreak(int32_t address);

void WriteBreak();
void WriteBreak(int32_t address);

void ReadWriteBreak();
void ReadWriteBreak(int32_t address);

}

#endif