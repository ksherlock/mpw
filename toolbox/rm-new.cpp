/*
 * Copyright (c) 2016, Kelvin W Sherlock
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


#include "rm.h"
#include "mm.h"
#include "toolbox.h"

#include <vector>
#include <algorithm>
#include <string>
#include <algorithm>
#include <memory>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>


#include <macos/errors.h>
#include <macos/tool_return.h>
#include <macos/sysequ.h>

#include <native/native.h>


#include "stackframe.h"
#include "fs_spec.h"

/*
 * todo -- iterators for the resource map?
 */ 


/*
 * refNum 0 = system resource
 * refNum 1 = ROM resource.
 */


using namespace MacOS;
using ToolBox::Log;
using ToolBox::TypeToString;

inline constexpr uint32_t read_32(const uint8_t *cp) {
	return (cp[0] << 24) | (cp[1] << 16) | (cp[2] << 8) | (cp[3] << 0);
}

inline constexpr uint32_t read_24(const uint8_t *cp) {
	return (cp[0] << 16) | (cp[1] << 8) | (cp[2] << 0);
}


inline constexpr uint16_t read_16(const uint8_t *cp) {
	return (cp[0] << 8) | (cp[1] << 0);
}

inline constexpr uint8_t read_8(const uint8_t *cp) {
	return cp[0];
}

inline void check_size(const std::vector<uint8_t> v, size_t size) {
	if (size > v.size()) throw std::out_of_range("check_size");
}

inline void check_offset(const std::vector<uint8_t> v, size_t offset) {
	if (offset >= v.size()) throw std::out_of_range("check_offset");
}

inline uint32_t read_32(const std::vector<uint8_t> v, size_t offset, const std::nothrow_t &) noexcept {
	return (v[offset+0] << 24) | (v[offset+1] << 16) | (v[offset+2] << 8) | (v[offset+3] << 0);
}

inline uint32_t read_24(const std::vector<uint8_t> v, size_t offset, const std::nothrow_t &) noexcept {
	return (v[offset+0] << 16) | (v[offset+1] << 8) | (v[offset+2] << 0);
}

inline uint32_t read_16(const std::vector<uint8_t> v, size_t offset, const std::nothrow_t &) noexcept {
	return (v[offset+0] << 8) | (v[offset+1] << 0);
}

inline uint8_t read_8(const std::vector<uint8_t> v, size_t offset, const std::nothrow_t &) noexcept {
	return v[offset+0];
}


inline uint32_t read_32(const std::vector<uint8_t> v, size_t offset) {
	check_size(v, offset + 4);
	return read_32(v, offset, std::nothrow);
}

inline uint32_t read_24(const std::vector<uint8_t> v, size_t offset) {
	check_size(v, offset + 3);
	return read_24(v, offset, std::nothrow);
}

inline uint32_t read_16(const std::vector<uint8_t> v, size_t offset) {
	check_size(v, offset + 2);
	return read_16(v, offset, std::nothrow);
}

inline uint8_t read_8(const std::vector<uint8_t> v, size_t offset) {
	return v.at(offset+0);
}


inline std::string read_string(const std::vector<uint8_t> v, size_t offset, const std::nothrow_t &)  noexcept {
	int length = v[offset + 0];
	return std::string((const char *)v.data() + offset + 1, length);
}

inline std::string read_string(const std::vector<uint8_t> v, size_t offset) {
	int length = v.at(offset + 0);
	check_size(v, offset + length + 1);
	return std::string((const char *)v.data() + offset + 1, length);
}


enum {
	resSysHeap = 64,
	resPurgeable = 32,
	resLocked = 16,
	resProtected = 8,
	resPreload = 4,
	resChanged = 2,
};

enum {
	mapReadOnly = 128,
	mapCompact = 64,
	mapChanged = 32
};


struct resource {
	uint32_t type = 0;
	uint16_t id = 0;
	uint16_t attr = 0;
	uint32_t data_offset = 0;
	//uint32_t name_offset = 0;
	uint32_t size = -1;
	uint32_t handle = 0;
	std::string name;
};


struct resource_file {
	resource_file *next = nullptr;
	int fd = -1;
	int refnum = -1;


