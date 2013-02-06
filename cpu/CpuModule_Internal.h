#ifndef CpuModule_Internal_H
#define CpuModule_Internal_H

// This header file defines the internal interfaces of the CPU module.
extern void cpuMakeOpcodeTableForModel(void);
extern void cpuCreateMulTimeTables(void);

// StackFrameGen
extern void cpuStackFrameGenerate(UWO vector_no, ULO pc);
extern void cpuStackFrameInit(void);

// Registers
extern ULO cpu_sr;  // Not static because the flags calculation uses it extensively
extern BOOLE cpuGetFlagSupervisor(void);
extern BOOLE cpuGetFlagMaster(void);
extern void cpuSetUspDirect(ULO usp);
extern ULO cpuGetUspDirect(void);
extern ULO cpuGetUspAutoMap(void);
extern void cpuSetSspDirect(ULO ssp);
extern ULO cpuGetSspDirect(void);
extern ULO cpuGetSspAutoMap(void);
extern void cpuSetMspDirect(ULO msp);
extern ULO cpuGetMspDirect(void);
extern ULO cpuGetMspAutoMap(void);
extern void cpuSetMspAutoMap(ULO new_msp);
extern ULO cpuGetIspAutoMap(void);
extern void cpuSetIspAutoMap(ULO new_isp);
extern void cpuSetDReg(ULO i, ULO value);
extern ULO cpuGetDReg(ULO i);
extern void cpuSetAReg(ULO i, ULO value);
extern ULO cpuGetAReg(ULO i);
extern void cpuSetReg(ULO da, ULO i, ULO value);
extern ULO cpuGetReg(ULO da, ULO i);
extern void cpuSetPC(ULO address);
extern ULO cpuGetPC(void);
extern void cpuSetStop(BOOLE stop);
extern BOOLE cpuGetStop(void);
extern void cpuSetVbr(ULO vbr);
extern ULO cpuGetVbr(void);
extern void cpuSetSfc(ULO sfc);
extern ULO cpuGetSfc(void);
extern void cpuSetDfc(ULO dfc);
extern ULO cpuGetDfc(void);
extern void cpuSetCacr(ULO cacr);
extern ULO cpuGetCacr(void);
extern void cpuSetCaar(ULO caar);
extern ULO cpuGetCaar(void);
extern void cpuSetSR(ULO sr);
extern ULO cpuGetSR(void);
extern void cpuSetIrqLevel(ULO irq_level);
extern ULO cpuGetIrqLevel(void);
extern void cpuSetIrqAddress(ULO irq_address);
extern ULO cpuGetIrqAddress(void);
extern void cpuSetInstructionTime(ULO cycles);
extern ULO cpuGetInstructionTime(void);
extern void cpuSetOriginalPC(ULO pc);
extern ULO cpuGetOriginalPC(void);

#ifdef CPU_INSTRUCTION_LOGGING

extern void cpuSetCurrentOpcode(UWO opcode);
extern UWO cpuGetCurrentOpcode(void);

#endif

extern void cpuProfileWrite(void);

extern void cpuSetModelMask(UBY model_mask);
extern UBY cpuGetModelMask(void);
extern void cpuSetDRegWord(ULO regno, UWO val);
extern void cpuSetDRegByte(ULO regno, UBY val);
extern UWO cpuGetRegWord(ULO i, ULO regno);
extern UWO cpuGetDRegWord(ULO regno);
extern UBY cpuGetDRegByte(ULO regno);
extern ULO cpuGetDRegWordSignExtLong(ULO regno);
extern UWO cpuGetDRegByteSignExtWord(ULO regno);
extern ULO cpuGetDRegByteSignExtLong(ULO regno);
extern UWO cpuGetARegWord(ULO regno);
extern UBY cpuGetARegByte(ULO regno);

extern UWO cpuGetNextWord(void);
extern ULO cpuGetNextWordSignExt(void);
extern ULO cpuGetNextLong(void);
extern void cpuSkipNextWord(void);
extern void cpuSkipNextLong(void);
extern void cpuClearPrefetch(void);
extern void cpuValidateReadPointer(void);

extern void cpuInitializeFromNewPC(ULO new_pc);

// Effective address
extern ULO cpuEA02(ULO regno);
extern ULO cpuEA03(ULO regno, ULO size);
extern ULO cpuEA04(ULO regno, ULO size);
extern ULO cpuEA05(ULO regno);
extern ULO cpuEA06(ULO regno);
extern ULO cpuEA70(void);
extern ULO cpuEA71(void);
extern ULO cpuEA72(void);
extern ULO cpuEA73(void);

// Flags
extern void cpuSetFlagsAdd(BOOLE z, BOOLE rm, BOOLE dm, BOOLE sm);
extern void cpuSetFlagsSub(BOOLE z, BOOLE rm, BOOLE dm, BOOLE sm);
extern void cpuSetFlagsCmp(BOOLE z, BOOLE rm, BOOLE dm, BOOLE sm);
extern void cpuSetZFlagBitOpsB(UBY res);
extern void cpuSetZFlagBitOpsL(ULO res);

extern void cpuSetFlagsNZ00NewB(UBY res);
extern void cpuSetFlagsNZ00NewW(UWO res);
extern void cpuSetFlagsNZ00NewL(ULO res);
extern void cpuSetFlagsNZ00New64(LLO res);

