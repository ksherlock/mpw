#ifndef FMEM_H
#define FMEM_H

/* Memory access functions */

extern UBY memoryReadByte(ULO address);
extern UWO memoryReadWord(ULO address);
extern ULO memoryReadLong(ULO address);
extern void memoryWriteByte(UBY data, ULO address);
extern void memoryWriteWord(UWO data, ULO address);
extern void memoryWriteLong(ULO data, ULO address);

extern UWO memoryChipReadWord(ULO address);
extern void memoryChipWriteWord(UWO data, ULO address);

#define memoryReadByteFromPointer(address) (address[0])
#define memoryReadWordFromPointer(address) ((address[0] << 8) | address[1])
#define memoryReadLongFromPointer(address) ((address[0] << 24) | (address[1] << 16) | (address[2] << 8) | address[3])

/* IO Bank functions */

typedef UWO (*memoryIoReadFunc)(ULO address);
typedef void (*memoryIoWriteFunc)(UWO data, ULO address);

extern void memorySetIoReadStub(ULO index, memoryIoReadFunc ioreadfunction);
extern void memorySetIoWriteStub(ULO index, memoryIoWriteFunc iowritefunction);

/* For the copper */
extern memoryIoWriteFunc memory_iobank_write[257];

/* Expansion card functions */

typedef void (*memoryEmemCardInitFunc)(void);
typedef void (*memoryEmemCardMapFunc)(ULO);

extern void memoryEmemClear(void);
extern void memoryEmemCardAdd(memoryEmemCardInitFunc cardinit,
			      memoryEmemCardMapFunc cardmap);
extern void memoryEmemSet(ULO index, ULO data);
extern void memoryEmemMirror(ULO emem_offset, UBY *src, ULO size);

/* Device memory functions. fhfile is using these. */

extern void memoryDmemSetByte(UBY data);
extern void memoryDmemSetWord(UWO data);
extern void memoryDmemSetLong(ULO data);
extern void memoryDmemSetLongNoCounter(ULO data, ULO offset);
extern void memoryDmemSetString(STR *data);
extern void memoryDmemSetCounter(ULO val);
extern ULO memoryDmemGetCounter(void);
extern void memoryDmemClear(void);

/* Module management functions */

extern void memorySaveState(FILE *F);
extern void memoryLoadState(FILE *F);
extern void memorySoftReset(void);
extern void memoryHardReset(void);
extern void memoryHardResetPost(void);
extern void memoryEmulationStart(void);
extern void memoryEmulationStop(void);
extern void memoryStartup(void);
extern void memoryShutdown(void);

/* Memory bank functions */

typedef UBY (*memoryReadByteFunc)(ULO address);
typedef UWO (*memoryReadWordFunc)(ULO address);
typedef ULO (*memoryReadLongFunc)(ULO address);
typedef void (*memoryWriteByteFunc)(UBY data, ULO address);
typedef void (*memoryWriteWordFunc)(UWO data, ULO address);
typedef void (*memoryWriteLongFunc)(ULO data, ULO address);

extern memoryReadByteFunc memory_bank_readbyte[65536];
extern memoryReadWordFunc memory_bank_readword[65536];
extern memoryReadLongFunc memory_bank_readlong[65536];
extern memoryWriteByteFunc memory_bank_writebyte[65536];
extern memoryWriteWordFunc memory_bank_writeword[65536];
extern memoryWriteLongFunc memory_bank_writelong[65536];

extern UBY *memory_bank_pointer[65536];
extern UBY *memory_bank_datapointer[65536];

extern void memoryBankSet(memoryReadByteFunc rb,
			  memoryReadWordFunc rw,
			  memoryReadLongFunc rl,
			  memoryWriteByteFunc wb,
			  memoryWriteWordFunc ww,
			  memoryWriteLongFunc wl,
			  UBY *basep,
			  ULO bank, 
			  ULO basebank,
			  BOOLE pointer_can_write);
extern UBY *memoryAddressToPtr(ULO address);
extern void memoryChipMap(BOOLE overlay);

/* Memory configuration properties */

extern BOOLE memorySetChipSize(ULO chipsize);
extern ULO memoryGetChipSize(void);
extern BOOLE memorySetFastSize(ULO fastsize);
extern ULO memoryGetFastSize(void);
extern void memorySetFastAllocatedSize(ULO fastallocatedsize);
extern ULO memoryGetFastAllocatedSize(void);
extern BOOLE memorySetSlowSize(ULO bogosize);
extern ULO memoryGetSlowSize(void);
extern BOOLE memorySetUseAutoconfig(BOOLE useautoconfig);
extern BOOLE memoryGetUseAutoconfig(void);
extern BOOLE memorySetAddress32Bit(BOOLE address32bit);
extern BOOLE memoryGetAddress32Bit(void);
extern BOOLE memorySetKickImage(STR *kickimage);
extern STR *memoryGetKickImage(void);
extern void memorySetKey(STR *key);
extern STR *memoryGetKey(void);
extern BOOLE memoryGetKickImageOK(void);

/* Derived from memory configuration */

extern ULO memoryGetKickImageBaseBank(void);
extern ULO memoryGetKickImageVersion(void);
extern ULO memoryInitialPC(void);
extern ULO memoryInitialSP(void);

/* Kickstart load error handling */

#define MEMORY_ROM_ERROR_SIZE           0
#define MEMORY_ROM_ERROR_AMIROM_VERSION 1
#define MEMORY_ROM_ERROR_AMIROM_READ    2
#define MEMORY_ROM_ERROR_KEYFILE        3
#define MEMORY_ROM_ERROR_EXISTS_NOT     4
#define MEMORY_ROM_ERROR_FILE           5
#define MEMORY_ROM_ERROR_KICKDISK_NOT   6
#define MEMORY_ROM_ERROR_CHECKSUM       7
#define MEMORY_ROM_ERROR_KICKDISK_SUPER 8
#define MEMORY_ROM_ERROR_BAD_BANK       9

/* Global variables */

extern UBY memory_chip[];
extern UBY *memory_fast;
extern UBY memory_slow[];
extern UBY memory_kick[];
extern ULO memory_chipsize;
extern UBY memory_emem[];

extern ULO intenar,intena,intreq;
extern ULO potgor;

extern ULO memory_fault_address;
extern BOOLE memory_fault_read;

#endif
