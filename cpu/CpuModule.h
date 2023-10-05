#pragma once

#ifdef __cplusplus
extern "C" {
#endif


// This header file defines the internal interfaces of the CPU module.

// MPW additions.
typedef void (*cpuLineExceptionFunc)(uint16_t);
extern void cpuSetALineExceptionFunc(cpuLineExceptionFunc func);
extern void cpuSetFLineExceptionFunc(cpuLineExceptionFunc func);
typedef void (*memoryLoggingFunc)(uint32_t address, int size, int readWrite, uint32_t value);
extern void memorySetLoggingFunc(memoryLoggingFunc func);


#ifdef _DEBUG
#define CPU_INSTRUCTION_LOGGING
#endif

// Function to check if there are any external interrupt sources wanting to issue interrupts
typedef void (*cpuCheckPendingInterruptsFunc)(void);
extern void cpuSetCheckPendingInterruptsFunc(cpuCheckPendingInterruptsFunc func);
extern void cpuCheckPendingInterrupts(void);
extern void cpuSetUpInterrupt(uint32_t new_interrupt_level);
extern void cpuInitializeFromNewPC(uint32_t new_pc);

// Logging interface
#ifdef CPU_INSTRUCTION_LOGGING

typedef void (*cpuInstructionLoggingFunc)(void);
extern void cpuSetInstructionLoggingFunc(cpuInstructionLoggingFunc func);
typedef void (*cpuExceptionLoggingFunc)(char *description, uint32_t original_pc, uint16_t opcode);
extern void cpuSetExceptionLoggingFunc(cpuExceptionLoggingFunc func);
typedef void (*cpuInterruptLoggingFunc)(uint32_t level, uint32_t vector_address);
extern void cpuSetInterruptLoggingFunc(cpuInterruptLoggingFunc func);

#endif

// CPU register and control properties
extern void cpuSetPC(uint32_t pc);
extern uint32_t cpuGetPC(void);

extern void cpuSetReg(uint32_t da, uint32_t i, uint32_t value);
extern uint32_t cpuGetReg(uint32_t da, uint32_t i);

extern void cpuSetDReg(uint32_t i, uint32_t value);
extern uint32_t cpuGetDReg(uint32_t i);

extern void cpuSetAReg(uint32_t i, uint32_t value);
extern uint32_t cpuGetAReg(uint32_t i);

extern void cpuSetSR(uint32_t sr);
extern uint32_t cpuGetSR(void);

extern void cpuSetUspDirect(uint32_t usp);
extern uint32_t cpuGetUspDirect(void);
extern uint32_t cpuGetUspAutoMap(void);

extern void cpuSetMspDirect(uint32_t msp);
extern uint32_t cpuGetMspDirect(void);

extern void cpuSetSspDirect(uint32_t ssp);
extern uint32_t cpuGetSspDirect(void);
extern uint32_t cpuGetSspAutoMap(void);

extern uint32_t cpuGetVbr(void);

extern void cpuSetStop(BOOLE stop);
extern BOOLE cpuGetStop(void);

extern void cpuSetInitialPC(uint32_t pc);
extern uint32_t cpuGetInitialPC(void);

extern void cpuSetInitialSP(uint32_t sp);
extern uint32_t cpuGetInitialSP(void);

extern uint32_t cpuGetInstructionTime(void);

extern BOOLE cpuSetIrqLevel(uint32_t irq_level);
extern uint32_t cpuGetIrqLevel(void);

extern uint32_t cpuExecuteInstruction(void);
extern uint32_t cpuDisOpcode(uint32_t disasm_pc, char *saddress, char *sdata, char *sinstruction, char *soperands);

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
extern void cpuSetModel(uint32_t major, uint32_t minor);
extern uint32_t cpuGetModelMajor(void);
extern uint32_t cpuGetModelMinor(void);

#ifdef __cplusplus
}
#endif