#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  M68000  = 0,
  M68010  = 1,
  M68020  = 2,
  M68030  = 3,
  M68EC30 = 4,
  M68EC20 = 9
} cpu_integration_models;

extern void cpuIntegrationCalculateMultiplier(void);

extern void cpuIntegrationExecuteInstructionEventHandler68000Fast(void);
extern void cpuIntegrationExecuteInstructionEventHandler68000General(void);
extern void cpuIntegrationExecuteInstructionEventHandler68020(void);
extern uint32_t cpuIntegrationDisOpcode(uint32_t disasm_pc, char *saddress, char *sdata, char *sinstruction, char *soperands);

extern BOOLE cpuIntegrationSetModel(cpu_integration_models model);
extern cpu_integration_models cpuIntegrationGetModel(void);
extern uint32_t cpuIntegrationGetModelMajor(void);

void cpuIntegrationSetIrqLevel(uint32_t new_interrupt_level, uint32_t chip_interrupt_number);
extern uint32_t cpuIntegrationGetInstructionTime(void);
extern void cpuIntegrationSetSpeed(uint32_t speed);
extern uint32_t cpuIntegrationGetSpeed(void);
extern void cpuIntegrationSetChipCycles(uint32_t chip_cycles);
extern uint32_t cpuIntegrationGetChipCycles(void);
extern void cpuIntegrationSetChipSlowdown(uint32_t chip_slowdown);
extern uint32_t cpuIntegrationGetChipSlowdown(void);

extern jmp_buf cpu_integration_exception_buffer;

// Fellow limecycle events
extern void cpuIntegrationSaveState(FILE *F);
extern void cpuIntegrationLoadState(FILE *F);
extern void cpuIntegrationEmulationStart(void);
extern void cpuIntegrationEmulationStop(void);
extern void cpuIntegrationHardReset(void);
extern void cpuIntegrationStartup(void);
extern void cpuIntegrationShutdown(void);

#ifdef __cplusplus
}
#endif
