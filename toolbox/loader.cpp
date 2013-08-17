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

#include <string>
#include <vector>

#include <CoreServices/CoreServices.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "loader.h"
#include "toolbox.h"
#include "stackframe.h"

#include "RM.h"
#include "MM.h"

#include <macos/sysequ.h>

using ToolBox::Log;

namespace Loader {

	namespace {

		const uint32_t kCODE = 0x434f4445;

		struct SegmentInfo
		{
			SegmentInfo()
			{}

			SegmentInfo(uint32_t h, uint32_t a, uint32_t s, bool fm = false):
				handle(h), address(a), size(s), farModel(fm)
			{}

			uint32_t handle = 0;
			uint32_t address = 0;
			uint32_t size = 0;
			bool farModel = false;
		};

		struct Segment0Info 
		{
			Segment0Info()
			{}
			uint32_t a5 = 0;
			uint32_t jtOffset = 0;
			uint32_t jtSize = 0;
			uint32_t jtStart = 0;
			uint32_t jtEnd = 0;
		};

		std::vector<SegmentInfo> Segments;

		void reloc1(const uint8_t *r, uint32_t address, uint32_t offset)
		{
			// %00000000 00000000 -> break
			// %0xxxxxxx -> 7-bit value
			// %1xxxxxxx xxxxxxxx -> 15-bit value
			// %00000000 1xxxxxxx x{8} x{8} x{8} -> 31 bit value
			// ^ that's what the documentation says.. 
			// that's how the 32-bit bootstrap works
			// DumpCode ignores the high 2 bytes.
			for(;;)
			{
				uint32_t x;
				uint32_t value;

				x = *r++;

				if (x == 0x00)
				{
					x = *r++;
					if (x == 0x00) break;

					x = (x << 8) | *r++;
					x = (x << 8) | *r++;
					x = (x << 8) | *r++;
				}
				else if (x & 0x80)
				{
					x &= 0x7f;
					x = (x << 8) | *r++;
				}

				x <<= 1; // * 2

				address += x;

				value = memoryReadLong(address);
				memoryWriteLong(value + offset, address);
			}

		}

		// relocate a far model segment.
		void relocate(uint32_t address, uint32_t size, uint32_t a5)
		{
			// see MacOS RT Architecture, 10-23 .. 10-26
			uint32_t offset;

			offset = memoryReadLong(address + 0x14);
			if (memoryReadLong(address + 0x18) != a5 && offset != 0)
			{
				memoryWriteLong(a5, address + 0x18); // current value of A5
				reloc1(memoryPointer(address + offset), address, a5);
			}

			offset = memoryReadLong(address + 0x1c);
			if (memoryReadLong(address + 0x20) != address && offset != 0)
			{
				memoryWriteLong(address, address + 0x20); // segment load address.
				reloc1(memoryPointer(address + offset), address, address + 0x28);
			}
		}


		// load code seg 0.
		uint16_t LoadCode0(Segment0Info &rv)
		{
			uint16_t err;
			uint32_t rHandle;
			uint32_t size;
			uint32_t address;

			SegmentInfo si;

			err = RM::Native::GetResource(kCODE, 0, rHandle);
			if (err) return err;


			MM::Native::HLock(rHandle);
			MM::Native::GetHandleSize(rHandle, size);

			address = memoryReadLong(rHandle);

			uint32_t above = memoryReadLong(address);
			uint32_t below = memoryReadLong(address + 4);
			rv.jtSize = memoryReadLong(address + 8);
			rv.jtOffset = memoryReadLong(address + 12);

			si.size = above + below;


			// create a new handle for the a5 segment.
			err = MM::Native::NewHandle(si.size, true, si.handle, si.address);
			if (err) return err;

			MM::Native::HLock(si.handle);

			rv.a5 = si.address + below;

			// copy jump table data from the CODE segment
			// to the new handle.
			std::memcpy(memoryPointer(rv.a5 + rv.jtOffset), memoryPointer(address + 16), rv.jtSize);

			if (Segments.size() <= 0)
				Segments.resize(0 + 1);
			Segments[0] = si;

			rv.jtStart = rv.a5 + rv.jtOffset;
			rv.jtEnd = rv.jtStart + rv.jtSize;

			// TODO -- should ReleaseResource on rHandle.
			return 0;
		}

