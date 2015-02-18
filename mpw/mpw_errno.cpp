/*
 * Copyright (c) 2014, Kelvin W Sherlock
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
#include <system_error>
#include <cerrno>

#include "mpw_errno.h"

namespace MPW {

	namespace {


		class __system_category : public std::error_category
		{
		public:
			virtual const char* name() const noexcept;
			virtual std::string message(int ev) const;
    		virtual std::error_condition default_error_condition(int ev) const noexcept;

		private:
			static int remap(mpw_errno e);

		};


		const char *__system_category::name() const noexcept
		{
			return "mpw/errno";
		}

		std::string __system_category::message(int ev) const
		{

			switch (ev)
			{
			case kEPERM: return "Permission denied";
			case kENOENT: return "No such file or directory";
			case kENORSRC: return "No such resource";
			case kEINTR: return "Interrupted system service";
			case kEIO: return "I/O error";
			case kENXIO: return "No such device or address";
			case kE2BIG: return "Argument list too long";
			case kENOEXEC: return "Exec format error";
			case kEBADF: return "Bad file number";
			case kECHILD: return "No children processes";
			case kEAGAIN: return "Resource temporarily unavailable, try again later";
			case kENOMEM: return "Not enough space";
			case kEACCES: return "Permission denied";
			case kEFAULT: return "Bad address";
			case kENOTBLK: return "Block device required";
			case kEBUSY: return "Device or resource busy";
			case kEEXIST: return "File exists";
			case kEXDEV: return "Cross-device link";
			case kENODEV: return "No such device";
			case kENOTDIR: return "Not a directory";
			case kEISDIR: return "Is a directory";
			case kEINVAL: return "Invalid argument";
			case kENFILE: return "File table overflow";
			case kEMFILE: return "Too many open files";
			case kENOTTY: return "Not a character device";
			case kETXTBSY: return "Text file busy";
			case kEFBIG: return "File too large";
			case kENOSPC: return "No space left on device";
			case kESPIPE: return "Illegal seek";
			case kEROFS: return "Read only file system";
			case kEMLINK: return "Too many links";
			case kEPIPE: return "Broken pipe";
			case kEDOM: return "Math arg out of domain of func";
			case kERANGE: return "Math result not representable";
			}

			std::string tmp("Unknown error: ");
			tmp += std::to_string(ev);
			return tmp;
		}

    	std::error_condition __system_category::default_error_condition(int ev) const noexcept
    	{

    		int x = remap(static_cast<mpw_errno>(ev));
    		if (x >= 0) return std::error_condition(x, std::generic_category());

    		return std::error_condition(ev, mpw_system_category());
    	}

		int __system_category::remap(mpw_errno e)
		{
			switch(e)
			{
			case kEPERM: return EPERM;
			case kENOENT: return ENOENT;
			#ifdef ENORSRC
			case kENORSRC: return ENORSRC;
			#endif
			case kEINTR: return EINTR;
			case kEIO: return EIO;
			case kENXIO: return ENXIO;
			case kE2BIG: return E2BIG;
			case kENOEXEC: return ENOEXEC;
			case kEBADF: return EBADF;
			case kECHILD: return ECHILD;
			case kEAGAIN: return EAGAIN;
			case kENOMEM: return ENOMEM;
			case kEACCES: return EACCES;
			case kEFAULT: return EFAULT;
			case kENOTBLK: return ENOTBLK;
			case kEBUSY: return EBUSY;
			case kEEXIST: return EEXIST;
			case kEXDEV: return EXDEV;
			case kENODEV: return ENODEV;
			case kENOTDIR: return ENOTDIR;
			case kEISDIR: return EISDIR;
			case kEINVAL: return EINVAL;
			case kENFILE: return ENFILE;
			case kEMFILE: return EMFILE;
			case kENOTTY: return ENOTTY;
			case kETXTBSY: return ETXTBSY;
			case kEFBIG: return EFBIG;
			case kENOSPC: return ENOSPC;
			case kESPIPE: return ESPIPE;
			case kEROFS: return EROFS;
			case kEMLINK: return EMLINK;
			case kEPIPE: return EPIPE;
			case kEDOM: return EDOM;
			case kERANGE: return ERANGE;

			default: return -1;
			}

		}

	}

	const std::error_category& mpw_system_category()
	{
		static __system_category s;
		return s;
	}


	mpw_errno mpw_errno_from_errno()
	{
		return mpw_errno_from_errno(errno);
	}

	mpw_errno mpw_errno_from_errno(int error)
	{

		switch(error)
		{
		case 0: return static_cast<mpw_errno>(0);

		case EPERM: return kEPERM;
		case ENOENT: return kENOENT;

		#ifdef ENORSRC
		case ENORSRC: return kENORSRC;
		#endif

		case EINTR: return kEINTR;
		case EIO: return kEIO;
		case ENXIO: return kENXIO;
		case E2BIG: return kE2BIG;
		case ENOEXEC: return kENOEXEC;
		case EBADF: return kEBADF;
		case ECHILD: return kECHILD;
		case EAGAIN: return kEAGAIN;
		case ENOMEM: return kENOMEM;
		case EACCES: return kEACCES;
		case EFAULT: return kEFAULT;
		case ENOTBLK: return kENOTBLK;
		case EBUSY: return kEBUSY;
		case EEXIST: return kEEXIST;
		case EXDEV: return kEXDEV;
		case ENODEV: return kENODEV;
		case ENOTDIR: return kENOTDIR;
		case EISDIR: return kEISDIR;
		case EINVAL: return kEINVAL;
		case ENFILE: return kENFILE;
		case EMFILE: return kEMFILE;
		case ENOTTY: return kENOTTY;
		case ETXTBSY: return kETXTBSY;
		case EFBIG: return kEFBIG;
		case ENOSPC: return kENOSPC;
		case ESPIPE: return kESPIPE;
		case EROFS: return kEROFS;
		case EMLINK: return kEMLINK;
		case EPIPE: return kEPIPE;
		case EDOM: return kEDOM;
		case ERANGE: return kERANGE;

		default:
			return kEINVAL;
		}


	}


}
