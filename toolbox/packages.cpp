/*
 * Copyright (c) 2015, Kelvin W Sherlock
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

#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <stdlib.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>
#include <macos/errors.h>
#include <macos/traps.h>

#include "toolbox.h"
#include "stackframe.h"
#include "os.h"
#include "packages.h"

using ToolBox::Log;
using OS::MacToUnix;

namespace Packages {

	enum {
		shortDate = 0,
		longDate = 1,
		abbrevDate = 2
	};

	enum {
		mdy = 0,
		dmy = 1,
		ymd = 2,
		myd = 3,
		dym = 4,
		ydm = 5
	};

	enum {
		timeCycle24                   = 0,    /*time sequence 0:00 - 23:59*/
		timeCycleZero                 = 1,    /*time sequence 0:00-11:59, 0:00 - 11:59*/
		timeCycle12                   = 255,  /*time sequence 12:00 - 11:59, 12:00 - 11:59*/
		zeroCycle                     = 1,    /*old name for timeCycleZero*/
		longDay                       = 0,    /*day of the month*/
		longWeek                      = 1,    /*day of the week*/
		longMonth                     = 2,    /*month of the year*/
		longYear                      = 3,    /*year*/
		supDay                        = 1,    /*suppress day of month*/
		supWeek                       = 2,    /*suppress day of week*/
		supMonth                      = 4,    /*suppress month*/
		supYear                       = 8,    /*suppress year*/

		dayLdingZ                     = 32,
		mntLdingZ                     = 64,
		century                       = 128,
		secLeadingZ                   = 32,
		minLeadingZ                   = 64,
		hrLeadingZ                    = 128
	};

	#if 0
