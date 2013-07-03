#ifndef __debugger_commands__
#define __debugger_commands__

enum {
	Help,
	Print,
	Dump,
	List,
	Break,
	TBreak,
	Continue,
	Step,
	SetARegister,
	SetDRegister,
	SetXRegister,
};

struct Command {
	bool valid;
	int action;
	uint32_t argc;
	uint32_t argv[10];
};


#endif