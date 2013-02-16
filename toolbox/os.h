#ifndef __mpw_os_h__
#define __mpw_os_h__

#include <cstdint>
#include <string>

namespace OS
{

	enum {
		paramErr = -50,                                 /*error in user parameter list*/
		noHardwareErr = -200,                           /*Sound Manager Error Returns*/
		notEnoughHardwareErr = -201,                    /*Sound Manager Error Returns*/
		userCanceledErr = -128,
		qErr = -1,                                      /*queue element not found during deletion*/
		vTypErr = -2,                                   /*invalid queue element*/
		corErr = -3,                                    /*core routine number out of range*/
		unimpErr = -4,                                  /*unimplemented core routine*/
		SlpTypeErr = -5,                                /*invalid queue element*/
		seNoDB = -8,                                    /*no debugger installed to handle debugger command*/
		controlErr = -17,                               /*I/O System Errors*/
		statusErr = -18,                                /*I/O System Errors*/
		readErr = -19,                                  /*I/O System Errors*/
		writErr = -20,                                  /*I/O System Errors*/
		badUnitErr = -21,                               /*I/O System Errors*/
		unitEmptyErr = -22,                             /*I/O System Errors*/
		openErr = -23,                                  /*I/O System Errors*/
		closErr = -24,                                  /*I/O System Errors*/
		dRemovErr = -25,                                /*tried to remove an open driver*/
		dInstErr = -26                                  /*DrvrInstall couldn't find driver in resources */
	};

	enum {
		abortErr = -27,                                 /*IO call aborted by KillIO*/
		iIOAbortErr = -27,                              /*IO abort error (Printing Manager)*/
		notOpenErr = -28,                               /*Couldn't rd/wr/ctl/sts cause driver not opened*/
		unitTblFullErr = -29,                           /*unit table has no more entries*/
		dceExtErr = -30,                                /*dce extension error*/
		slotNumErr = -360,                              /*invalid slot # error*/
		gcrOnMFMErr = -400,                             /*gcr format on high density media error*/
		dirFulErr = -33,                                /*Directory full*/
		dskFulErr = -34,                                /*disk full*/
		nsvErr = -35,                                   /*no such volume*/
		ioErr = -36,                                    /*I/O error (bummers)*/
		bdNamErr = -37,                                 /*there may be no bad names in the final system!*/
		fnOpnErr = -38,                                 /*File not open*/
		eofErr = -39,                                   /*End of file*/
		posErr = -40,                                   /*tried to position to before start of file (r/w)*/
		mFulErr = -41,                                  /*memory full (open) or file won't fit (load)*/
		tmfoErr = -42,                                  /*too many files open*/
		fnfErr = -43,                                   /*File not found*/
		wPrErr = -44,                                   /*diskette is write protected.*/
		fLckdErr = -45                                  /*file is locked*/
	};

	enum {
		vLckdErr = -46,                                 /*volume is locked*/
		fBsyErr = -47,                                  /*File is busy (delete)*/
		dupFNErr = -48,                                 /*duplicate filename (rename)*/
		opWrErr = -49,                                  /*file already open with with write permission*/
		rfNumErr = -51,                                 /*refnum error*/
		gfpErr = -52,                                   /*get file position error*/
		volOffLinErr = -53,                             /*volume not on line error (was Ejected)*/
		permErr = -54,                                  /*permissions error (on file open)*/
		volOnLinErr = -55,                              /*drive volume already on-line at MountVol*/
		nsDrvErr = -56,                                 /*no such drive (tried to mount a bad drive num)*/
		noMacDskErr = -57,                              /*not a mac diskette (sig bytes are wrong)*/
		extFSErr = -58,                                 /*volume in question belongs to an external fs*/
		fsRnErr = -59,                                  /*file system internal error:during rename the old entry was deleted but could not be restored.*/
		badMDBErr = -60,                                /*bad master directory block*/
		wrPermErr = -61,                                /*write permissions error*/
		dirNFErr = -120,                                /*Directory not found*/
		tmwdoErr = -121,                                /*No free WDCB available*/
		badMovErr = -122,                               /*Move into offspring error*/
		wrgVolTypErr = -123,                            /*Wrong volume type error [operation not supported for MFS]*/
		volGoneErr = -124                               /*Server volume has been disconnected.*/
	};

	enum {
		fidNotFound = -1300,                            /*no file thread exists.*/
		fidExists = -1301,                              /*file id already exists*/
		notAFileErr = -1302,                            /*directory specified*/
		diffVolErr = -1303,                             /*files on different volumes*/
		catChangedErr = -1304,                          /*the catalog has been modified*/
		desktopDamagedErr = -1305,                      /*desktop database files are corrupted*/
		sameFileErr = -1306,                            /*can't exchange a file with itself*/
		badFidErr = -1307,                              /*file id is dangling or doesn't match with the file number*/
		envNotPresent = -5500,                          /*returned by glue.*/
		envBadVers = -5501,                             /*Version non-positive*/
		envVersTooBig = -5502,                          /*Version bigger than call can handle*/
		fontDecError = -64,                             /*error during font declaration*/
		fontNotDeclared = -65,                          /*font not declared*/
		fontSubErr = -66,                               /*font substitution occured*/
		fontNotOutlineErr = -32615,                     /*bitmap font passed to routine that does outlines only*/
		firstDskErr = -84,                              /*I/O System Errors*/
		lastDskErr = -64,                               /*I/O System Errors*/
		noDriveErr = -64,                               /*drive not installed*/
		offLinErr = -65,                                /*r/w requested for an off-line drive*/
		noNybErr = -66                                  /*couldn't find 5 nybbles in 200 tries*/
	};


	bool IsTextFile(const std::string &s);
	bool IsBinaryFile(const std::string &s);


	uint16_t Create(uint16_t trap);
	uint16_t Delete(uint16_t trap);
	
	uint16_t GetFileInfo(uint16_t trap);
	uint16_t SetFileInfo(uint16_t trap);

	uint16_t GetVol(uint16_t trap);
}

#endif