	// header
	uint32_t offset_rdata = 0;	/* offset to resource data */
	uint32_t offset_rmap = 0;	/* offset to resource map  */
	uint32_t length_rdata = 0;	/* length of resource data */
	uint32_t length_rmap = 0;	/* length of resource map  */

	// map
	uint16_t attr = 0;

	std::vector<resource> resources;


	resource_file() = default;
	resource_file(resource_file &&) = delete;
	resource_file(const resource_file &) = delete;

	resource_file &operator=(const resource_file &) = delete;
	resource_file &operator=(resource_file &&) = delete;

	~resource_file() {
		if (fd >= 0) close(fd);
	}
};


bool operator<(const resource &lhs, const resource &rhs) {
	return (lhs.type < rhs.type) || (lhs.type == rhs.type && lhs.id < rhs.id);
}



namespace {

	int refnum = 100;
	bool res_load = true;

	resource_file *current = nullptr;
	resource_file *head = nullptr;


	tool_return<void> read_resource_map(resource_file &rf) {

		ssize_t ok;
		uint8_t header[100];

		ok = read(rf.fd, header, sizeof(header));
		if (ok != sizeof(header)) return mapReadErr;

		rf.offset_rdata = read_32(header+0);
		rf.offset_rmap =  read_32(header+4);
		rf.length_rdata = read_32(header+8);
		rf.length_rmap =  read_32(header+12);

		if (rf.length_rmap < 30) return mapReadErr;
		std::vector<uint8_t> rmap(rf.length_rmap);

		if (lseek(rf.fd, rf.offset_rmap, SEEK_SET) < 0) return macos_error_from_errno();
		ok = read(rf.fd, rmap.data(), rf.length_rmap);
		if (ok != rf.length_rmap) return mapReadErr;

		rf.attr = read_16(rmap, 22, std::nothrow);
		uint16_t offset_type_list = read_16(rmap, 24, std::nothrow);
		uint16_t offset_name_list = read_16(rmap, 26, std::nothrow);
		int count = (read_16(rmap, 28, std::nothrow) + 1) & 0xffff;

		if (offset_type_list < 30-2) return mapReadErr;
		if (offset_name_list < 30) return mapReadErr;
		if (offset_type_list > rf.length_rmap) return mapReadErr;
		if (offset_name_list > rf.length_rmap) return mapReadErr;


		try {

			unsigned tl_offset = offset_type_list + 2;
			check_size(rmap, tl_offset + (count + 1) * 8);

			while (count--) {

				struct resource r;

				r.type = read_32(rmap, tl_offset + 0, std::nothrow);
				int count = (read_16(rmap, tl_offset + 4, std::nothrow) + 1) & 0xffff;
				unsigned rl_offset = offset_type_list + read_16(rmap, tl_offset + 6, std::nothrow);

				check_size(rmap, rl_offset + (count + 1) * 12);

				while (count--) {

					r.id = read_16(rmap, rl_offset + 0, std::nothrow);

					unsigned nl_offset = read_16(rmap, rl_offset + 2, std::nothrow);
					r.attr = read_8(rmap, rl_offset + 4, std::nothrow);
					r.data_offset = read_24(rmap, rl_offset + 5, std::nothrow);
					// 4 bytes reserved.

					r.name.clear();
					if (nl_offset != 0xffff) {
						r.name = read_string(rmap, nl_offset + offset_name_list);
					}


					rf.resources.emplace_back(std::move(r));

					rl_offset += 12;
				}

				tl_offset += 8;
			}

		} catch (std::out_of_range &ex) {
			return mapReadErr;
		}

		std::sort(rf.resources.begin(), rf.resources.end());
		return noErr;
	}