extern void cpuSetFlagZ(BOOLE f);
extern void cpuSetFlagN(BOOLE f);
extern void cpuSetFlagV(BOOLE f);
extern void cpuSetFlagC(BOOLE f);
extern void cpuSetFlagXC(BOOLE f);
extern void cpuSetFlags0100(void);
extern void cpuSetFlagsNeg(BOOLE z, BOOLE rm, BOOLE dm);
extern BOOLE cpuGetFlagX(void);
extern void cpuSetFlagsNegx(BOOLE z, BOOLE rm, BOOLE dm);
extern BOOLE cpuGetFlagV(void);
extern void cpuSetFlagsNZVC(BOOLE z, BOOLE n, BOOLE v, BOOLE c);
extern void cpuSetFlagsVC(BOOLE v, BOOLE c);
extern void cpuSetFlagsShiftZero(BOOLE z, BOOLE rm);
extern void cpuSetFlagsShift(BOOLE z, BOOLE rm, BOOLE c, BOOLE v);
extern void cpuSetFlagsRotate(BOOLE z, BOOLE rm, BOOLE c);
extern void cpuSetFlagsRotateX(UWO z, UWO rm, UWO x);
extern void cpuSetFlagsAddX(BOOLE z, BOOLE rm, BOOLE dm, BOOLE sm);
extern void cpuSetFlagsSubX(BOOLE z, BOOLE rm, BOOLE dm, BOOLE sm);
extern void cpuSetFlagsAbs(UWO f);
extern UWO cpuGetZFlagB(UBY res);
extern UWO cpuGetZFlagW(UWO res);
extern UWO cpuGetZFlagL(ULO res);
extern UWO cpuGetNFlagB(UBY res);
extern UWO cpuGetNFlagW(UWO res);
extern UWO cpuGetNFlagL(ULO res);

extern BOOLE cpuCalculateConditionCode0(void);
extern BOOLE cpuCalculateConditionCode1(void);
extern BOOLE cpuCalculateConditionCode2(void);
extern BOOLE cpuCalculateConditionCode3(void);
extern BOOLE cpuCalculateConditionCode4(void);
extern BOOLE cpuCalculateConditionCode5(void);
extern BOOLE cpuCalculateConditionCode6(void);
extern BOOLE cpuCalculateConditionCode7(void);
extern BOOLE cpuCalculateConditionCode8(void);
extern BOOLE cpuCalculateConditionCode9(void);
extern BOOLE cpuCalculateConditionCode10(void);
extern BOOLE cpuCalculateConditionCode11(void);
extern BOOLE cpuCalculateConditionCode12(void);
extern BOOLE cpuCalculateConditionCode13(void);
extern BOOLE cpuCalculateConditionCode14(void);
extern BOOLE cpuCalculateConditionCode15(void);
extern BOOLE cpuCalculateConditionCode(ULO cc);

// Logging
#ifdef CPU_INSTRUCTION_LOGGING
extern void cpuCallInstructionLoggingFunc(void);
extern void cpuCallExceptionLoggingFunc(STR *description, ULO original_pc, UWO opcode);
extern void cpuCallInterruptLoggingFunc(ULO level, ULO vector_address);
#endif

// Interrupt
extern void cpuCallCheckPendingInterruptsFunc(void);
extern ULO cpuActivateSSP(void);
extern void cpuSetRaiseInterrupt(BOOLE raise_irq);
extern BOOLE cpuGetRaiseInterrupt(void);

// Exceptions
extern void cpuThrowPrivilegeViolationException(void);
extern void cpuThrowIllegalInstructionException(BOOLE executejmp);
extern void cpuThrowFLineException(void);
extern void cpuThrowALineException(void);
extern void cpuThrowTrapVException(void);
extern void cpuThrowTrapException(ULO vector_no);
extern void cpuThrowDivisionByZeroException(BOOLE executejmp);
extern void cpuThrowChkException(void);
extern void cpuThrowTraceException(void);
extern void cpuThrowResetException(void);
extern void cpuCallResetExceptionFunc(void);
extern void cpuFrame1(UWO vector_offset, ULO pc);

// Private help functions
static ULO cpuSignExtByteToLong(UBY v) {return (ULO)(LON)(BYT) v;}
static UWO cpuSignExtByteToWord(UBY v) {return (UWO)(WOR)(BYT) v;}
static ULO cpuSignExtWordToLong(UWO v) {return (ULO)(LON)(WOR) v;}
static ULO cpuJoinWordToLong(UWO upper, UWO lower) {return (((ULO)upper) << 16) | ((ULO)lower);}
static ULO cpuJoinByteToLong(UBY upper, UBY midh, UBY midl, UBY lower) {return (((ULO)upper) << 24) | (((ULO)midh) << 16) | (((ULO)midl) << 8) | ((ULO)lower);}
static UWO cpuJoinByteToWord(UBY upper, UBY lower) {return (((UWO)upper) << 8) | ((UWO)lower);}
static BOOLE cpuMsbB(UBY v) {return v>>7;}
static BOOLE cpuMsbW(UWO v) {return v>>15;}
static BOOLE cpuMsbL(ULO v) {return v>>31;}
static BOOLE cpuIsZeroB(UBY v) {return v == 0;}
static BOOLE cpuIsZeroW(UWO v) {return v == 0;}
static BOOLE cpuIsZeroL(ULO v) {return v == 0;}


#endif