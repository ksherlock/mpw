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

struct Command {
	bool valid;
	int action;
	uint32_t argc;
	uint32_t argv[10];
};


bool ParseLine(const char *iter, Command *command);

std::unordered_map<std::string, uint32_t> LoadTrapFile(const std::string &path);

uint32_t EnvLookup(const std::string &);


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