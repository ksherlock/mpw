#ifndef __mpw_toolbox_h__
#define __mpw_toolbox_h__

#include <string>

namespace ToolBox
{
	void dispatch(uint16_t trap);


	std::string ReadCString(uint32_t address);
	std::string ReadPString(uint32_t address);
	bool WritePString(uint32_t address, const std::string &s);

}


#endif