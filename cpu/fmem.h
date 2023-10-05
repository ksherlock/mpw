#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// MPW
extern void memorySetMemory(uint8_t *memory, uint32_t size);
extern void memorySetGlobalLog(uint32_t globalLog);
extern uint8_t *memoryPointer(uint32_t address);


/* Access for chipset emulation that already have validated addresses */

#define chipmemReadByte(address) (memory_chip[address])
#define chipmemReadWord(address) ((((uint16_t) memory_chip[address]) << 8) | ((uint16_t) memory_chip[address + 1]))
#define chipmemWriteWord(data, address) \
  memory_chip[address] = (uint8_t) (data >> 8); \
  memory_chip[address + 1] = (uint8_t) data

/* Memory access functions */

extern uint8_t memoryReadByte(uint32_t address);
extern uint16_t memoryReadWord(uint32_t address);
extern uint32_t memoryReadLong(uint32_t address);
extern uint64_t memoryReadLongLong(uint32_t address);
extern void memoryWriteByte(uint8_t data, uint32_t address);
extern void memoryWriteWord(uint16_t data, uint32_t address);
extern void memoryWriteLong(uint32_t data, uint32_t address);
extern void memoryWriteLongLong(uint64_t data, uint32_t address);

extern uint16_t memoryChipReadWord(uint32_t address);
extern void memoryChipWriteWord(uint16_t data, uint32_t address);

#define memoryReadByteFromPointer(address) (address[0])
#define memoryReadWordFromPointer(address) ((address[0] << 8) | address[1])
#define memoryReadLongFromPointer(address) ((address[0] << 24) | (address[1] << 16) | (address[2] << 8) | address[3])

extern void memoryWriteLongToPointer(uint32_t data, uint8_t *address);

/* IO Bank functions */

typedef uint16_t (*memoryIoReadFunc)(uint32_t address);
typedef void (*memoryIoWriteFunc)(uint16_t data, uint32_t address);

extern void memorySetIoReadStub(uint32_t index, memoryIoReadFunc ioreadfunction);
extern void memorySetIoWriteStub(uint32_t index, memoryIoWriteFunc iowritefunction);

/* For the copper */
extern memoryIoWriteFunc memory_iobank_write[257];

/* Expansion card functions */

typedef void (*memoryEmemCardInitFunc)(void);
typedef void (*memoryEmemCardMapFunc)(uint32_t);

extern void memoryEmemClear(void);
extern void memoryEmemCardAdd(memoryEmemCardInitFunc cardinit,
			      memoryEmemCardMapFunc cardmap);
extern void memoryEmemSet(uint32_t index, uint32_t data);
extern void memoryEmemMirror(uint32_t emem_offset, uint8_t *src, uint32_t size);

/* Device memory functions. fhfile is using these. */

extern void memoryDmemSetByte(uint8_t data);
extern void memoryDmemSetWord(uint16_t data);
extern void memoryDmemSetLong(uint32_t data);
extern void memoryDmemSetLongNoCounter(uint32_t data, uint32_t offset);
extern void memoryDmemSetString(const char *data);
extern void memoryDmemSetCounter(uint32_t val);
extern uint32_t memoryDmemGetCounter(void);
extern uint32_t memoryDmemGetCounterWithoutOffset(void);
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

typedef uint8_t (*memoryReadByteFunc)(uint32_t address);
typedef uint16_t (*memoryReadWordFunc)(uint32_t address);
typedef uint32_t (*memoryReadLongFunc)(uint32_t address);
typedef void (*memoryWriteByteFunc)(uint8_t data, uint32_t address);
typedef void (*memoryWriteWordFunc)(uint16_t data, uint32_t address);
typedef void (*memoryWriteLongFunc)(uint32_t data, uint32_t address);

extern memoryReadByteFunc memory_bank_readbyte[65536];
extern memoryReadWordFunc memory_bank_readword[65536];
extern memoryReadLongFunc memory_bank_readlong[65536];
extern memoryWriteByteFunc memory_bank_writebyte[65536];
extern memoryWriteWordFunc memory_bank_writeword[65536];
extern memoryWriteLongFunc memory_bank_writelong[65536];

extern uint8_t *memory_bank_pointer[65536];
extern uint8_t *memory_bank_datapointer[65536];

extern void memoryBankSet(memoryReadByteFunc rb,
			  memoryReadWordFunc rw,
			  memoryReadLongFunc rl,
			  memoryWriteByteFunc wb,
			  memoryWriteWordFunc ww,
			  memoryWriteLongFunc wl,
			  uint8_t *basep,
			  uint32_t bank, 
			  uint32_t basebank,
			  BOOLE pointer_can_write);
extern uint8_t *memoryAddressToPtr(uint32_t address);
extern void memoryChipMap(bool overlay);

/* Memory configuration properties */

extern BOOLE memorySetChipSize(uint32_t chipsize);
extern uint32_t memoryGetChipSize(void);
extern BOOLE memorySetFastSize(uint32_t fastsize);
extern uint32_t memoryGetFastSize(void);
extern void memorySetFastAllocatedSize(uint32_t fastallocatedsize);
extern uint32_t memoryGetFastAllocatedSize(void);
extern BOOLE memorySetSlowSize(uint32_t bogosize);
extern uint32_t memoryGetSlowSize(void);
extern bool memorySetUseAutoconfig(bool useautoconfig);
extern bool memoryGetUseAutoconfig(void);
extern BOOLE memorySetAddress32Bit(BOOLE address32bit);
extern BOOLE memoryGetAddress32Bit(void);
extern BOOLE memorySetKickImage(char *kickimage);
extern BOOLE memorySetKickImageExtended(char *kickimageext);
extern char *memoryGetKickImage(void);
extern void memorySetKey(char *key);
extern char *memoryGetKey(void);
extern BOOLE memoryGetKickImageOK(void);

/* Derived from memory configuration */

extern uint32_t memoryGetKickImageBaseBank(void);
extern uint32_t memoryGetKickImageVersion(void);
extern uint32_t memoryInitialPC(void);
extern uint32_t memoryInitialSP(void);

/* Kickstart decryption */
extern int memoryKickLoadAF2(char *filename, FILE *F, uint8_t *memory_kick, const bool);

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

extern uint8_t memory_chip[];
extern uint8_t *memory_fast;
extern uint8_t memory_slow[];
extern uint8_t memory_kick[];
extern uint32_t memory_chipsize;
extern uint8_t memory_emem[];

extern uint32_t potgor;

extern uint32_t memory_fault_address;
extern BOOLE memory_fault_read;


#ifdef __cplusplus
}
#endif