struct Intl0Rec {
  char                decimalPt;              /*decimal point character*/
  char                thousSep;               /*thousands separator character*/
  char                listSep;                /*list separator character*/
  char                currSym1;               /*currency symbol*/
  char                currSym2;
  char                currSym3;
  UInt8               currFmt;                /*currency format flags*/
  UInt8               dateOrder;              /*order of short date elements: mdy, dmy, etc.*/
  UInt8               shrtDateFmt;            /*format flags for each short date element*/
  char                dateSep;                /*date separator character*/
  UInt8               timeCycle;              /*specifies time cycle: 0..23, 1..12, or 0..11*/
  UInt8               timeFmt;                /*format flags for each time element*/
  char                mornStr[4];             /*trailing string for AM if 12-hour cycle*/
  char                eveStr[4];              /*trailing string for PM if 12-hour cycle*/
  char                timeSep;                /*time separator character*/
  char                time1Suff;              /*trailing string for AM if 24-hour cycle*/
  char                time2Suff;
  char                time3Suff;
  char                time4Suff;
  char                time5Suff;              /*trailing string for PM if 24-hour cycle*/
  char                time6Suff;
  char                time7Suff;
  char                time8Suff;
  UInt8               metricSys;              /*255 if metric, 0 if inches etc.*/
  short               intl0Vers;              /*region code (hi byte) and version (lo byte)*/
};
	#endif
	enum {
		o_decimalPt = 0,
		o_thousSep = 1,
		o_listSep = 2, 
		o_currSym1 = 3,
		o_currSym2 = 4,
		o_currSym3 = 5,
		o_currFmt = 6,
		o_dateOrder = 7,
		o_shrtDateFmt = 8,
		o_dateSep = 9,
		o_timeCycle = 10,
		o_timeFmt = 11,
		o_mornStr = 12,
		o_eveStr = 16,
		o_timeSep = 20,
		o_time1Suff = 21,
		o_time2Suff = 22,
		o_time3Suff = 23,
		o_time4Suff = 24,
		o_time5Suff = 25,
		o_time6Suff = 26,
		o_time7Suff = 27,
		o_time8Suff = 28,
		o_metricSys = 29,
		o_intl0Vers = 30, /* 16-bit */

	};

	struct DateFormat {
		uint8_t dateOrder = mdy;
		uint8_t dateSep = '/';
		uint8_t shrtDateFmt = 0;

		void unpack(uint32_t intlPtr) {
			if (!intlPtr) return;

			dateOrder = memoryReadByte(intlPtr + o_dateOrder);
			shrtDateFmt = memoryReadByte(intlPtr + o_shrtDateFmt);
			dateSep = memoryReadByte(intlPtr + o_dateSep);
		}
	};

	struct TimeFormat {
		uint8_t timeCycle = timeCycle12;
		uint8_t timeSep = ':';
		uint8_t timeFmt = secLeadingZ + minLeadingZ;

		std::string mornStr = " AM";
		std::string eveStr = " PM";

		void unpack(uint32_t intlPtr) {
			if (!intlPtr) return;

			timeCycle = memoryReadByte(intlPtr + o_timeCycle);
			timeFmt = memoryReadByte(intlPtr + o_timeFmt);
			timeSep = memoryReadByte(intlPtr + o_timeSep);

			if (timeCycle ==  timeCycle24) {
				mornStr = read_string(intlPtr + o_time1Suff);
				eveStr = read_string(intlPtr + o_time5Suff);
			} else {
				mornStr = read_string(intlPtr + o_mornStr);
				eveStr = read_string(intlPtr + o_eveStr);
			}
		}

		static std::string read_string(uint32_t address) {
			std::string rv;
			for (unsigned i = 0; i < 4; ++i, ++address) {
				char c = memoryReadByte(address);
				if (!c) break;
				rv.push_back(c);
			}
			return rv;
		}
	};


	namespace {

		std::string FormatAbbrDate(uint32_t dateTime, uint32_t intlPtr) {
			std::string rv;

			char dd[8];
			char mm[8];
			char yy[8];

			std::string sep;

			DateFormat df;

			time_t t = MacToUnix(dateTime);
			struct tm *tm = ::localtime(&t);

			if (intlPtr) df.unpack(intlPtr);
			if (df.dateSep) sep.push_back(df.dateSep);

			if (df.shrtDateFmt & dayLdingZ) snprintf(dd, sizeof(dd), "%02u", tm->tm_mday);
			else snprintf(dd, sizeof(dd), "%u", tm->tm_mday);

			if (df.shrtDateFmt & mntLdingZ) snprintf(mm, sizeof(mm), "%02u", tm->tm_mon + 1);
			else snprintf(mm, sizeof(mm), "%u", tm->tm_mon + 1);

			if (df.shrtDateFmt & century) snprintf(yy, sizeof(yy), "%04u", tm->tm_year + 1900);
			else snprintf(yy, sizeof(yy), "%02u", tm->tm_year % 100);


			switch(df.dateOrder) {
				default:
				case mdy:
  					rv = mm + sep + dd + sep + yy;
  					break;
				case dmy:
  					rv = dd + sep + mm + sep + yy;
  					break;
				case ymd:
  					rv = yy + sep + mm + sep + dd;
					break;
				case myd:
  					rv = mm + sep + yy + sep + dd;
  					break;
				case dym:
  					rv = dd + sep + yy + sep + mm;
  					break;
				case ydm:
  					rv = yy + sep + dd + sep + mm;
  					break;
			}
			return rv;
		}

		std::string FormatDate(uint32_t dateTime, unsigned form, uint32_t intlPtr) {
			/* not localized.  some foreign day/month strings include macroman chars */

			if (form == abbrevDate)
				return FormatAbbrDate(dateTime, intlPtr);

			char buffer[256];
			int length;
			std::string rv;



			time_t t = MacToUnix(dateTime);
			struct tm *tm = ::localtime(&t);


			switch(form) {
				case shortDate:
					// Sat, Jul 11, 2020
					// length = std::strftime(buffer, sizeof(buffer), "%a, %b %e, %Y", tm);
					length = std::strftime(buffer, sizeof(buffer), "%a, %b ", tm);
					rv = std::string(buffer, buffer + length);
					length = std::snprintf(buffer, sizeof(buffer), "%u, %04u",
						tm->tm_mday, tm->tm_year + 1900);
					rv.append(buffer, buffer + length);
					break;
				case longDate:
				default:
					// Saturday, July 11, 2020
					// length = std::strftime(buffer, sizeof(buffer), "%A, %B %e, %Y", tm);
					length = std::strftime(buffer, sizeof(buffer), "%A, %B ", tm);
					rv = std::string(buffer, buffer + length);
					length = std::snprintf(buffer, sizeof(buffer), "%u, %04u",
						tm->tm_mday, tm->tm_year + 1900);
					rv.append(buffer, buffer + length);
					break;
			}

			return rv;

		}


		std::string FormatTime(uint32_t dateTime, bool seconds, uint32_t intlPtr) {

			char hh[8];
			char mm[8];
			char ss[8];

			std::string rv;

			std::string sep;

			TimeFormat tf;

			time_t t = MacToUnix(dateTime);
			struct tm *tm = ::localtime(&t);

			if (intlPtr) tf.unpack(intlPtr);
			if (tf.timeSep) sep.push_back(tf.timeSep);


			if (tf.timeFmt & secLeadingZ) snprintf(ss, sizeof(ss), "%02u", tm->tm_sec);
			else snprintf(ss, sizeof(ss), "%u", tm->tm_sec);

			if (tf.timeFmt & minLeadingZ) snprintf(mm, sizeof(mm), "%02u", tm->tm_min);
			else snprintf(mm, sizeof(mm), "%u", tm->tm_min);


			unsigned h = tm->tm_hour;
			bool am = h < 12;
			switch (tf.timeCycle) {
				default:
				case timeCycle12:
					h %= 12;
					if (!h) h = 12;
					break;
				case timeCycle24:
					break;
				case timeCycleZero:
					h %= 12;
					break;
			}

			if (tf.timeFmt & hrLeadingZ) snprintf(hh, sizeof(hh), "%02u", h);
			else snprintf(hh, sizeof(hh), "%u", h);


			rv = hh + sep + mm;
			if (seconds) {
				rv += sep + ss;
			}

			rv.append( am ? tf.mornStr : tf.eveStr);

			return rv;
		}

	}

	uint16_t IUDateString()
	{
		// void IUDateString(long dateTime, DateForm longFlag, Str255 result)

		std::string out;

		uint32_t dateTime;
		uint8_t flag;
		uint32_t result;


		StackFrame<10>(dateTime, flag, result);

		Log("     IUDateString(%08x, %02x, %08x)\n", dateTime, flag, result);

		out = FormatDate(dateTime, flag, 0);
		ToolBox::WritePString(result, out);
		return 0;

	}

	uint16_t IUDatePString()
	{
		// void IUDateString(long dateTime, DateForm longFlag, Str255 result, Handle intlHandle)

		std::string out;

		uint32_t dateTime;
		uint8_t flag;
		uint32_t result;
		uint32_t intlHandle;
		uint32_t intlPtr;


		StackFrame<14>(dateTime, flag, result, intlHandle);

		Log("     IUDatePString(%08x, %02x, %08x, %08x)\n", dateTime, flag, result, intlHandle);

		intlPtr = intlHandle ? memoryReadLong(intlHandle) : 0;

		out = FormatDate(dateTime, flag, intlPtr);
		ToolBox::WritePString(result, out);
		return 0;

	}

	uint16_t IUTimeString()
	{
		// void IUTimeString(long dateTime,Boolean wantSeconds,Str255 result)
		// output: 12:00:00 AM or 12:00 AM

		std::string out;

		uint32_t dateTime;
		uint8_t wantSeconds;
		uint32_t result;


		StackFrame<10>(dateTime, wantSeconds, result);
		Log("     IUTimeString(%08x, %02x, %08x)\n", dateTime, wantSeconds, result);

		out = FormatTime(dateTime, wantSeconds, 0);

		ToolBox::WritePString(result, out);
		return 0;
	}


	uint16_t IUTimePString() {
		// void IUTimePString(long dateTime, Boolean wantSeconds, Str255 result, Handle intlHandle)


		std::string out;

		uint32_t dateTime;
		uint8_t wantSeconds;
		uint32_t result;
		uint32_t intlHandle;
		uint32_t intlPtr;

		StackFrame<14>(dateTime, wantSeconds, result, intlHandle);
		Log("     IUTimePString(%08x, %02x, %08x, %08x)\n", dateTime, wantSeconds, result, intlHandle);

		intlPtr = intlHandle ? memoryReadLong(intlHandle) : 0;

		out = FormatTime(dateTime, wantSeconds, intlPtr);
		ToolBox::WritePString(result, out);
		return 0;
	}

	uint16_t InitDateCache()
	{
		// OSErr InitDateCache(DateCachePtr theCache) 
		uint32_t theCache;
		uint32_t sp;
		sp = StackFrame<4>(theCache);
		Log("     InitDateCache(%08x)\n", theCache);
		/* cache not used */

		ToolReturn<4>(sp, 0);
		return 0;
	}

	enum {
		                                /* StringToDate status values */
		fatalDateTime                 = 0x8000, /* StringToDate and String2Time mask to a fatal error */
		longDateFound                 = 1,    /* StringToDate mask to long date found */
		leftOverChars                 = 2,    /* StringToDate & Time mask to warn of left over characters */
		sepNotIntlSep                 = 4,    /* StringToDate & Time mask to warn of non-standard separators */
		fieldOrderNotIntl             = 8,    /* StringToDate & Time mask to warn of non-standard field order */
		extraneousStrings             = 16,   /* StringToDate & Time mask to warn of unparsable strings in text */
		tooManySeps                   = 32,   /* StringToDate & Time mask to warn of too many separators */
		sepNotConsistent              = 64,   /* StringToDate & Time mask to warn of inconsistent separators */
		tokenErr                      = 0x8100, /* StringToDate & Time mask for 'tokenizer err encountered' */
		cantReadUtilities             = 0x8200,
		dateTimeNotFound              = 0x8400,
		dateTimeInvalid               = 0x8800
	};

