
#include "mpw_time.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>


namespace {

	const long EpochAdjust = 86400 * (365 * (1970 - 1904) + 17); // 17 leap years.

}
namespace Time
{

	time_t UnixToMac(time_t t)
	{
		return t + EpochAdjust;
	}
	time_t MacToUnix(time_t t)
	{
		return t - EpochAdjust;
	}

	uint16_t ReadDateTime(uint16_t trap)
	{

		/* 
		 * on entry:
		 * A0 Pointer to long word secs
		 *
		 * on exit:
		 * A0 pointer to long word secs
		 * D0 Result code
		 *
		 */

		time_t now;

		uint32_t secsPtr = cpuGetAReg(0);

		fprintf(stderr, "%04x ReadDateTime(%08x)\n", trap, secsPtr);

		now = ::time(NULL);


		now = UnixToMac(now);
		if (secsPtr) memoryWriteLong(now, secsPtr);

		// also set global variable Time.
		memoryWriteLong(now, 0x020c);
		return 0;
	}

	uint16_t SecondsToDate(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0 Seconds since midnight, January 1, 1904
		 * A0 pointer to date-time record
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t s = cpuGetDReg(0);
		uint32_t dtPtr = cpuGetAReg(0);

		fprintf(stderr, "%04x SecondsToDate(%08x, %08x)\n", trap, s, dtPtr);


		if (dtPtr)
		{
			struct tm *tm;
			time_t t;
			t = MacToUnix(s);

			tm = ::localtime(&t);

			memoryWriteWord(tm->tm_year + 1900, dtPtr + 0);
			memoryWriteWord(tm->tm_mon + 1, dtPtr + 2);
			memoryWriteWord(tm->tm_mday, dtPtr + 4);
			memoryWriteWord(tm->tm_hour, dtPtr + 6);
			memoryWriteWord(tm->tm_min, dtPtr + 8);
			memoryWriteWord(tm->tm_sec, dtPtr + 10);
			memoryWriteWord(tm->tm_wday + 1, dtPtr + 12);
		}

		return 0;
	}

}