	tool_return<uint32_t> read_resource(resource_file &rf, resource &r) {

		ssize_t ok;

		if (r.handle) return r.handle;

		if (!res_load) {
			/* create a null handle */
			auto hh = MM::Native::NewHandle();
			if (hh.error()) return hh.error();
			r.handle = *hh;
			return *hh;
		}

		if (r.size == -1) {
			uint8_t buffer[4];
			if (lseek(rf.fd, rf.offset_rdata + r.data_offset, SEEK_SET) < 0)
				return macos_error_from_errno();
			ok = read(rf.fd, buffer, sizeof(buffer));
			if (ok != 4) return badExtResource;

			r.size = read_32(buffer);
		} else {

			if (lseek(rf.fd, rf.offset_rdata + r.data_offset + 4, SEEK_SET) < 0)
				return macos_error_from_errno();

		}

		auto hh = MM::Native::NewHandle(r.size, false);
		if (hh.error()) return hh.error();
		MM::Native::HSetRBit(hh->handle);

		ok = read(rf.fd, memoryPointer(hh->pointer), r.size);
		if (ok < 0) {
			auto rv = macos_error_from_errno();
			MM::Native::DisposeHandle(hh->handle);
			return rv;
		}

		if (ok != r.size) {
			MM::Native::DisposeHandle(hh->handle);
			return badExtResource;
		}

		r.handle = hh->handle;
		return hh->handle;
	}

	inline macos_error SetResError(uint16_t error)
	{
		memoryWriteWord(error, MacOS::ResErr);
		return (macos_error)error;
	}

};


namespace RM {
#pragma mark - Native

namespace Native {

	tool_return<int16_t> OpenResFile(const std::string &path_name, uint16_t perm) {

		int fd = native::open_resource_fork(path_name, O_RDONLY);
		if (fd < 0) return macos_error_from_errno();

		auto rf = std::make_unique<resource_file>();

		rf->fd = fd;
		rf->attr |= mapReadOnly;
		auto rv = read_resource_map(*rf);
		if (rv.error()) return rv.error();

		rf->refnum = ++refnum;

		rf->next = head;
		head = rf.release();
		current = head;

		return refnum;
	}


	tool_return<void> UpdateResFile(resource_file &rf)
	{
		return noErr;
	}



	tool_return<void> CloseResFile(uint16_t refNum) {

		resource_file *rf = head;
		resource_file *prev = nullptr;
		while (rf) {
			if (rf->refnum == refNum) break;
			prev = rf;
			rf = rf->next;
		}
		if (!rf) return SetResError(MacOS::resFNotFound);

		auto rv = UpdateResFile(*rf);

		for (auto &r : rf->resources) {
			if (r.handle) MM::Native::DisposeHandle(r.handle);
		}

		if (prev) prev->next = rf->next;
		else head = rf->next;

		if (current == rf) current = rf->next;

		delete rf;

		return rv.error();
	}

	tool_return<void> SetResLoad(bool load)
	{
		res_load = load;
		memoryWriteByte(load ? 0xff : 0x00, MacOS::ResLoad); // word or byte?
		return SetResError(MacOS::noErr);
	}


	tool_return<uint32_t> Get1Resource(resource_file &rf, uint32_t type, uint16_t id) {

		auto &resources = rf.resources;
		auto iter = std::lower_bound(resources.begin(), resources.end(), 0,
			[type,id](const resource &lhs, int rhs){
				if (lhs.type < type) return true;
				if (lhs.type == type && lhs.id < id) return true;
				return false;
		});

		if (iter == resources.end())
			return SetResError(MacOS::resNotFound);

		auto &r = *iter;
		if (r.type != type || r.id != id)
			return SetResError(MacOS::resNotFound);

		auto rv = read_resource(rf, r);
		SetResError(rv.error());
		return rv;
	}


	tool_return<uint32_t> Get1NamedResource(resource_file &rf, uint32_t type, const std::string &name) {

		auto &resources = rf.resources;
		auto iter = std::lower_bound(resources.begin(), resources.end(), 0,
			[type](const resource &lhs, int rhs){
				if (lhs.type < type) return true;
				return false;
		});

		if (iter == resources.end())
			return SetResError(MacOS::resNotFound);

		for (; iter != resources.end(); ++iter) {
			auto &r = *iter;
			if (r.type != type) break;
			if (r.name == name) {
				auto rv = read_resource(rf, r);
				SetResError(rv.error());
				return rv;
			}
		}
		return SetResError(MacOS::resNotFound);
	}


	tool_return<uint32_t> Get1Resource(uint32_t type, uint16_t id) {

		resource_file *rf = current;
		if (rf) return Get1Resource(*rf, type, id);
		return SetResError(MacOS::resNotFound);
	}

	tool_return<uint32_t> Get1NamedResource(uint32_t type, const std::string &name) {

		resource_file *rf = current;
		if (rf) return Get1NamedResource(*rf, type, name);
		return SetResError(MacOS::resNotFound);
	}

