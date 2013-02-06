#ifndef CpuModule_H
#define CpuModule_H

// This header file defines the internal interfaces of the CPU module.

#define CPU_INSTRUCTION_LOGGING

// Function to check if there are any external interrupt sources wanting to issue interrupts
typedef BOOLE (*cpuCheckPendingInterruptsFunc)(void);
extern void cpuSetCheckPendingInterruptsFunc(cpuCheckPendingInterruptsFunc func);
extern void cpuCheckPendingInterrupts(void);
extern void cpuSetUpInterrupt(void);
extern void cpuInitializeFromNewPC(ULO new_pc);

// Logging interface
#ifdef CPU_INSTRUCTION_LOGGING

typedef void (*cpuInstructionLoggingFunc)(void);
extern void cpuSetInstructionLoggingFunc(cpuInstructionLoggingFunc func);
typedef void (*cpuExceptionLoggingFunc)(STR *description, ULO original_pc, UWO opcode);
extern void cpuSetExceptionLoggingFunc(cpuExceptionLoggingFunc func);
typedef void (*cpuInterruptLoggingFunc)(ULO level, ULO vector_address);
extern void cpuSetInterruptLoggingFunc(cpuInterruptLoggingFunc func);

#endif

// CPU register and control properties
extern void cpuSetPC(ULO pc);
extern ULO cpuGetPC(void);

extern void cpuSetReg(ULO da, ULO i, ULO value);
extern ULO cpuGetReg(ULO da, ULO i);

extern void cpuSetDReg(ULO i, ULO value);
extern ULO cpuGetDReg(ULO i);

extern void cpuSetAReg(ULO i, ULO value);
extern ULO cpuGetAReg(ULO i);

extern void cpuSetSR(ULO sr);
extern ULO cpuGetSR(void);

extern void cpuSetUspDirect(ULO usp);
extern ULO cpuGetUspDirect(void);
extern ULO cpuGetUspAutoMap(void);

extern void cpuSetMspDirect(ULO msp);
extern ULO cpuGetMspDirect(void);

extern void cpuSetSspDirect(ULO ssp);
extern ULO cpuGetSspDirect(void);
extern ULO cpuGetSspAutoMap(void);

extern ULO cpuGetVbr(void);

extern void cpuSetStop(BOOLE stop);
extern BOOLE cpuGetStop(void);

extern void cpuSetInitialPC(ULO pc);
extern ULO cpuGetInitialPC(void);

extern void cpuSetInitialSP(ULO sp);
extern ULO cpuGetInitialSP(void);

extern ULO cpuGetInstructionTime(void);

extern void cpuSetIrqLevel(ULO irq_level);
extern ULO cpuGetIrqLevel(void);

extern void cpuSetIrqAddress(ULO irq_address);
extern ULO cpuGetIrqAddress(void);

extern ULO cpuExecuteInstruction(void);
extern ULO cpuDisOpcode(ULO disasm_pc, STR *saddress, STR *sdata, STR *sinstruction, STR *soperands);

extern void cpuSaveState(FILE *F);
extern void cpuLoadState(FILE *F);
extern void cpuHardReset(void);
extern void cpuStartup(void);

typedef void (*cpuMidInstructionExceptionFunc)(void);
extern void cpuSetMidInstructionExceptionFunc(cpuMidInstructionExceptionFunc func);
extern void cpuThrowAddressErrorException(void);

typedef void (*cpuResetExceptionFunc)(void);
extern void cpuSetResetExceptionFunc(cpuResetExceptionFunc func);

// Configuration settings
extern void cpuSetModel(ULO major, ULO minor);
extern ULO cpuGetModelMajor(void);
extern ULO cpuGetModelMinor(void);

#endif