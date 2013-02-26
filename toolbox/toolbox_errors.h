/************************************************************

Created: Thursday, March 14, 1991 at 3:53 PM
    Errors.h
    C Interface to the Macintosh Libraries


        Copyright Apple Computer, Inc.    1985-1990
        All rights reserved

************************************************************/


#ifndef __mpw_ERRORS__
#define __mpw_ERRORS__

namespace ToolBox { namespace Errors {

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
enum {
    noAdrMkErr = -67,                               /*couldn't find valid addr mark*/
    dataVerErr = -68,                               /*read verify compare failed*/
    badCksmErr = -69,                               /*addr mark checksum didn't check*/
    badBtSlpErr = -70,                              /*bad addr mark bit slip nibbles*/
    noDtaMkErr = -71,                               /*couldn't find a data mark header*/
    badDCksum = -72,                                /*bad data mark checksum*/
    badDBtSlp = -73,                                /*bad data mark bit slip nibbles*/
    wrUnderrun = -74,                               /*write underrun occurred*/
    cantStepErr = -75,                              /*step handshake failed*/
    tk0BadErr = -76,                                /*track 0 detect doesn't change*/
    initIWMErr = -77,                               /*unable to initialize IWM*/
    twoSideErr = -78,                               /*tried to read 2nd side on a 1-sided drive*/
    spdAdjErr = -79,                                /*unable to correctly adjust disk speed*/
    seekErr = -80,                                  /*track number wrong on address mark*/
    sectNFErr = -81,                                /*sector number never found on a track*/
    fmt1Err = -82,                                  /*can't find sector 0 after track format*/
    fmt2Err = -83,                                  /*can't get enough sync*/
    verErr = -84,                                   /*track failed to verify*/
    clkRdErr = -85,                                 /*unable to read same clock value twice*/
    clkWrErr = -86                                  /*time written did not verify*/
};
enum {
    prWrErr = -87,                                  /*parameter ram written didn't read-verify*/
    prInitErr = -88,                                /*InitUtil found the parameter ram uninitialized*/
    rcvrErr = -89,                                  /*SCC receiver error (framing; parity; OR)*/
    breakRecd = -90,                                /*Break received (SCC)*/

/*Power Manager Errors*/
    pmBusyErr = -13000,                             /*Power Mgr never ready to start handshake*/
    pmReplyTOErr = -13001,                          /*Timed out waiting for reply*/
    pmSendStartErr = -13002,                        /*during send, pmgr did not start hs*/
    pmSendEndErr = -13003,                          /*during send, pmgr did not finish hs*/
    pmRecvStartErr = -13004,                        /*during receive, pmgr did not start hs*/
    pmRecvEndErr = -13005,                          /*during receive, pmgr did not finish hs configured for this connection*/

/*Scrap Manager errors*/
    noScrapErr = -100,                              /*No scrap exists error*/
    noTypeErr = -102,                               /*No object of that type in scrap*/
    memROZWarn = -99,                               /*soft error in ROZ*/
    memROZError = -99,                              /*hard error in ROZ*/
    memROZErr = -99,                                /*hard error in ROZ*/
    memFullErr = -108,                              /*Not enough room in heap zone*/
    nilHandleErr = -109,                            /*Master Pointer was NIL in HandleZone or other*/
    memWZErr = -111,                                /*WhichZone failed (applied to free block)*/
    memPurErr = -112,                               /*trying to purge a locked or non-purgeable block*/
    memAdrErr = -110                                /*address was odd; or out of range*/
};
enum {
    memAZErr = -113,                                /*Address in zone check failed*/
    memPCErr = -114,                                /*Pointer Check failed*/
    memBCErr = -115,                                /*Block Check failed*/
    memSCErr = -116,                                /*Size Check failed*/
    memLockedErr = -117,                            /*trying to move a locked block (MoveHHi)*/
    resNotFound = -192,                             /*Resource not found*/
    resFNotFound = -193,                            /*Resource file not found*/
    addResFailed = -194,                            /*AddResource failed*/
    addRefFailed = -195,                            /*AddReference failed*/
    rmvResFailed = -196,                            /*RmveResource failed*/
    rmvRefFailed = -197,                            /*RmveReference failed*/
    resAttrErr = -198,                              /*attribute inconsistent with operation*/
    mapReadErr = -199,                              /*map inconsistent with operation*/
    CantDecompress = -186,                          /*resource bent ("the bends") - can't decompress a compressed resource*/
    badExtResource = -185,                          /*extended resource has a bad format.*/
    evtNotEnb = 1,                                  /*event not enabled at PostEvent*/
    noMemForPictPlaybackErr = -145,
    rgnTooBigError = -147,
    pixMapTooDeepErr = -148,
    nsStackErr = -149
};
enum {
    cMatchErr = -150,                               /*Color2Index failed to find an index*/
    cTempMemErr = -151,                             /*failed to allocate memory for temporary structures*/
    cNoMemErr = -152,                               /*failed to allocate memory for structure*/
    cRangeErr = -153,                               /*range error on colorTable request*/
    cProtectErr = -154,                             /*colorTable entry protection violation*/
    cDevErr = -155,                                 /*invalid type of graphics device*/
    cResErr = -156,                                 /*invalid resolution for MakeITable*/
    rgnTooBigErr = -500,
    updPixMemErr = -125,                            /*insufficient memory to update a pixmap*/
    pictInfoVersionErr = -11000,                    /* wrong version of the PictInfo structure */
    pictInfoIDErr = -11001,                         /* the internal consistancy check for the PictInfoID is wrong */
    pictInfoVerbErr = -11002,                       /* the passed verb was invalid */
    cantLoadPickMethodErr = -11003,                 /* unable to load the custom pick proc */
    colorsRequestedErr = -11004,                    /* the number of colors requested was illegal */
    pictureDataErr = -11005,                        /* the picture data was invalid */

/*Sound Manager errors*/
    noHardware = noHardwareErr,                     /* *** obsolete spelling */
    notEnoughHardware = notEnoughHardwareErr,       /* *** obsolete spelling */
    queueFull = -203,                               /*Sound Manager Error Returns*/
    resProblem = -204,                              /*Sound Manager Error Returns*/
    badChannel = -205                               /*Sound Manager Error Returns*/
};
enum {
    badFormat = -206,                               /*Sound Manager Error Returns*/
    notEnoughBufferSpace = -207,                    /* could not allocate enough memory */
    badFileFormat = -208,                           /* was not type AIFF or was of bad format,corrupt */
    channelBusy = -209,                             /* the Channel is being used for a PFD already */
    buffersTooSmall = -210,                         /* can not operate in the memory allowed */
    channelNotBusy = -211,
    noMoreRealTime = -212,                          /* not enough CPU cycles left to add another task */
    siNoSoundInHardware = -220,                     /*no Sound Input hardware*/
    siBadSoundInDevice = -221,                      /*invalid index passed to SoundInGetIndexedDevice*/
    siNoBufferSpecified = -222,                     /*returned by synchronous SPBRecord if nil buffer passed*/
    siInvalidCompression = -223,                    /*invalid compression type*/
    siHardDriveTooSlow = -224,                      /*hard drive too slow to record to disk*/
    siInvalidSampleRate = -225,                     /*invalid sample rate*/
    siInvalidSampleSize = -226,                     /*invalid sample size*/
    siDeviceBusyErr = -227,                         /*input device already in use*/
    siBadDeviceName = -228,                         /*input device could not be opened*/
    siBadRefNum = -229,                             /*invalid input device reference number*/
    siInputDeviceErr = -230,                        /*input device hardware failure*/
    siUnknownInfoType = -231,                       /*invalid info type selector (returned by driver)*/
    siUnknownQuality = -232                         /*invalid quality selector (returned by driver)*/
};
enum {

/*Notification Manager errors*/
    nmTypErr = -299,                                /*wrong queue type*/
    siInitSDTblErr = 1,                             /*slot int dispatch table could not be initialized.*/
    siInitVBLQsErr = 2,                             /*VBLqueues for all slots could not be initialized.*/
    siInitSPTblErr = 3,                             /*slot priority table could not be initialized.*/
    sdmJTInitErr = 10,                              /*SDM Jump Table could not be initialized.*/
    sdmInitErr = 11,                                /*SDM could not be initialized.*/
    sdmSRTInitErr = 12,                             /*Slot Resource Table could not be initialized.*/
    sdmPRAMInitErr = 13,                            /*Slot PRAM could not be initialized.*/
    sdmPriInitErr = 14,                             /*Cards could not be initialized.*/
    smSDMInitErr = -290,                            /*Error; SDM could not be initialized.*/
    smSRTInitErr = -291,                            /*Error; Slot Resource Table could not be initialized.*/
    smPRAMInitErr = -292,                           /*Error; Slot Resource Table could not be initialized.*/
    smPriInitErr = -293,                            /*Error; Cards could not be initialized.*/
    smEmptySlot = -300,                             /*No card in slot*/
    smCRCFail = -301,                               /*CRC check failed for declaration data*/
    smFormatErr = -302,                             /*FHeader Format is not Apple's*/
    smRevisionErr = -303,                           /*Wrong revison level*/
    smNoDir = -304,                                 /*Directory offset is Nil */
    smDisabledSlot = -305,                          /*This slot is disabled (-305 use to be smLWTstBad)*/
    smNosInfoArray = -306                           /*No sInfoArray. Memory Mgr error.*/
};
enum {
    smResrvErr = -307,                              /*Fatal reserved error. Resreved field <> 0.*/
    smUnExBusErr = -308,                            /*Unexpected BusError*/
    smBLFieldBad = -309,                            /*ByteLanes field was bad.*/
    smFHBlockRdErr = -310,                          /*Error occured during _sGetFHeader.*/
    smFHBlkDispErr = -311,                          /*Error occured during _sDisposePtr (Dispose of FHeader block).*/
    smDisposePErr = -312,                           /*_DisposePointer error*/
    smNoBoardSRsrc = -313,                          /*No Board sResource.*/
    smGetPRErr = -314,                              /*Error occured during _sGetPRAMRec (See SIMStatus).*/
    smNoBoardId = -315,                             /*No Board Id.*/
    smInitStatVErr = -316,                          /*The InitStatusV field was negative after primary or secondary init.*/
    smInitTblVErr = -317,                           /*An error occured while trying to initialize the Slot Resource Table.*/
    smNoJmpTbl = -318,                              /*SDM jump table could not be created.*/
    smBadBoardId = -319,                            /*BoardId was wrong; re-init the PRAM record.*/
    smBusErrTO = -320,                              /*BusError time out.*/

/* The following errors are for primary or secondary init code.  The errors are logged in the
vendor status field of the sInfo record.  Normally the vendor error is not Apple's concern,
but a special error is needed to patch secondary inits.*/