	tool_return<uint32_t> GetResource(uint32_t type, uint16_t id) {

		for (auto rf = current; rf; rf = rf->next) {
			auto rv = Get1Resource(*rf, type, id);
			if (!rv.error()) return rv;
		}
		return SetResError(MacOS::resNotFound);
	}


	tool_return<uint32_t> GetNamedResource(uint32_t type, const std::string &name) {

		for (auto rf = current; rf; rf = rf->next) {
			auto rv = Get1NamedResource(*rf, type, name);
			if (!rv.error()) return rv;
		}
		return SetResError(MacOS::resNotFound);
	}




	tool_return<void> CreateResFile(const std::string &path, uint32_t creator, uint32_t fileType)
	{
		if (path.empty()) return MacOS::paramErr;

		fprintf(stderr, "%s : not yet implemented!\n", __func__);
		exit(1);
		return noErr;
	}


} // Native


	uint16_t CurResFile(uint16_t trap)
	{

		Log("%04x CurResFile()\n", trap);

		uint16_t refNum = 0xffff;
		if (current) refNum = current->refnum;

		ToolReturn<2>(-1, refNum);
		return SetResError(refNum == 0xffff ? resFNotFound : noErr);
	}


	uint16_t UseResFile(uint16_t trap)
	{
		uint16_t refNum;

		StackFrame<2>(refNum);

		Log("%04x UseResFile(%04x)\n", trap, refNum);

		// todo -- 0 is the system resource file.

		resource_file *rf;
		for (rf = head; rf; rf = rf->next) {
			if (rf->refnum == refNum) break;
		}
		if (rf) current = rf;

		return SetResError(rf ? noErr : resFNotFound);
	}



	uint16_t GetResInfo(uint16_t trap)
	{
		// PROCEDURE GetResInfo (theResource: Handle;
		// VAR theID: INTEGER; VAR theType: ResType; VAR name: Str255);

		uint32_t theResource;
		uint32_t theID;
		uint32_t theType;
		uint32_t name;
		StackFrame<16>(theResource, theID, theType, name);

		Log("%04x GetResInfo(%08x)\n", trap, theResource);

		if (!theResource) return SetResError(resNotFound);

		for (resource_file *rf = head; rf; rf = rf->next) {
			for (const auto &r : rf->resources) {

				if (r.handle == theResource) {
					if (theID) memoryWriteWord(r.id, theID);
					if (theType) memoryWriteLong(r.type, theType);
					if (name) ToolBox::WritePString(name, r.name);

					return SetResError(noErr);
				}

			}
		}
		return SetResError(resNotFound);
	}


	uint16_t GetResAttrs(uint16_t trap)
	{
		// FUNCTION GetResAttrs (theResource: Handle): Integer;

		uint32_t sp;
		uint32_t theResource;

		sp = StackFrame<4>(theResource);

		Log("%04x GetResAttrs(%08x)\n", trap, theResource);


		if (!theResource) return SetResError(resNotFound);

		for (resource_file *rf = head; rf; rf = rf->next) {
			for (const auto &r : rf->resources) {

				if (r.handle == theResource) {
					ToolReturn<2>(sp, r.attr);
					return SetResError(noErr);
				}

			}
		}
		return SetResError(resNotFound);
	}


	uint16_t SetResAttrs(uint16_t trap)
	{
		// PROCEDURE SetResAttrs (theResource: Handle; attrs: Integer);

		uint32_t theResource;
		uint16_t attrs;

		StackFrame<6>(theResource, attrs);

		Log("%04x SetResAttrs(%08x, %04x)\n", trap, theResource, attrs);

		if (!theResource) return SetResError(resNotFound);

		for (resource_file *rf = head; rf; rf = rf->next) {
			for (auto &r : rf->resources) {

				if (r.handle == theResource) {
					if (r.attr & resChanged) attrs |= resChanged;
					r.attr = attrs;
					return SetResError(noErr);
				}

			}
		}
		return SetResError(resNotFound);
	}


