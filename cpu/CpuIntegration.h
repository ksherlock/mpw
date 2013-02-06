#ifndef CpuIntegration_H
#define CpuIntegration_H

typedef enum {
  M68000  = 0,
  M68010  = 1,
  M68020  = 2,
  M68030  = 3,
  M68EC30 = 4,
  M68EC20 = 9
} cpu_integration_models;

extern void cpuIntegrationSetUpInterruptEventHandler(void);
extern void cpuIntegrationExecuteInstructionEventHandler68000Fast(void);
extern void cpuIntegrationExecuteInstructionEventHandler68000General(void);
extern void cpuIntegrationExecuteInstructionEventHandler68020(void);
extern void cpuIntegrationCheckPendingInterrupts(void);
extern ULO cpuIntegrationDisOpcode(ULO disasm_pc, STR *saddress, STR *sdata, STR *sinstruction, STR *soperands);

extern BOOLE cpuIntegrationSetModel(cpu_integration_models model);
extern cpu_integration_models cpuIntegrationGetModel(void);
extern ULO cpuIntegrationGetModelMajor(void);
extern ULO cpuIntegrationGetPC(void);

extern ULO cpuIntegrationGetInstructionTime(void);
extern void cpuIntegrationSetSpeed(ULO speed);
extern ULO cpuIntegrationGetSpeed(void);
extern void cpuIntegrationSetChipCycles(ULO chip_cycles);
extern ULO cpuIntegrationGetChipCycles(void);
extern void cpuIntegrationSetChipSlowdown(ULO chip_slowdown);
extern ULO cpuIntegrationGetChipSlowdown(void);

extern jmp_buf cpu_integration_exception_buffer;

// Fellow limecycle events
extern void cpuIntegrationSaveState(FILE *F);
extern void cpuIntegrationLoadState(FILE *F);
extern void cpuIntegrationEmulationStart(void);
extern void cpuIntegrationEmulationStop(void);
extern void cpuIntegrationHardReset(void);
extern void cpuIntegrationStartup(void);
extern void cpuIntegrationShutdown(void);

#endif
