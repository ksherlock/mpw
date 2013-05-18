#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <strings.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"

using ToolBox::Log;
using namespace ToolBox::Errors;

namespace OS {




	uint16_t AliasDispatch(uint16_t trap)
	{

		uint16_t selector = cpuGetDReg(0);

		Log("%04x AliasDispatch($%04x)\n", trap, selector);

		switch (selector)
		{

			case 0x000c:
				return ResolveAliasFile();
				break;

			default:
					fprintf(stderr, "AliasDispatch: selector $%04x not implemented\n", 
					selector);
				exit(1);		
		}


		return 0;
	}

}