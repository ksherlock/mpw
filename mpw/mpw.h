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

	// from MPW's FCntl.h
	enum
	{
		kF_OPEN = (('d' << 8)| 0x00),
		kF_DELETE = (('d' << 8) | 0x01),
		kF_RENAME = (('d' << 8) | 0x02),
	};

	// from MPW's IOCtl.h
	enum
	{
		kFIOLSEEK = (('f' << 8) | 0x00),		/* Apple internal use only */
		kFIODUPFD = (('f' << 8) | 0x01),		/* Apple internal use only */
		kFIOINTERACTIVE = (('f' << 8) | 0x02),	/* If device is interactive */
		kFIOBUFSIZE = (('f' << 8) | 0x03),		/* Return optimal buffer size */
		kFIOFNAME = (('f' << 8) | 0x04),		/* Return filename */
		kFIOREFNUM = (('f' << 8) | 0x05),		/* Return fs refnum */
		kFIOSETEOF = (('f' << 8) | 0x06),		/* Set file length */
	};

	enum
	{
		kSEEK_CUR = 1,
		kSEEK_END = 2,
		kSEEK_SET = 0,
	};

	enum
	{
		// despite these constants, 0x01 seems to be read, 0x02 is write, 0x03 is read/write.
		kO_RDONLY = 0,			/* Bits 0 and 1 are used internally */
		kO_WRONLY = 1, 			/* Values 0..2 are historical */
		kO_RDWR = 2,			/* NOTE: it goes 0, 1, 2, *!* 8, 16, 32, ... */
		kO_APPEND = (1 << 3),	/* append (writes guaranteed at the end) */
		kO_RSRC = (1 << 4),		/* Open the resource fork */
		kO_CREAT = (1 << 8),	/* Open with file create */
		kO_TRUNC = (1 << 9),	/* Open with truncation */
		kO_EXCL = (1 << 10), 	/* w/ O_CREAT Exclusive "create-only" */
		kO_BINARY = (1 << 11), 	/* Open as a binary stream */
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