    svTempDisable = -32768,                         /*Temporarily disable card but run primary init.*/
    svDisabled = -32640,                            /*Reserve range -32640 to -32768 for Apple temp disables.*/
    smBadRefId = -330,                              /*Reference Id not found in List*/
    smBadsList = -331,                              /*Bad sList: Id1 < Id2 < Id3 ...format is not followed.*/
    smReservedErr = -332,                           /*Reserved field not zero*/
    smCodeRevErr = -333                             /*Code revision is wrong*/
};
enum {
    smCPUErr = -334,                                /*Code revision is wrong*/
    smsPointerNil = -335,                           /*LPointer is nil From sOffsetData. If this error occurs; check sInfo rec for more information.*/
    smNilsBlockErr = -336,                          /*Nil sBlock error (Dont allocate and try to use a nil sBlock)*/
    smSlotOOBErr = -337,                            /*Slot out of bounds error*/
    smSelOOBErr = -338,                             /*Selector out of bounds error*/
    smNewPErr = -339,                               /*_NewPtr error*/
    smBlkMoveErr = -340,                            /*_BlockMove error*/
    smCkStatusErr = -341,                           /*Status of slot = fail.*/
    smGetDrvrNamErr = -342,                         /*Error occured during _sGetDrvrName.*/
    smDisDrvrNamErr = -343,                         /*Error occured during _sDisDrvrName.*/
    smNoMoresRsrcs = -344,                          /*No more sResources*/
    smsGetDrvrErr = -345,                           /*Error occurred during _sGetDriver.*/
    smBadsPtrErr = -346,                            /*Bad pointer was passed to sCalcsPointer*/
    smByteLanesErr = -347,                          /*NumByteLanes was determined to be zero.*/
    smOffsetErr = -348,                             /*Offset was too big (temporary error*/
    smNoGoodOpens = -349,                           /*No opens were successfull in the loop.*/
    smSRTOvrFlErr = -350,                           /*SRT over flow.*/
    smRecNotFnd = -351,                             /*Record not found in the SRT.*/
    editionMgrInitErr = -450,                       /*edition manager not inited by this app*/
    badSectionErr = -451                            /*not a valid SectionRecord*/
};
enum {
    notRegisteredSectionErr = -452,                 /*not a registered SectionRecord*/
    badEditionFileErr = -453,                       /*edition file is corrupt*/
    badSubPartErr = -454,                           /*can not use sub parts in this release*/
    multiplePublisherWrn = -460,                    /*A Publisher is already registered for that container*/
    containerNotFoundWrn = -461,                    /*could not find editionContainer at this time*/
    containerAlreadyOpenWrn = -462,                 /*container already opened by this section*/
    notThePublisherWrn = -463,                      /*not the first registered publisher for that container*/
    teScrapSizeErr = -501,                          /*scrap item too big for text edit record*/
    hwParamErr = -502,                              /*bad selector for _HWPriv*/

/* Process Manager errors */
    procNotFound = -600,                            /* no eligible process with specified descriptor */
    memFragErr = -601,                              /* not enough room to launch app w/special requirements */
    appModeErr = -602,                              /* memory mode is 32-bit, but app not 32-bit clean */
    protocolErr = -603,                             /* app made module calls in improper order */
    hardwareConfigErr = -604,                       /* hardware configuration not correct for call */
    appMemFullErr = -605,                           /* application SIZE not big enough for launch */
    appIsDaemon = -606,                             /* app is BG-only, and launch flags disallow this */

/*MemoryDispatch errors*/
    notEnoughMemoryErr = -620,                      /*insufficient physical memory*/
    notHeldErr = -621,                              /*specified range of memory is not held*/
    cannotMakeContiguousErr = -622,                 /*cannot make specified range contiguous*/
    notLockedErr = -623                             /*specified range of memory is not locked*/
};
enum {
    interruptsMaskedErr = -624,                     /*donÕt call with interrupts masked*/
    cannotDeferErr = -625,                          /*unable to defer additional functions*/
    ddpSktErr = -91,                                /*error in soket number*/
    ddpLenErr = -92,                                /*data length too big*/
    noBridgeErr = -93,                              /*no network bridge for non-local send*/
    lapProtErr = -94,                               /*error in attaching/detaching protocol*/
    excessCollsns = -95,                            /*excessive collisions on write*/
    portInUse = -97,                                /*driver Open error code (port is in use)*/
    portNotCf = -98,                                /*driver Open error code (parameter RAM not configured for this connection)*/
    nbpBuffOvr = -1024,                             /*Buffer overflow in LookupName*/
    nbpNoConfirm = -1025,
    nbpConfDiff = -1026,                            /*Name confirmed at different socket*/
    nbpDuplicate = -1027,                           /*Duplicate name exists already*/
    nbpNotFound = -1028,                            /*Name not found on remove*/
    nbpNISErr = -1029,                              /*Error trying to open the NIS*/
    aspBadVersNum = -1066,                          /*Server cannot support this ASP version*/
    aspBufTooSmall = -1067,                         /*Buffer too small*/
    aspNoMoreSess = -1068,                          /*No more sessions on server*/
    aspNoServers = -1069,                           /*No servers at that address*/
    aspParamErr = -1070                             /*Parameter error*/
};
enum {
    aspServerBusy = -1071,                          /*Server cannot open another session*/
    aspSessClosed = -1072,                          /*Session closed*/
    aspSizeErr = -1073,                             /*Command block too big*/
    aspTooMany = -1074,                             /*Too many clients (server error)*/
    aspNoAck = -1075,                               /*No ack on attention request (server err)*/
    reqFailed = -1096,
    tooManyReqs = -1097,
    tooManySkts = -1098,
    badATPSkt = -1099,
    badBuffNum = -1100,
    noRelErr = -1101,
    cbNotFound = -1102,
    noSendResp = -1103,
    noDataArea = -1104,
    reqAborted = -1105,
    buf2SmallErr = -3101,
    noMPPErr = -3102,
    ckSumErr = -3103,
    extractErr = -3104,
    readQErr = -3105
};
enum {
    atpLenErr = -3106,
    atpBadRsp = -3107,
    recNotFnd = -3108,
    sktClosedErr = -3109,
    afpAccessDenied = -5000,
    afpAuthContinue = -5001,
    afpBadUAM = -5002,
    afpBadVersNum = -5003,
    afpBitmapErr = -5004,
    afpCantMove = -5005,
    afpDenyConflict = -5006,
    afpDirNotEmpty = -5007,
    afpDiskFull = -5008,
    afpEofError = -5009,
    afpFileBusy = -5010,
    afpFlatVol = -5011,
    afpItemNotFound = -5012,
    afpLockErr = -5013,
    afpMiscErr = -5014,
    afpNoMoreLocks = -5015
};
enum {
    afpNoServer = -5016,
    afpObjectExists = -5017,
    afpObjectNotFound = -5018,
    afpParmErr = -5019,
    afpRangeNotLocked = -5020,
    afpRangeOverlap = -5021,
    afpSessClosed = -5022,
    afpUserNotAuth = -5023,
    afpCallNotSupported = -5024,
    afpObjectTypeErr = -5025,
    afpTooManyFilesOpen = -5026,
    afpServerGoingDown = -5027,
    afpCantRename = -5028,
    afpDirNotFound = -5029,
    afpIconTypeError = -5030,
    afpVolLocked = -5031,                           /*Volume is Read-Only*/
    afpObjectLocked = -5032,                        /*Object is M/R/D/W inhibited*/
    afpContainsSharedErr = -5033,                   /*$FFFFEC57   the folder being shared contains a shared folder */
    afpIDNotFound = -5034,                          /*$FFFFEC56*/
    afpIDExists = -5035                             /*$FFFFEC55*/
};
enum {
    afpDiffVolErr = -5036,                          /*$FFFFEC54*/
    afpCatalogChanged = -5037,                      /*$FFFFEC53*/
    afpSameObjectErr = -5038,                       /*$FFFFEC52*/
    afpBadIDErr = -5039,                            /*$FFFFEC51*/
    afpPwdSameErr = -5040,                          /*$FFFFEC50  someone tried to change their password to the same password on a mantadory password change */
    afpPwdTooShortErr = -5041,                      /*$FFFFEC4F  the password being set is too short: there is a minimum length that must be met or exceeded */
    afpPwdExpiredErr = -5042,                       /*$FFFFEC4E  the password being used is too old: this requires the user to change the password before log-in can continue */
    afpInsideSharedErr = -5043,                     /*$FFFFEC4D  the folder being shared is inside a shared folder OR the folder contains a shared folder and is being moved into a shared folder OR the folder contains a shared folder and is being moved into the descendent of a shared folder. */
    afpInsideTrashErr = -5044,                      /*$FFFFEC4C  the folder being shared is inside the trash folder OR the shared folder is being moved into the trash folder OR the folder is being moved to the trash and it contains a shared folder */

/*PPC errors*/
    notInitErr = -900,                              /* PPCToolBox not initialized */
    nameTypeErr = -902,                             /* Invalid or inappropriate locationKindSelector in locationName */
    noPortErr = -903,                               /* Unable to open port or bad portRefNum */
    noGlobalsErr = -904,                            /* The system is hosed, better re-boot */
    localOnlyErr = -905,                            /* Network activity is currently disabled */
    destPortErr = -906,                             /* Port does not exist at destination */
    sessTableErr = -907,                            /* Out of session tables, try again later */
    noSessionErr = -908,                            /* Invalid session reference number */
    badReqErr = -909,                               /* bad parameter or invalid state for operation */
    portNameExistsErr = -910,                       /* port is already open (perhaps in another app) */
    noUserNameErr = -911                            /* user name unknown on destination machine */
};
enum {
    userRejectErr = -912,                           /* Destination rejected the session request */
    noMachineNameErr = -913,                        /* user hasn't named his Macintosh in the Network Setup Control Panel */
    noToolboxNameErr = -914,                        /* A system resource is missing, not too likely */
    noResponseErr = -915,                           /* unable to contact destination */
    portClosedErr = -916,                           /* port was closed */
    sessClosedErr = -917,                           /* session was closed */
    badPortNameErr = -919,                          /* PPCPortRec malformed */
    noDefaultUserErr = -922,                        /* user hasn't typed in owners name in Network Setup Control Pannel */
    notLoggedInErr = -923,                          /* The default userRefNum does not yet exist */
    noUserRefErr = -924,                            /* unable to create a new userRefNum */
    networkErr = -925,                              /* An error has occured in the network, not too likely */
    noInformErr = -926,                             /* PPCStart failed because destination did not have inform pending */
    authFailErr = -927,                             /* unable to authenticate user at destination */
    noUserRecErr = -928,                            /* Invalid user reference number */
    badServiceMethodErr = -930,                     /* illegal service type, or not supported */
    badLocNameErr = -931,                           /* location name malformed */
    guestNotAllowedErr = -932,                      /* destination port requires authentication */
    swOverrunErr = 1,                               /*serial driver error masks*/
    parityErr = 16,                                 /*serial driver error masks*/
    hwOverrunErr = 32                               /*serial driver error masks*/
};
enum {
    framingErr = 64,                                /*serial driver error masks*/
    dsBusError = 1,                                 /*bus error */
    dsAddressErr = 2,                               /*address error*/
    dsIllInstErr = 3,                               /*illegal instruction error*/
    dsZeroDivErr = 4,                               /*zero divide error*/
    dsChkErr = 5,                                   /*check trap error*/
    dsOvflowErr = 6,                                /*overflow trap error*/
    dsPrivErr = 7,                                  /*privilege violation error*/
    dsTraceErr = 8,                                 /*trace mode error*/
    dsLineAErr = 9,                                 /*line 1010 trap error*/
    dsLineFErr = 10,                                /*line 1111 trap error*/
    dsMiscErr = 11,                                 /*miscellaneous hardware exception error*/
    dsCoreErr = 12,                                 /*unimplemented core routine error*/
    dsIrqErr = 13,                                  /*uninstalled interrupt error*/
    dsIOCoreErr = 14,                               /*IO Core Error*/
    dsLoadErr = 15,                                 /*Segment Loader Error*/
    dsFPErr = 16,                                   /*Floating point error*/
    dsNoPackErr = 17,                               /*package 0 not present*/
    dsNoPk1 = 18,                                   /*package 1 not present*/
    dsNoPk2 = 19                                    /*package 2 not present*/
};
enum {
    dsNoPk3 = 20,                                   /*package 3 not present*/
    dsNoPk4 = 21,                                   /*package 4 not present*/
    dsNoPk5 = 22,                                   /*package 5 not present*/
    dsNoPk6 = 23,                                   /*package 6 not present*/
    dsNoPk7 = 24,                                   /*package 7 not present*/
    dsMemFullErr = 25,                              /*out of memory!*/
    dsBadLaunch = 26,                               /*can't launch file*/
    dsFSErr = 27,                                   /*file system map has been trashed*/
    dsStknHeap = 28,                                /*stack has moved into application heap*/
    negZcbFreeErr = 33,                             /*ZcbFree has gone negative*/
    dsFinderErr = 41,                               /*can't load the Finder error*/
    dsBadSlotInt = 51,                              /*unserviceable slot interrupt*/
    dsBadSANEOpcode = 81,                           /*bad opcode given to SANE Pack4*/
    dsBadPatchHeader = 83,                          /*SetTrapAddress saw the Òcome-fromÓ header*/
    menuPrgErr = 84,                                /*happens when a menu is purged*/
    dsMBarNFnd = 85,                                /*Menu Manager Errors*/
    dsHMenuFindErr = 86,                            /*Menu Manager Errors*/
    dsWDEFNotFound = 87,                            /*could not load WDEF*/
    dsCDEFNotFound = 88,                            /*could not load CDEF*/
    dsMDEFNotFound = 89                             /*could not load MDEF*/
};
enum {
    dsNoFPU = 90,                                   /*an FPU instruction was executed and the machine doesnÕt have one*/
    dsNoPatch = 98,                                 /*Can't patch for particular Model Mac*/
    dsBadPatch = 99,                                /*Can't load patch resource*/
    dsParityErr = 101,                              /*memory parity error*/
    dsOldSystem = 102,                              /*System is too old for this ROM*/
    ds32BitMode = 103,                              /*booting in 32-bit on a 24-bit sys*/
    dsNeedToWriteBootBlocks = 104,                  /*need to write new boot blocks*/
    dsNotEnoughRAMToBoot = 105,                     /*must have at least 1.5MB of RAM to boot 7.0*/
    dsBufPtrTooLow = 106,                           /*bufPtr moved too far during boot*/
    dsReinsert = 30,                                /*request user to reinsert off-line volume*/
    shutDownAlert = 42,                             /*handled like a shutdown error*/
    dsShutDownOrRestart = 20000,                    /*user choice between ShutDown and Restart*/
    dsSwitchOffOrRestart = 20001,                   /*user choice between switching off and Restart*/
    dsForcedQuit = 20002,                           /*allow the user to ExitToShell, return if Cancel*/

/*System Errors that are used after MacsBug is loaded to put up dialogs since these should not cause MacsBug to stop, they must be in the range (30, 42, 16384-32767) negative numbers add to an existing dialog without putting up a whole new dialog*/
    dsMacsBugInstalled = -10,                       /*say ÒMacsBug InstalledÓ*/
    dsDisassemblerInstalled = -11,                  /*say ÒDisassembler InstalledÓ*/
    dsExtensionsDisabled = -13,                     /*say ÒExtensions DisabledÓ*/
    dsGreeting = 40,                                /*welcome to Macintosh greeting*/
    dsSysErr = 32767,                               /*general system error*/

/*old names here for compatibilityÕs sake*/
    WDEFNFnd = dsWDEFNotFound
};
enum {
    CDEFNFnd = dsCDEFNotFound,
    dsNotThe1 = 31,                                 /*not the disk I wanted*/
    dsBadStartupDisk = 42,                          /*unable to mount boot volume (sad Mac only)*/
    dsSystemFileErr = 43,                           /*canÕt find System file to open (sad Mac only)*/
    dsHD20Installed = -12,                          /*say ÒHD20 StartupÓ*/
    mBarNFnd = -126,                                /*system error code for MBDF not found*/
    hMenuFindErr = -127,                            /*could not find HMenu's parent in MenuKey*/
    userBreak = -490,                               /*user debugger break*/
    strUserBreak = -491,                            /*user debugger break; display string on stack*/
    exUserBreak = -492,                             /*user debugger break; execute debugger commands on stack*/

/*obsolete errors that are no longer used, but I donÕt have the guts to remove from this file*/
    selectorErr = paramErr                          /* bad selector, for selector-based traps */
};

} }

#endif
