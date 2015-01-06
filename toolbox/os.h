#ifndef __mpw_os_h__
#define __mpw_os_h__

#include <cstdint>
#include <string>
#include <ctime>

namespace OS
{

	enum {
		fsCurPerm = 0x00,
		fsRdPerm = 0x01,
		fsWrPerm = 0x02,
		fsRdWrPerm = 0x03,
		fsRdWrShPerm = 0x04,
		fsRdDenyPerm = 0x10,
		fsWrDenyPerm = 0x20
	};

	enum {
		fsAtMark = 0,
		fsFromStart = 1,
		fsFromLEOF = 2,
		fsFromMark = 3,
	};



	bool Init();

	bool IsTextFile(const std::string &s);
	bool IsBinaryFile(const std::string &s);

	time_t UnixToMac(time_t);
	time_t MacToUnix(time_t);

	uint16_t OpenCommon(uint32_t parm, bool fsspec, bool resource);


	#pragma mark FS Utilities
	uint16_t Close(uint16_t trap);
	uint16_t Create(uint16_t trap);
	uint16_t Delete(uint16_t trap);
	uint16_t FlushVol(uint16_t trap);
	
	uint16_t GetFileInfo(uint16_t trap);
	uint16_t HGetFileInfo(uint16_t trap);
	uint16_t SetFileInfo(uint16_t trap);

	uint16_t GetEOF(uint16_t trap);
	uint16_t SetEOF(uint16_t trap);

	uint16_t GetFPos(uint16_t trap);
	uint16_t SetFPos(uint16_t trap);

	uint16_t GetVol(uint16_t trap);
	uint16_t HGetVol(uint16_t trap);
	uint16_t HGetVInfo(uint16_t trap);

	uint16_t SetVol(uint16_t trap);



	uint16_t Open(uint16_t trap);
	uint16_t OpenRF(uint16_t trap);
	uint16_t HOpen(uint16_t trap);
	uint16_t Read(uint16_t trap);
	uint16_t Write(uint16_t trap);

	#pragma mark String Utilities
	uint16_t CmpString(uint16_t trap);


	#pragma mark - Time Utilities
	uint16_t ReadDateTime(uint16_t trap);
	uint16_t SecondsToDate(uint16_t trap);
	uint16_t Pack6(uint16_t trap);

	uint16_t TickCount(uint16_t trap);
	uint16_t Microseconds(uint16_t trap);

	uint16_t FSDispatch(uint16_t trap);
	uint16_t HFSDispatch(uint16_t trap);
	uint16_t HighLevelHFSDispatch(uint16_t trap);

	#pragma mark - Trap Manager

	uint16_t GetToolTrapAddress(uint16_t trap);
	uint16_t SetToolTrapAddress(uint16_t trap);

	uint16_t GetOSTrapAddress(uint16_t trap);

	#pragma mark - Alias Manager

	uint16_t ResolveAliasFile();
	uint16_t AliasDispatch(uint16_t trap);

	#pragma mark - Gestalt Manager
	uint16_t Gestalt(uint16_t trap);

	#pragma mark - XP Ram
	uint16_t ReadXPRam(uint16_t trap);
	uint16_t WriteXPRam(uint16_t trap);

	uint16_t InsTime(uint16_t trap);
	uint16_t InsXTime(uint16_t trap);
	uint16_t PrimeTime(uint16_t trap);
	uint16_t RmvTime(uint16_t trap);

}

#endif