#if 0
typedef short                           StringToDateStatus;
typedef StringToDateStatus              String2DateStatus;
struct DateCacheRecord {
  short               hidden[256];            /* only for temporary use */
};
typedef struct DateCacheRecord          DateCacheRecord;
typedef DateCacheRecord *               DateCachePtr;
struct DateTimeRec {
  short               year;
  short               month;
  short               day;
  short               hour;
  short               minute;
  short               second;
  short               dayOfWeek;
};
typedef struct DateTimeRec              DateTimeRec;

typedef SInt64                          LongDateTime;
union LongDateCvt {
  SInt64              c;
  struct {
    UInt32              lHigh;
    UInt32              lLow;
  }                       hl;
};
typedef union LongDateCvt               LongDateCvt;
union LongDateRec {
  struct {
    short               era;
    short               year;
    short               month;
    short               day;
    short               hour;
    short               minute;
    short               second;
    short               dayOfWeek;
    short               dayOfYear;
    short               weekOfYear;
    short               pm;
    short               res1;
    short               res2;
    short               res3;
  }                       ld;
  short               list[14];               /*Index by LongDateField!*/
  struct {
    short               eraAlt;
    DateTimeRec         oldDate;
  }                       od;
};
typedef union LongDateRec               LongDateRec;
#endif

	enum {

		o_era = 0, /* 0 = AD, -1 = BC */
		o_year = 2,
		o_month = 4, /* 1-12 */
		o_day = 6, /* 1-31 */
		o_hour = 8,
		o_minute = 10,
		o_second = 12,
		o_dayOfWeek = 14, /*1-7, sun-sat */
		o_dayOfYear = 16, /* 1-365 (366) */
		o_weekOfYear = 18, /* 1- 52 */
		o_pm = 20, /* 0 = am, 1 = pm */
		o_res1 = 22,
		o_res2 = 24,
		o_res3 = 26,
	};

	uint16_t StringToDate()
	{
		// StringToDateStatus StringToDate(Ptr textPtr, long textLen, DateCachePtr theCache, long *lengthUsed, LongDateRec *dateTime)
		uint32_t sp;
		uint32_t textPtr;
		uint32_t textLen;
		uint32_t theCache;
		uint32_t lengthUsed;
		uint32_t dateTime;

		uint16_t rv = 0;
		uint32_t used = 0;

		struct tm tm = {};

		sp = StackFrame<20>(textPtr, textLen, theCache, lengthUsed, dateTime);
		std::string s = ToolBox::ReadString(textPtr, textLen);

		Log("     StringToDate(%s, %08lx)\n", s.c_str(), dateTime);

		rv = dateTimeNotFound;
		if (s.length()) {
			/* only allow %Y-%m-%d for now */
			char *cp;
			cp = strptime(s.c_str(), " %Y-%m-%d", &tm);
			if (!cp) cp = strptime(s.c_str(), "%m/%d/%Y", &tm);

			if (cp) {
				rv = longDateFound;
				used = cp - s.c_str();

				memoryWriteWord(0, dateTime+o_era); /* o = AD, -1 = BC */
				memoryWriteWord(1900 + tm.tm_year, dateTime+o_year);
				memoryWriteWord(1 + tm.tm_mon, dateTime+o_month);
				memoryWriteWord(0 + tm.tm_mday, dateTime+o_day);
				/* not populated */
				//memoryWriteWord(1 + tm.tm_wday, dateTime+o_dayOfWeek);
				//memoryWriteWord(1 + tm.tm_yday, dateTime+o_dayOfYear);
				// unsupported - week of year....
			}
		}


		memoryWriteLong(used, lengthUsed);
		ToolReturn<2>(sp, rv);
		return 0;
	}

	uint16_t StringToTime()
	{
		// StringToDateStatus StringToTime(Ptr textPtr, long textLen, DateCachePtr theCache, long *lengthUsed, LongDateRec *dateTime)
		uint32_t sp;
		uint32_t textPtr;
		uint32_t textLen;
		uint32_t theCache;
		uint32_t lengthUsed;
		uint32_t dateTime;

		uint16_t rv = 0;
		uint32_t used = 0;

		sp = StackFrame<20>(textPtr, textLen, theCache, lengthUsed, dateTime);
		std::string s = ToolBox::ReadString(textPtr, textLen);

		Log("     StringToTime(%s, %08lx)\n", s.c_str(), dateTime);


		rv = dateTimeNotFound;

		memoryWriteLong(used, lengthUsed);
		ToolReturn<2>(sp, rv);
		return 0;
	}

	uint16_t GetIntlResource()
	{
		// FUNCTION GetIntlResource (theID: Integer) :Handle;

		// todo -- actually load the resource

		/*
		 * theID Contains an integer (0, 1, 2, 4, or 5 respectively for
		 * the 'itl0', 'itl1', 'itl2',  'itl4', and 'itl5'resources) to
		 * identify the type of the desired international resource.
		 */


		uint16_t theID;

		uint32_t sp = StackFrame<2>(theID);

		Log("     GetIntlResource(%04x)\n", theID);

		ToolReturn<4>(sp, 0);
		return 0; // should set res error.
	}

	uint16_t Pack6(uint16_t trap)
	{

		uint16_t selector;
		StackFrame<2>(selector);

		Log("%04x Pack6(%04x)\n", trap, selector);

		switch(selector)
		{
		case 0x0000:
			return IUDateString();
		case 0x0002:
			return IUTimeString();
		// case 0x0004: return IsMetric();
		case 0x0006:
			return GetIntlResource();
		// case 0x0008: return SetIntlResource();
		case 0x000e:
			return IUDatePString();
		case 0x0010:
			return IUTimePString();

		default:
			fprintf(stderr, "Pack6: selector %04x not supported\n", selector);
			exit(1);
		}
		return 0;
	}


	uint16_t ScriptUtil(uint16_t trap)
	{
		uint32_t selector;
		StackFrame<4>(selector);
		Log("%04x ScriptUtil(%08x)\n", trap, selector);

		switch(selector)
		{
		case 0x8204fff8:
			return InitDateCache();
		case 0x8214fff6:
			return StringToDate();
		case 0x8214fff4:
			return StringToTime();
		default:
			fprintf(stderr, "ScriptUtil: selector %08x not supported\n", selector);
			exit(1);
		}



	}

}
