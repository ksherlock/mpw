#ifndef __MPW_TIME_H__
#define __MPW_TIME_H__

#include <cstdint>
#include <ctime>

namespace Time
{
	
	time_t UnixToMac(time_t);
	time_t MacToUnix(time_t);

	uint16_t ReadDateTime(uint16_t trap);
	uint16_t SecondsToDate(uint16_t trap);

}

#endif