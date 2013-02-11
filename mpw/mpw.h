#ifndef __mpw_mpw_h__
#define __mpw_mpw_h__

#include <cstdint>

namespace MPW {


	struct MPWFile
	{
		uint16_t flags;
		uint16_t error;
		uint32_t device;
		uint32_t cookie;
		uint32_t count;
		uint32_t buffer;
	};

	enum {
		fQuit = 0xf000,
		fAccess,
		fClose,
		fRead,
		fWrite,
		fIOCtl,
	};

	void dispatch(uint16_t trap);

}

#endif