	uint16_t GetResFileAttrs(uint16_t trap)
	{
		// FUNCTION GetResFileAttrs (refNum: Integer): Integer;
		uint32_t sp;
		uint16_t refNum;

		sp = StackFrame<2>(refNum);
		Log("%04x GetResFileAttrs(%04x)\n", trap, refNum);

		for (auto rf = head ; rf; rf = rf->next) {
			if (rf->refnum == refNum) {
				ToolReturn<2>(sp, rf->attr);
				return SetResError(noErr);
			}
		}

		return SetResError(resFNotFound);
	}


	uint16_t SetResFileAttrs(uint16_t trap)
	{
		// PROCEDURE SetResFileAttrs  (refNum: Integer; attrs: Integer);
		uint32_t sp;
		uint16_t attrs;
		uint16_t refNum;

		sp = StackFrame<4>(refNum, attrs);
		Log("%04x GetResFileAttrs(%04x, %04x)\n", trap, refNum, attrs);


		for (auto rf = head ; rf; rf = rf->next) {
			if (rf->refnum == refNum) {
				// don't clear the read-only flag [???]
				if (rf->attr & mapReadOnly) attrs |= mapReadOnly;
				rf->attr = attrs;
				return SetResError(noErr);
			}
		}

		return SetResError(resFNotFound);
	}




	uint16_t DetachResource(uint16_t trap)
	{
		// PROCEDURE DetachResource (theResource: Handle);

		uint32_t theResource;
		StackFrame<4>(theResource);

		Log("%04x DetachResource(%08x)\n", trap, theResource);

		if (!theResource) return SetResError(resNotFound);

		for (resource_file *rf = head; rf; rf = rf->next) {
			for (auto &r : rf->resources) {

				if (r.handle == theResource) {
					/*
					 * DetachResource won't let you detach a resource whose resChanged
					 * attribute has been set; however, ResError will still return noErr.
					 */
					if ((r.attr & resChanged) == 0) {
						r.handle = 0;
						MM::Native::HClrRBit(theResource);
					}
					return SetResError(noErr);
				}
			}
		}
		return SetResError(resNotFound);
	}


	uint16_t ReleaseResource(uint16_t trap)
	{
		// ReleaseResource (theResource: Handle);

		/*
		 * ------------
		 * +0 theResource
		 * ------------
		 *
		 */

		uint32_t sp;
		uint32_t theResource;

		sp = StackFrame<4>(theResource);

		Log("%04x ReleaseResource(%08x)\n", trap, theResource);

		if (!theResource) return SetResError(resNotFound);

		for (resource_file *rf = head; rf; rf = rf->next) {
			for (auto &r : rf->resources) {

				if (r.handle == theResource) {
					/*
					 * ReleaseResource won't let you release a resource whose resChanged
					 * attribute has been set; however, ResError will still return noErr.
					 */
					if ((r.attr & resChanged) == 0) {
						MM::Native::DisposeHandle(theResource);
						r.handle = 0;
					}
					return SetResError(noErr);
				}
			}
		}
		return SetResError(resNotFound);
	}



	uint16_t HomeResFile(uint16_t trap)
	{
		// PPCAsm

		// FUNCTION HomeResFile (theResource: Handle): Integer;

		uint32_t sp;
		uint32_t theResource;

		sp = StackFrame<4>(theResource);
		Log("%04x HomeResFile(%08x)\n", trap, theResource);

		if (!theResource) {
			ToolReturn<2>(sp, -1);
			return SetResError(resNotFound);
		}

		for (resource_file *rf = head; rf; rf = rf->next) {
			for (const auto &r : rf->resources) {

				if (r.handle == theResource) {
					ToolReturn<2>(sp, rf->refnum);
					return SetResError(noErr);
				}

			}
		}



		ToolReturn<2>(sp, -1);
		return SetResError(resNotFound);
	}

	uint16_t ResError(uint16_t trap)
	{
		uint32_t sp;

		Log("%04x ResError()\n", trap);

		sp = cpuGetAReg(7);
		ToolReturn<2>(sp, memoryReadWord(MacOS::ResErr));
		return 0;
	}


	// SetResLoad (load: BOOLEAN);
	uint16_t SetResLoad(uint16_t trap)
	{
		uint16_t load;

		StackFrame<2>(load);

		Log("%04x SetResLoad(%04x)\n", trap, load);

		res_load = load;

		memoryWriteByte(load ? 0xff : 0x00, MacOS::ResLoad); // word or byte?
		return SetResError(0);
	}





#pragma mark - Open Resource Files

