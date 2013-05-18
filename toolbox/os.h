#ifndef __mpw_os_h__
#define __mpw_os_h__

#include <cstdint>
#include <string>
#include <ctime>

namespace OS
{

	enum {
		fsCurPerm = 0,
		fsRdPerm = 1,
		fsWrPerm = 2,
		fsRdWrPerm = 3,
		fsRdWrShPerm = 4,
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


	#pragma mark FS Utilities
	uint16_t Close(uint16_t trap);
	uint16_t Create(uint16_t trap);
	uint16_t Delete(uint16_t trap);
	uint16_t FlushVol(uint16_t trap);
	
	uint16_t GetFileInfo(uint16_t trap);
	uint16_t SetFileInfo(uint16_t trap);

	uint16_t GetEOF(uint16_t trap);
	uint16_t SetEOF(uint16_t trap);

	uint16_t SetFPos(uint16_t trap);

	uint16_t GetVol(uint16_t trap);
	uint16_t HGetVol(uint16_t trap);

	uint16_t Open(uint16_t trap);
	uint16_t Read(uint16_t trap);
	uint16_t Write(uint16_t trap);

	#pragma mark String Utilities
	uint16_t CmpString(uint16_t trap);


	#pragma mark - Time Utilities
	uint16_t ReadDateTime(uint16_t trap);
	uint16_t SecondsToDate(uint16_t trap);
	uint16_t Pack6(uint16_t trap);

	uint16_t TickCount(uint16_t trap);

	uint16_t HFSDispatch(uint16_t trap);
	uint16_t HighLevelHFSDispatch(uint16_t trap);

	#pragma mark - Trap Manager

	uint16_t GetToolTrapAddress(uint16_t trap);
	uint16_t GetOSTrapAddress(uint16_t trap);

	#pragma mark - Alias Manager

	uint16_t ResolveAliasFile();
	uint16_t AliasDispatch(uint16_t trap);

}

#endif
