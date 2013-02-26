#include <cstdio>
#include <cstdint>
#include <stdlib.h>
#include <string>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "toolbox.h"
#include "rm.h"
#include "mm.h"
#include "os.h"
#include "qd.h"
#include "sane.h"


// yuck.  TST.W d0
extern "C" void cpuSetFlagsNZ00NewW(UWO res);

namespace ToolBox {

	bool Trace = false;


	void dispatch(uint16_t trap)
	{
		// todo -- check/remove extra bits for save a0, toolglue, etc.

		uint32_t d0 = 0;
		switch (trap)
		{
			case 0xa000:
				d0 = OS::Open(trap);
				break;

			case 0xa001:
				d0 = OS::Close(trap);
				break;

			case 0xa002:
				d0 = OS::Read(trap);
				break;

			case 0xa008:
				d0 = OS::Create(trap);
				break;
			case 0xa009:
				d0 = OS::Delete(trap);
				break;

			case 0xA00C:
				d0 = OS::GetFileInfo(trap);
				break;

			case 0xa00d:
				d0 = OS::SetFileInfo(trap);
				break;

			case 0xa011:
				d0 = OS::GetEOF(trap);
				break;

			case 0xa014:
				d0 = OS::GetVol(trap);
				break;

			// SetPtrSize (p: Ptr; newSize: Size);
			case 0xa020:
				d0 = MM::SetPtrSize(trap);
				break;

			// GetPtrSize (p: Ptr) : Size
			case 0xa021:
				d0 = MM::GetPtrSize(trap);
				break;

			case 0xA023:
				d0 = MM::DisposeHandle(trap);
				break;

			case 0xa024:
				d0 = MM::SetHandleSize(trap);
				break;

			case 0xa025:
				d0 = MM::GetHandleSize(trap);
				break;

			case 0xa029:
				d0 = MM::HLock(trap);
				break;

			case 0xa02a:
				d0 = MM::HUnlock(trap);
				break;

			// BlockMove (sourcePtr,destPtr: Ptr; byteCount: Size);
			case 0xa02e:
				d0 = MM::BlockMove(trap);
				break;

			case 0xa049:
				d0 = MM::HPurge(trap);
				break;

			// CompactMem (cbNeeded: Size) : Size;
			case 0xa04c:
				d0 = MM::CompactMem(trap);
				break;

			case 0xa055:
				d0 = MM::StripAddress(trap);
				break;

			// MoveHHi (h: Handle);
			case 0xa064:
				d0 = MM::MoveHHi(trap);
				break;

			case 0xa9e3:
				d0 = MM::PtrToHand(trap);

			// ReadDateTime (VAR sees: LONGINT) : OSErr;
			case 0xa039:
				d0 = OS::ReadDateTime(trap);
				break;

			// SecondsToDate (s: LongInt; VAR d: DateTimeRec);
			case 0xa9c6:
				d0 = OS::SecondsToDate(trap);
				break;
		
			// TickCount : LONGINT;
			case 0xa975:
				d0 = OS::TickCount(trap);
				break;

			case 0xa9ed:
				d0 = OS::Pack6(trap);
				break;

			//_CmpString [MARKS,CASE]
			case 0xa03c:
			case 0xa23c:
			case 0xa43c:
			case 0xa63c:
				d0 = OS::CmpString(trap);
				break;

			// NewPtr [Sys, Clear] (logicalSize: Size): Ptr;
			case 0xa11e:
			case 0xa31e:
			case 0xa51e:
			case 0xa71e:
				// clear = bit 9, sys = bit 10
				d0 = MM::NewPtr(trap);
				break;

			// DisposPtr (p: Ptr);
			case 0xa01f:
				d0 = MM::DisposePtr(trap);
				break;

			// NewHandle (logicalSize: Size) : Handle;
			case 0xA122:
			case 0xa322:
				d0 = MM::NewHandle(trap);
				break;

			// resource manager stuff.

			// Get1Resource (theType: ResType; thelD: INTEGER) : Handle;
			case 0xa81f:
				d0 = RM::Get1Resource(trap);
				break;

			// Get1NamedResource (theType: ResType; name: Str255) : Handle;
			case 0xa820:
				d0 = RM::Get1NamedResource(trap);
				break;

			// GetResource (theType: ResType; thelD: INTEGER) : Handle;
			case 0xa9a0:
				d0 = RM::GetResource(trap);
				break;

			// ReleaseResource (theResource: Handle);
			case 0xa9a3:
				d0 = RM::ReleaseResource(trap);
				break;

			// UnloadSeg (routineAddr: Ptr);
			case 0xa9f1:
				d0 = RM::UnloadSeg(trap);
				break;

			// quickdraw (AsmIIgs ... )

			// _ShowCursor();
			case 0xA853:
				d0 = QD::ShowCursor(trap);
				break;

			// GetCursor (cursorlD: INTEGER) : CursHandle;
			case 0xA9B9:
				d0 = QD::GetCursor(trap);
				break;


			//SetCursor (crsr: Cursor)
			case 0xA851:
				d0 = QD::SetCursor(trap);
				break;

			// GetFNum (fontName: Str255; VAR theNum: INTEGER);
			case 0xa900:
				d0 = QD::GetFNum(trap);
				break;


			// sane
			case 0xa9Eb:
				d0 = SANE::fp68k(trap);
				break;

			case 0x0a9ee:
				d0 = SANE::decstr68k(trap);
				break;

			default:
				fprintf(stderr, "Unsupported tool trap: %04x\n", trap);
				fprintf(stderr, "pc: %08x\n", cpuGetPC());
				exit(255);
		}

		cpuSetDReg(0, d0);
		cpuSetFlagsNZ00NewW(d0);
	}

	std::string ReadCString(uint32_t address, bool fname)
	{
		std::string tmp;

		if (address)
		{
			tmp.assign((char *)memoryPointer(address));
		}

		if (fname) std::replace(tmp.begin(), tmp.end(), ':', '/');

		return tmp;
	}
	
	std::string ReadPString(uint32_t address, bool fname)
	{
		std::string tmp;

		if (address)
		{
			unsigned length = memoryReadByte(address);
		
			tmp.assign((char *)memoryPointer(address + 1), length);

			if (fname) std::replace(tmp.begin(), tmp.end(), ':', '/');
			
		}

		return tmp;
	}

	std::string ReadString(uint32_t address, uint32_t length)
	{
		std::string tmp;

		if (address)
		{
			tmp.assign((char *)memoryPointer(address), length);
		}
		return tmp;
	}


	bool WritePString(uint32_t address, const std::string &s)
	{
		int length = s.length();
		if (length > 255) return false;
		if (address == 0) return false;

		uint8_t *ptr = memoryPointer(address);
		*ptr++  = (uint8_t)length;
		for (char c : s)
		{
			*ptr++ = (uint8_t)c;
		}
		return true;
	}


}