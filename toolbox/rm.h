#ifndef __mpw_rm_h__
#define __mpw_rm_h__

#include <cstdint>

#include <macos/tool_return.h>

namespace RM
{

	namespace Native
	{
		using MacOS::tool_return;
		
		tool_return<void> SetResLoad(bool tf);
		tool_return<uint32_t> GetResource(uint32_t type, uint16_t id);
		tool_return<int16_t> OpenResFile(const std::string &path_name, uint16_t permissions = 0);
		tool_return<void> CloseResFile(uint16_t refNum);

		tool_return<void> CreateResFile(const std::string &path, uint32_t creator = 0, uint32_t fileType = 0);

	}

	uint16_t CloseResFile(uint16_t trap);
	uint16_t GetNamedResource(uint16_t trap);
	uint16_t Get1NamedResource(uint16_t trap);

	uint16_t GetResource(uint16_t trap);
	uint16_t Get1Resource(uint16_t trap);
	uint16_t Get1IndResource(uint16_t trap);

	uint16_t ReleaseResource(uint16_t trap);
	uint16_t ResError(uint16_t trap);

	uint16_t OpenResFile(uint16_t trap);
	uint16_t OpenRFPerm(uint16_t trap);
	uint16_t HOpenResFile(uint16_t trap);

	uint16_t SetResLoad(uint16_t trap);

	uint16_t CurResFile(uint16_t trap);
	uint16_t UseResFile(uint16_t trap);

	uint16_t CreateResFile(uint16_t trap);
	uint16_t HCreateResFile(uint16_t trap);

	uint16_t Count1Resources(uint16_t trap);
	uint16_t UpdateResFile(uint16_t trap);
	uint16_t GetResFileAttrs(uint16_t trap);
	uint16_t SetResFileAttrs(uint16_t trap);

	uint16_t AddResource(uint16_t trap);
	uint16_t GetResAttrs(uint16_t trap);
	uint16_t SetResAttrs(uint16_t trap);
	uint16_t WriteResource(uint16_t trap);
	uint16_t DetachResource(uint16_t trap);
	uint16_t ChangedResource(uint16_t trap);
	uint16_t RemoveResource(uint16_t trap);

	uint16_t GetResourceSizeOnDisk(uint16_t trap);

	uint16_t GetResInfo(uint16_t trap);
	uint16_t LoadResource(uint16_t trap);

	uint16_t HomeResFile(uint16_t trap);
	uint16_t Count1Types(uint16_t trap);
	uint16_t Get1IndType(uint16_t trap);


	uint16_t FSpOpenResFile(void);
	uint16_t FSpCreateResFile(void);

}


#endif