		// load a standard code segment.
		uint16_t LoadCode(uint16_t segment)
		{
			uint16_t err;

			SegmentInfo si;

			err = RM::Native::GetResource(kCODE, segment, si.handle);
			if (err) return err;

			MM::Native::HLock(si.handle);
			MM::Native::GetHandleSize(si.handle, si.size);

			si.address = memoryReadLong(si.handle);

			if (memoryReadWord(si.address) == 0xffff)
				si.farModel = true;

			if (Segments.size() <= segment)
				Segments.resize(segment + 1);

			Segments[segment] = si;

			return 0;
		}


	}

	namespace Native 
	{

		uint16_t LoadFile(const std::string &path)
		{

			HFSUniStr255 fork = {0,{0}};
			ResFileRefNum refNum;
			FSRef ref;
			OSErr err;

			// open the file
			// load code seg 0
			// iterate and load other segments


			// TODO -- call RM::Native::OpenResourceFile(...);

			err = FSPathMakeRef( (const UInt8 *)path.c_str(), &ref, NULL);
			if (err) return err;


			::FSGetResourceForkName(&fork);

			err = ::FSOpenResourceFile(&ref, 
				fork.length, 
				fork.unicode, 
				fsRdPerm, 
				&refNum);

			if (err) return err;


			// in case of restart?
			Segments.clear();

			RM::Native::SetResLoad(true);

			// load code 0.
			Segment0Info seg0;
			err = LoadCode0(seg0);

			// iterate through the jump table to get the other 
			// code segments to load
			bool farModel = false;
			for (uint32_t jtEntry = seg0.jtStart; jtEntry < seg0.jtEnd; jtEntry += 8)
			{
				uint16_t seg;
				uint32_t offset;

				if (farModel)
				{
					seg = memoryReadWord(jtEntry + 0);
					offset = memoryReadLong(jtEntry + 4);

					assert(memoryReadWord(jtEntry + 2) == 0xA9F0);
				}
				else
				{
					if (memoryReadWord(jtEntry + 2) == 0xffff)
					{
						farModel = true;
						continue;
					}

					offset = memoryReadWord(jtEntry + 0);
					seg = memoryReadWord(jtEntry + 4);

					assert(memoryReadWord(jtEntry + 2) == 0x3F3C);
					assert(memoryReadWord(jtEntry + 6) == 0xA9F0);
				}

				// load, if necessary.
				assert(seg);

				if (seg >= Segments.size() || Segments[seg].address == 0)
				{
					err = LoadCode(seg);
					if (err) return err;

					const auto &p = Segments[seg];
					if (p.farModel)
					{
						relocate(p.address, p.size, seg0.a5);
					}
				}

				const auto &p = Segments[seg];

				assert(p.address); // missing segment?!
				assert(offset < p.size);

				// +$4/$28 for the jump table info header.
				uint32_t address = p.address + offset + (p.farModel ? 0x00 : 0x04); // was 0x28

				if (!p.farModel)
					memoryWriteWord(seg, jtEntry + 0);

				memoryWriteWord(0x4EF9, jtEntry + 2);
				memoryWriteLong(address, jtEntry + 4);
			}

			// seg:16, jmp:16, address:32
			uint32_t pc = memoryReadLong(seg0.jtStart + 4);

			cpuSetAReg(5, seg0.a5);
			cpuInitializeFromNewPC(pc);


			// 0x0934 - CurJTOffset (16-bit)
			memoryWriteWord(seg0.jtOffset, MacOS::CurJTOffset);

			// 0x0904 -- CurrentA5 (32-bit)
			memoryWriteLong(seg0.a5, MacOS::CurrentA5);

			// 0x0910 CurApName (31-char max pstring.)
			{
				std::string s;

				auto ix = path.rfind('/');
				if (ix == path.npos)
				{
					s = path.substr(0, 31);
				}
				else
				{
					s = path.substr(ix + 1, 31);
				}

				ToolBox::WritePString(MacOS::CurApName, s);
			}



			return 0;
		}


	} // Internal namespace


	/*
	 * struct { uint32_t a5; uint32_t pc; };
	 * future state: Native::LoadFile(const std::string &path, a5pc) -> 
	 * load file, set global page variables, set A5, return PC.
	 *
	 */


	uint16_t UnloadSeg(uint16_t trap)
	{
		// UnloadSeg (routineAddr: Ptr);	

		/*
		 * ------------
		 * +0 routineAddr
		 * ------------
		 *
		 */

		uint32_t sp;
		uint32_t routineAddr;

		sp = StackFrame<4>(routineAddr);

		Log("%04x UnloadSeg(%08x)\n", trap, routineAddr);

		return 0;
	}



}