	uint16_t OpenResFile(uint16_t trap)
	{
		// OpenResFile (fileName: Str255) : INTEGER;

		uint32_t sp;
		uint32_t fileName;

		sp = StackFrame<4>(fileName);

		std::string sname = ToolBox::ReadPString(fileName, true);

		Log("%04x OpenResFile(%s)\n", trap, sname.c_str());

		auto rv = Native::OpenResFile(sname);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
	}

	uint16_t HOpenResFile(uint16_t trap)
	{
		// FUNCTION HOpenResFile (vRefNum: Integer; dirID: LongInt;
		// fileName: Str255; permission: SignedByte): Integer;

		uint32_t sp;

		uint16_t vRefNum;
		uint32_t dirID;
		uint32_t fileName;
		uint8_t permission;

		sp = StackFrame<12>(vRefNum, dirID, fileName, permission);

		std::string sname = ToolBox::ReadPString(fileName, true);

		Log("%04x HOpenResFile(%04x, %08x, %s, %04x)\n",
			trap, vRefNum, dirID, sname.c_str(), permission);

		if (vRefNum) {
			fprintf(stderr, "HOpenResFile: vRefNum not supported yet.\n");
			exit(1);
		}

		sname = OS::FSSpecManager::ExpandPath(sname, dirID);
		if (sname.empty())
		{
			ToolReturn<2>(sp, (uint16_t)-1);
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = Native::OpenResFile(sname, permission);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
	}

	uint16_t OpenRFPerm(uint16_t trap)
	{
		// FUNCTION OpenRFPerm (fileName: Str255; vRefNum: Integer;
        //           permission: SignedByte): Integer;
		uint32_t sp;
		uint32_t fileName;
		uint16_t vRefNum;
		uint16_t permission;

		sp = StackFrame<8>(fileName, vRefNum, permission);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("%04x OpenRFPerm(%s, %04x, %04x)\n",
			trap, sname.c_str(), vRefNum, permission);

		auto rv = Native::OpenResFile(sname, permission);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
	}

	uint16_t FSpOpenResFile(void)
	{
		// FUNCTION FSpOpenResFile (spec: FSSpec; permission: SignedByte): Integer;

		uint32_t sp;
		uint32_t spec;
		uint8_t permission;

		sp = StackFrame<6>(spec, permission);


		int parentID = memoryReadLong(spec + 2);

		std::string sname = ToolBox::ReadPString(spec + 6, false);

		Log("     FSpOpenResFile(%s, %04x)\n",  sname.c_str(), permission);


		sname = OS::FSSpecManager::ExpandPath(sname, parentID);
		if (sname.empty())
		{
			ToolReturn<2>(sp, (uint16_t)-1);
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = Native::OpenResFile(sname, permission);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
	}

	uint16_t CloseResFile(uint16_t trap)
	{
		// PROCEDURE CloseResFile (refNum: Integer);
		uint16_t refNum;

		StackFrame<2>(refNum);

		Log("%04x CloseResFile(%04x)\n", trap, refNum);

		// If the value of the refNum parameter is 0, it represents the System file and is ignored.
		// n.b. -- should close --all-- resources.

		auto rv = Native::CloseResFile(refNum);
		return SetResError(rv.error());
	}


	uint16_t Get1NamedResource(uint16_t trap)
	{
		// Get1NamedResource (theType: ResType; name: Str255) : Handle;

		/*
		 * -----------
		 * +8 outHandle
		 * ------------
		 * +4 theType
		 * ------------
		 * +0 name
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint32_t name;

		sp = StackFrame<8>(theType, name);

		std::string sname = ToolBox::ReadPString(name);

		Log("%04x Get1NamedResource(%08x ('%s'), %s)\n",
			trap, theType, TypeToString(theType).c_str(), sname.c_str());

		auto rv = Native::Get1NamedResource(theType, sname);

		ToolReturn<4>(sp, rv.value());
		return SetResError(rv.error());
	}

	uint16_t GetNamedResource(uint16_t trap)
	{
		// FUNCTION GetNamedResource (theType: ResType; name: Str255): Handle;

		uint32_t sp;
		uint32_t theType;
		uint32_t name;

		sp = StackFrame<8>(theType, name);

		std::string sname = ToolBox::ReadPString(name);

		Log("%04x GetNamedResource(%08x ('%s'), %s)\n",
			trap, theType, TypeToString(theType).c_str(), sname.c_str());

		auto rv = Native::GetNamedResource(theType, sname);

		ToolReturn<4>(sp, rv.value());
		return SetResError(rv.error());
	}

	uint16_t GetResource(uint16_t trap)
	{
		// GetResource (theType: ResType; theID: Integer): Handle;

		/*
		 * -----------
		 * +6 outHandle
		 * ------------
		 * +2 theType
		 * ------------
		 * +0 theID
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint16_t theID;

		sp = StackFrame<6>(theType, theID);

		Log("%04x GetResource(%08x ('%s'), %04x)\n",
				trap, theType, TypeToString(theType).c_str(), theID);

		auto rv = Native::GetResource(theType, theID);

		ToolReturn<4>(sp, rv.value());
		return SetResError(rv.error());
	}


	uint16_t Get1Resource(uint16_t trap)
	{
		// Get1Resource (theType: ResType; theID: Integer): Handle;

		/*
		 * -----------
		 * +6 outHandle
		 * ------------
		 * +2 theType
		 * ------------
		 * +0 theID
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint16_t theID;

		sp = StackFrame<6>(theType, theID);

		Log("%04x Get1Resource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);


		auto rv = Native::Get1Resource(theType, theID);

		ToolReturn<4>(sp, rv.value());
		return SetResError(rv.error());
	}


	uint16_t ChangedResource(uint16_t trap)
	{
		// PROCEDURE ChangedResource (theResource: Handle);

		uint32_t theResource;

		StackFrame<4>(theResource);

		Log("%04x ChangedResource(%08x)\n", trap, theResource);


		if (!theResource) return SetResError(resNotFound);

		for (auto rf = head; rf; rf = rf->next) {
			for (auto &r : rf->resources) {
				if (r.handle == theResource) {
					if (rf->attr & mapReadOnly) return wPrErr; // ?
					if (r.attr & resProtected) return noErr;
					r.attr |= resChanged;
					rf->attr |= mapChanged;

					// inside macintosh says it also extends the file size.
					return noErr;
				}
			}
		}
		return SetResError(resNotFound);
	}



#pragma mark - Create Resource Files.


	uint16_t CreateResFile(uint16_t trap)
	{
		// PROCEDURE CreateResFile (fileName: Str255);

		/*
		 * CreateResFile creates a resource file containing no resource
		 * data. If there's no file at all with the given name, it also
		 * creates an empty data fork for the file. If there's already a
		 * resource file with the given name (that is, a resource fork
		 * that isn't empty), CreateResFile will do nothing and the
		 * ResError function will return an appropriate Operating System
		 * result code.
		 */

		uint32_t fileName;

		StackFrame<4>(fileName);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("%04x CreateResFile(%s)\n", trap, sname.c_str());

		if (!sname.length()) return SetResError(MacOS::paramErr);


		auto rv = Native::CreateResFile(sname);

		return SetResError(rv.error());
	}

	uint16_t HCreateResFile(uint16_t trap)
	{
		// PROCEDURE HCreateResFile (vRefNum: Integer; dirID: LongInt;
		//                           fileName: Str255);

		uint16_t vRefNum;
		uint32_t dirID;
		uint32_t fileName;

		StackFrame<10>(vRefNum, dirID, fileName);

		std::string sname = ToolBox::ReadPString(fileName, true);

		Log("%04x HCreateResFile(%04x, %08x, %s)\n",
			trap, vRefNum, dirID, sname.c_str());


		sname = OS::FSSpecManager::ExpandPath(sname, dirID);
		if (sname.empty())
		{
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = Native::CreateResFile(sname);

		return SetResError(rv.error() == MacOS::dupFNErr ? 0 : rv.error());
	}


	uint16_t FSpCreateResFile(void)
	{

		// PROCEDURE FSpCreateResFile (spec: FSSpec; creator, fileType: OSType; scriptTag: ScriptCode);

		// creates the file, if necessary.

		uint32_t sp;
		uint32_t spec;
		uint32_t creator;
		uint32_t fileType;
		uint16_t scriptTag;

		sp = StackFrame<14>(spec, creator, fileType, scriptTag);


		int parentID = memoryReadLong(spec + 2);
		std::string sname = ToolBox::ReadPString(spec + 6, false);

		Log("     FSpCreateResFile(%s, %08x ('%s'), %08x ('%s'), %02x)\n",
			sname.c_str(),
			creator, ToolBox::TypeToString(creator).c_str(),
			fileType, ToolBox::TypeToString(fileType).c_str(),
			scriptTag);


		sname = OS::FSSpecManager::ExpandPath(sname, parentID);
		if (sname.empty())
		{
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = Native::CreateResFile(sname, creator, fileType);

		return SetResError(rv.error() == MacOS::dupFNErr ? 0 : rv.error());
	}

#pragma mark - Not Yet Implemented.


	uint16_t AddResource(uint16_t trap)
	{
		// PROCEDURE AddResource (theData: Handle; theType: ResType;
		// theID: Integer; name: Str255);

		uint32_t theData;
		uint32_t theType;
		uint16_t theID;
		uint32_t namePtr;


		StackFrame<14>(theData, theType, theID, namePtr);

		std::string sname = ToolBox::ReadPString(namePtr, false);

		Log("%04x AddResource(%08x, %08x ('%s'), %04x, %s)\n",
			trap, theData, theType, TypeToString(theType).c_str(), theID, sname.c_str()
		);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}


	uint16_t LoadResource(uint16_t trap)
	{
		// PROCEDURE LoadResource (theResource: Handle);

		// this loads the resource from disk, if not already
		// loaded. (if purgeable or SetResLoad(false))

		// this needs cooperation with MM to check if
		// handle was purged.

		uint32_t theResource;

		StackFrame<4>(theResource);

		Log("%04x LoadResource(%08x)\n", trap, theResource);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}

	uint16_t UpdateResFile(uint16_t trap)
	{
		// PROCEDURE UpdateResFile (refNum: Integer);

		uint16_t refNum;

		StackFrame<2>(refNum);

		Log("%04x UpdateResFile(%04x)\n", trap, refNum);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}

	uint16_t WriteResource(uint16_t trap)
	{
		// PROCEDURE WriteResource (theResource: Handle);

		uint32_t theResource;
		StackFrame<4>(theResource);

		Log("%04x WriteResource(%08x)\n", trap, theResource);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}

	uint16_t RemoveResource(uint16_t trap)
	{
		// PROCEDURE RemoveResource (theResource: Handle);

		uint32_t theResource;

		StackFrame<4>(theResource);

		Log("%04x RemoveResource(%08x)\n", trap, theResource);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}

	uint16_t GetResourceSizeOnDisk(uint16_t trap)
	{
		// FUNCTION GetResourceSizeOnDisk (theResource: Handle): LongInt;

		uint32_t sp;
		uint32_t theResource;

		sp = StackFrame<4>(theResource);

		Log("%04x GetResourceSizeOnDisk(%08x)\n", trap, theResource);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}


	uint16_t Count1Resources(uint16_t trap)
	{
		// FUNCTION Count1Resources (theType: ResType): Integer;

		uint32_t sp;
		uint32_t theType;

		sp = StackFrame<4>(theType);

		Log("%04x Count1Resources(%08x ('%s'))\n",
			trap, theType, TypeToString(theType).c_str());

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}

	uint16_t Get1IndResource(uint16_t trap)
	{
		// FUNCTION Get1IndResource (theType: ResType; index: Integer): Handle;

		uint32_t sp;
		uint32_t theType;
		uint16_t index;

		sp = StackFrame<6>(theType, index);
		Log("%04x Get1IndResource(%08x ('%s'), %04x)\n",
			trap, theType, TypeToString(theType).c_str(), index);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}


	uint16_t Count1Types(uint16_t trap)
	{
		// FUNCTION Count1Types: Integer;

		Log("%04x Count1Types\n", trap);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}


	uint16_t Get1IndType(uint16_t trap)
	{
		// PROCEDURE Get1IndType (VAR theType: ResType; index: Integer);

		uint32_t theType;
		uint16_t index;

		StackFrame<6>(theType, index);

		Log("%04x Get1IndType(%08x, %04x)\n", trap, theType, index);

		fprintf(stderr, "%s not yet implemented\n", __func__);
		exit(1);
		return SetResError(0);
	}

} // RM




