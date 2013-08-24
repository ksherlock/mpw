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
#include <algorithm>
#include <unordered_map>

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
			// todo -- also add std::string segmentName?
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



		// 
		void LoadDebugNames(DebugNameTable &table)
		{

			if (Segments.empty()) return;


			// skip segment 0 since that's the dispatch table.
			for (unsigned seg = 1; seg < Segments.size(); ++seg)
			{

				const auto &si = Segments[seg];
				const uint8_t *memory = memoryPointer(si.address);
				unsigned size = si.size;

				// store entry for start?
				// SEG_## = pc ?


				// See MacsBugs appendix D.

				/*
				 * format is:
				 * (optional) LINK A6, [$4e56]
				 * opcodes...
				 * RTS/JMP (A0)/RTD
				 * procedure name
				 * procedure constants
				 *
				 * name is fixed at 8 or 16 bytes or variable length.
				 * valid characters = [a-zA-Z0-9_%.] and space, to pad fixed length names.
				 *
				 */

				uint32_t start = 0;
				for (unsigned pc = 0; pc < size; )
				{
					bool eof = false; // end of function
					uint32_t oldpc = pc;

					uint16_t opcode = (memory[pc + 0] << 8) | memory[pc + 1];
					pc += 2;

					switch(opcode)
					{
					case 0x4E56:
						// link A6,#
						start = oldpc;
						break;

					case 0x4E75: // rts
					case 0x4ED0: // jmp (a0)
						eof = true;
						break;
					case 0x4E74: // rtd #
						pc += 2; // skip the argument.
						eof = true;
						break;	

					default:
						break;
					}

					if (!eof) continue;

					std::string s;

					unsigned length = memory[pc];

					// variable length.
					if (length >= 0x80 && length <= 0x9f)
					{
						length &= 0x7f;
						++pc;

						if (length == 0)
							length = memory[++pc];

						s.assign((const char *)memory + pc, length);

						// align to word boundary
						pc = (pc + length + 1) & ~1;
					}
					else
					{
						// fixed length.  high byte may or may not be set.
						// if high byte of second char is set, it's a 16-char string.

						length = 8;
						s.assign((const char *)memory + pc, 8);
						s[0] &= 0x7f;

						while (s.length() && s.back() == ' ') s.pop_back();

						if ((s.length() >= 2) && (s[1] & 0x80))
						{
							s[1] &= 0x7f;
							length = 16;

							std::string tmp((const char *)memory + pc + 8, 8);
							while (tmp.length() && tmp.back() == ' ') tmp.pop_back();

							tmp.push_back('.');
							tmp.append(s);
							s = std::move(tmp);
						}

						pc += length;
					}

					// verify name is legal.
					bool ok = std::all_of(s.begin(), s.end(), 
						[](char c) {
							if (c >= 'A' && c <= 'Z') return true;
							if (c >= 'a' && c <= 'z') return true;
							if (c >= '0' && c <= '9') return true;
							if (c == '.') return true;
							if (c == '_') return true;
							if (c == '%') return true;
							return false;
					});

					if (!ok)
					{
						// also set start.

						start = pc = oldpc + 2;
						if (opcode == 0x4E74) start += 2;
						continue;
					}



					// constant data
					length = (memory[pc + 0] << 8) | memory[pc + 1];
					pc += 2;
					if (length)
					{
						pc = (pc + length + 1) & ~1;
					}


					// TODO -- should this include the name and data?
					table.emplace(std::move(s), std::make_pair(start + si.address, pc + si.address));

					// in case no link instruction...
					start = pc;

				}

			}
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