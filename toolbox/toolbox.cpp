/*
 * Copyright (c) 2013, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <cstdio>
#include <cstdint>
#include <cassert>

#include <stdlib.h>
#include <string>
#include <utility>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "toolbox.h"
#include "rm.h"
#include "mm.h"
#include "os.h"
#include "qd.h"
#include "sane.h"
#include "utility.h"
#include "loader.h"
#include "stackframe.h"
#include "macos/traps.h"

// yuck.  TST.W d0
extern "C" void cpuSetFlagsNZ00NewW(UWO res);


template<int Bytes>
void Push__(uint32_t sp)
{
	static_assert(Bytes == 0, "Invalid Stack Size");
}

template<int Bytes, typename... Args>
void Push__(uint32_t sp, uint16_t value, Args&&... args);

template<int Bytes, typename... Args>
void Push__(uint32_t sp, uint32_t value, Args&&... args);


template<int Bytes, typename... Args>
void Push__(uint32_t sp, uint16_t value, Args&&... args)
{
	memoryWriteWord(value, sp);
	Push__<Bytes-2>(sp + 2, std::forward<Args>(args)...);
}

template<int Bytes, typename... Args>
void Push__(uint32_t sp, uint32_t value, Args&&... args)
{
	memoryWriteLong(value, sp);
	Push__<Bytes-4>(sp + 4, std::forward<Args>(args)...);
}

template<int Bytes, typename... Args>
void Push(Args&&... args)
{
	uint32_t sp = cpuGetAReg(7) - Bytes;
	cpuSetAReg(7, sp);

	Push__<Bytes>(sp, std::forward<Args>(args)...);

}


namespace ToolBox {

	bool Trace = false;

	uint32_t trap_address[1024];
	uint32_t os_address[256];

	namespace {
		inline constexpr bool is_tool_trap(uint16_t trap)
		{
			// %1010 | x . . .  | . . . . . . . . |
			//
			// x = 1 if this is a toolbox call, x = 0 if this is an os call.

			return (trap & 0x0800) == 0x0800;
		}

		inline constexpr bool auto_pop(uint16_t trap)
		{
			// %1010 | 1 x . .  | . . . . . . . . |
			//
			// if x = 1, an extra rts is pushed,
			return (trap & 0x0c00) == 0x0c00;			
		}

		inline constexpr bool save_a0(uint16_t trap)
		{
			// %1010 | 0 . . x  | . . . . . . . . |
			// 
			// if x = 0, a0 is saved and restored.
			// if x = 1, a0 is not saved and restored.
			return (trap & 0x0900) == 0x0000;
		}

		inline constexpr unsigned os_flags(uint16_t trap)
		{
			// %1010 | 0 x x .  | . . . . . . . . |
			return trap & 0x0600;
		}

	}

	void pre_dispatch(uint16_t trap)
	{
		uint32_t returnPC = 0;

		bool saveA0 = false;
		uint32_t a0 = cpuGetAReg(0);

		if (trap == 0xafff)
		{
			// called from the trap address stub.

			// uint16_t 0xafff
			// uint16_t trap
			// return address, etc, is on stack.


			uint32_t pc = cpuGetPC();
			trap = memoryReadWord(pc);

			if (is_tool_trap(trap))
			{
				StackFrame<4>(returnPC);
			}

			#if 0

			/*
			 * os stubs: 
			 * os_return_a0
			 * pop registers a0, a1, d0, d1, d2
			 * pop long word
			 * tst.w d0
			 * rts
			 *
			 * os_return:
			 * pop registers a0, a1, d0, d1, d2
			 * pop long word
			 * tst.w d0
			 * rts
			 *
			 * os_entry_a0:
			 * 
			 */
			else
			{
				// os calls push the trap dispatcher pc as well.  
				uint32_t tmp;
				uint32_t a0, a1, d0, d1, d2;
				if (save_a0(trap))
				{
					StackFrame<4*8>(returnPC, tmp, a0, a1, d0, d1, d2, tmp);
				}
				else
				{
					StackFrame<4*7>(returnPC, tmp, a1, d0, d1, d2, tmp);
				}
				trap = d1;  // trap w/ flag bits.
			}
			#endif

			dispatch(trap);
			cpuInitializeFromNewPC(returnPC);
			return;
		}



		/*
		 * The auto-pop bit is bit 10 in an A-line instruction for a
		 * Toolbox routine. Some language systems prefer to generate
		 * jump-subroutine calls (JSR) to intermediate routines, called
		 * glue routines, which then call Toolbox routines instead of
		 * executing the Toolbox routine directly. This glue method
		 * would normally interfere with Toolbox traps because the
		 * return address of the glue subroutine is placed on the stack
		 * between the Toolbox routine's parameters and the address of
		 * the place where the glue routine was called from (where
		 * control returns once the Toolbox routine has completed
		 * execution).
		 *
		 * The auto-pop bit forces the trap dispatcher to remove the
		 * top 4 bytes from the stack before dispatching to the Toolbox
		 * routine. After the Toolbox routine completes execution,
		 * control is transferred back to the place where the glue
		 * routine was called from, not back to the glue routine.
		 *
		 * Most development environments, including MPW, do not use
		 * this feature.
		 * 
		 * (Trap Manager, 8-20)
		 */

		if (auto_pop(trap))
		{
			/*
			 * | args      |
			 * | returnPC  |
			 * -------------
			 */

			StackFrame<4>(returnPC);
			trap &= ~0x0400;
		}


		if (is_tool_trap(trap))
		{
			uint16_t tt = trap & 0x03ff;
			uint32_t address = trap_address[tt];
			if (address)
			{
				/*
				 * parameters were pushed on the stack but
				 * the a-line instruction was intercepted
				 * before an exception occurred.  Therefore,
				 * we need to push the return address on the 
				 * stack and set the new pc to the trap address.
				 *
				 * returnPC may have been previously set from the
				 * auto pop bit.
				 */

				Push<4>(returnPC == 0 ? cpuGetPC() : returnPC);
				cpuInitializeFromNewPC(address);
				return;
			}
		}
		else
		{
			// os calls - check the return/save a0 bit.
			// if bit 8 is 0, a0 is saved and restored.
			// if bit 8 is 1, a0 is modified.
			// Trap Manager 8-12.

			// not actually an issue yet, will only matter
			// if os address is overridden. 

			if (save_a0(trap)) {
				saveA0 = true;
			}
			uint16_t tt = trap & 0x00ff;

			uint32_t address = os_address[tt];

			if (address) {
				assert("OS trap overrides are not yet supported.");
			}
		}


		dispatch(trap);

		if (saveA0) cpuSetAReg(0, a0);
		if (returnPC) cpuInitializeFromNewPC(returnPC);
	}

	void dispatch(uint16_t trap)
	{

		uint32_t d0 = 0;
		switch (trap)
		{
			case 0xa000: // Open
			case 0xa200: // HOpen
				d0 = OS::Open(trap);
				break;

			case 0xa00a: // OpenRF
			case 0xa20a: // HOpenRF
				d0 = OS::OpenRF(trap);
				break;


			case 0xa001:
				d0 = OS::Close(trap);
				break;

			case 0xa002:
				d0 = OS::Read(trap);
				break;

			case 0xa003:
				d0 = OS::Write(trap);
				break;

			case 0xa008: // Create
			case 0xa208: // HCreate
				d0 = OS::Create(trap);
				break;

			case 0xa009:
				d0 = OS::Delete(trap);
				break;

			case 0xa00c: // GetFInfo
			case 0xa20c: // HGetFInfo
				d0 = OS::GetFileInfo(trap);
				break;


			case 0xa00d: // SetFileInfo
			case 0xa20d: // HSetFileInfo
				d0 = OS::SetFileInfo(trap);
				break;

			case 0xa011:
				d0 = OS::GetEOF(trap);
				break;
				
			case 0xa012:
				d0 = OS::SetEOF(trap);
				break;
			case 0xa013:
				d0 = OS::FlushVol(trap);
				break;

			case 0xa014:
				d0 = OS::GetVol(trap);
				break;

			case 0xa015: // SetVol
			case 0xa215: // HSetVol
				d0 = OS::SetVol(trap);
				break;
				
			case 0xa214:
				d0 = OS::HGetVol(trap);
				break;

			case 0xa018:
				d0 = OS::GetFPos(trap);
				break;

			case 0xa044:
				d0 = OS::SetFPos(trap);
				break;

			case 0xa060:
				d0 = OS::FSDispatch(trap);
				break;
				
			case 0xa260:
				d0 = OS::HFSDispatch(trap);
				break;

			case 0xaa52:
				d0 = OS::HighLevelHFSDispatch(trap);
				break;


			case 0xA746:
				d0 = OS::GetToolTrapAddress(trap);
				break;

			case 0xa647:
				d0 = OS::SetToolTrapAddress(trap);
				break;

			case 0xA346:
				d0 = OS::GetOSTrapAddress(trap);
				break;

			case 0xA823:
				d0 = OS::AliasDispatch(trap);
				break;

			case 0xA1AD:
				d0 = OS::Gestalt(trap);
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

			case 0xA11D:
				d0 = MM::MaxMem(trap);
				break;

			case 0xA01C:
				d0 = MM::FreeMem(trap);
				break;
				
			// CompactMem (cbNeeded: Size) : Size;
			case 0xa04c:
				d0 = MM::CompactMem(trap);
				break;

			case 0xa040:
				d0 = MM::ReserveMem(trap);
				break;

			case 0xa055:
				d0 = MM::StripAddress(trap);
				break;

			case 0xa061:
				d0 = MM::MaxBlock(trap);
				break;

			case 0xa069:
				d0 = MM::HGetState(trap);
				break;

			// MoveHHi (h: Handle);
			case 0xa064:
				d0 = MM::MoveHHi(trap);
				break;

			case 0xa9e1:
				d0 = MM::HandToHand(trap);
				break;

			case 0xa9e3:
				d0 = MM::PtrToHand(trap);
				break;
				
			case 0xa11a:
				d0 = MM::GetZone(trap);
				break;
				
			case 0xa01b:
				d0 = MM::SetZone(trap);
				break;

			case 0xa126:
				d0 = MM::HandleZone(trap);
				break;

			// MaxApplZone
			case 0xa063:
				d0 = MM::MaxApplZone(trap);
				break;

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

			case 0xa193:
				d0 = OS::Microseconds(trap);
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

			case 0xa065:
				d0 = MM::StackSpace(trap);
				break;

			// NewHandle (logicalSize: Size) : Handle;
			case 0xA122:
			case 0xa322:
				d0 = MM::NewHandle(trap);
				break;

			case 0xA027:
				d0 = MM::ReallocHandle(trap);
				break;
				
			case 0xA02B:
				d0 = MM::EmptyHandle(trap);
				break;

			// resource manager stuff.

			// Count1Resources (theType: ResType): Integer;
			case 0xa80d:
				d0 = RM::Count1Resources(trap);
				break;

			case 0xa80e:
				d0 = RM::Get1IndResource(trap);
				break;

			case 0xa80f:
				d0 = RM::Get1IndType(trap);
				break;

			case 0xa81c:
				d0 = RM::Count1Types(trap);
				break;

			// Get1Resource (theType: ResType; thelD: INTEGER) : Handle;
			case 0xa81f:
				d0 = RM::Get1Resource(trap);
				break;

			// Get1NamedResource (theType: ResType; name: Str255) : Handle;
			case 0xa820:
				d0 = RM::Get1NamedResource(trap);
				break;


			case 0xa992:
				d0 = RM::DetachResource(trap);
				break;

			case 0xa994:
				d0 = RM::CurResFile(trap);
				break;

			case 0xA997:
				d0 = RM::OpenResFile(trap);
				break;

			case 0xA998:
				d0 = RM::UseResFile(trap);
				break;

			case 0xa999:
				d0 = RM::UpdateResFile(trap);
				break;

			case 0xa99a:
				d0 = RM::CloseResFile(trap);
				break;

			case 0xa99b:
				d0 = RM::SetResLoad(trap);
				break;

			// GetResource (theType: ResType; thelD: INTEGER) : Handle;
			case 0xa9a0:
				d0 = RM::GetResource(trap);
				break;

			case 0xa9a2:
				d0 = RM::LoadResource(trap);
				break;

			// ReleaseResource (theResource: Handle);
			case 0xa9a3:
				d0 = RM::ReleaseResource(trap);
				break;

			case 0xa9a4:
				d0 = RM::HomeResFile(trap);
				break;

			case 0xa9a5:
				d0 = RM::GetResourceSizeOnDisk(trap);
				break;
				
			case 0xa9a6:
				d0 = RM::GetResAttrs(trap);
				break;

			case 0xa9a7:
				d0 = RM::SetResAttrs(trap);
				break;

			case 0xa9a8:
				d0 = RM::GetResInfo(trap);
				break;

			case 0xa9ab:
				d0 = RM::AddResource(trap);
				break;

			case 0xa9ad:
				d0 = RM::RemoveResource(trap);
				break;
				
			// ResError : INTEGER;
			case 0xa9af:
				d0 = RM::ResError(trap);
				break;

			case 0xa9b0:
				d0 = RM::WriteResource(trap);
				break;

			// CreateResFile(fileName: Str255);
			case 0xa9b1:
				d0 = RM::CreateResFile(trap);
				break;

			// OpenRFPerm (fileName: Str255; vRefNum: Integer;
            //       permission: SignedByte): Integer;
			case 0xa9c4:
				d0 = RM::OpenRFPerm(trap);
				break;

			case 0xa9f6:
				d0 = RM::GetResFileAttrs(trap);
				break;

			case 0xa9f7:
				d0 = RM::SetResFileAttrs(trap);
				break;


			// UnloadSeg (routineAddr: Ptr);
			case 0xa9f1:
				d0 = Loader::UnloadSeg(trap);
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


			case 0xa834:
				d0 = QD::SetFScaleDisable(trap);
				break;

			//SetCursor (crsr: Cursor)
			case 0xA851:
				d0 = QD::SetCursor(trap);
				break;

			case 0xa86e:
				d0 = QD::InitGraf(trap);
				break;

			// GetFNum (fontName: Str255; VAR theNum: INTEGER);
			case 0xa900:
				d0 = QD::GetFNum(trap);
				break;


			// sane
			case 0xa9Eb:
				d0 = SANE::fp68k(trap);
				cpuSetDReg(0, d0);
				return; // SANE sets the flags.
				break;

			case 0x0a9ee:
				d0 = SANE::decstr68k(trap);
				break;

			// utility

			case 0xa906:
				d0 = Utility::NewString(trap);
				break;
			case 0xa9ba:
				d0 = Utility::GetString(trap);
				break;

			case 0xa85d:
				d0 = Utility::BitTst(trap);
				break;

			default:
				fprintf(stderr, "Unsupported tool trap: %04x (%s)\n",
						trap, TrapName(trap));
				fprintf(stderr, "pc: %08x\n", cpuGetPC());
				exit(255);
		}

		if (d0)
		{
			int16_t v = (int16_t)d0;
			Log("     -> %d\n", v);
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

		if (fname) tmp = MacToUnix(tmp);

		return tmp;
	}
	
	std::string ReadPString(uint32_t address, bool fname)
	{
		std::string tmp;

		if (address)
		{
			unsigned length = memoryReadByte(address);
		
			tmp.assign((char *)memoryPointer(address + 1), length);

			if (fname) tmp = MacToUnix(tmp);
			
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

	std::string TypeToString(uint32_t type)
	{
		char tmp[5] = { 0, 0, 0, 0, 0};

		for (unsigned i = 0; i < 4; ++i)
		{
			char c = type & 0xff;
			type >>= 8;

			c = isprint(c) ? c : '.';

			tmp[3 - i] = c;
		}

		return std::string(tmp);
	}

}
