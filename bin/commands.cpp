
#include <cstdint>
#include <ctime>


#include <toolbox/os.h>
#include <macos/errors.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>

#include "debugger.h"
#include "debugger_internal.h"


namespace Debug {

	using namespace Internal;

	void PrintError(uint32_t value)
	{
		/* expr ; error -- interpret expr as an OSErr */


		// errors are signed 16-bit values.

		if ((int32_t)value > UINT16_MAX) return;
		if ((int32_t)value < INT16_MIN) return;

		
		uint16_t error = value;
		printf("%d\n", (int16_t)error);

		if (error)
		{
			bool found = false;
			const char *cp = ErrorName(error);
			if (cp)
			{
				printf("%s\n", cp);
				found = true;
			}


			for (auto iter = ErrorTableInvert.find(error); iter != ErrorTableInvert.end(); ++iter) {

				// multimap - continue until error doesn't match.
				if (iter->first != error) break;

				printf("%s\n", iter->second.c_str());
				found = true;
			}

			if (!found) printf("Unknown error\n");
		}
		else
		{
			printf("noErr\n");
			return;
		} 
	}


	void PrintDate(uint32_t value)
	{
		/* expr ; date -- interpret expr as a macos date */

		char buffer[64];
		struct tm *tm;
		time_t t = OS::MacToUnix(value);

		// localtime vs gmtime?
		tm = ::localtime(&t);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M:%S %p", tm);
		puts(buffer);
	}


	void Moof(void)
	{

		puts("");
		puts("              ##                                            ");
		puts("            ##  ##  ####                                    ");
		puts("            ##  ####  ##                                    ");
		puts("          ##          ##                                    ");
		puts("        ##    ##    ##                              ##      ");
		puts("      ##            ##                              ####    ");
		puts("    ##                ##                          ##  ##    ");
		puts("      ########          ####                    ##    ##    ");
		puts("              ##            ####################      ##    ");
		puts("              ##            ##############          ##      ");
		puts("                ####          ############        ##        ");
		puts("                ######            ######          ##        ");
		puts("                ######                          ##          ");
		puts("                ####                            ##          ");
		puts("                ##                              ##          ");
		puts("                ##      ################        ##          ");
		puts("                ##    ##                ##        ##        ");
		puts("                ##    ##                  ##      ##        ");
		puts("                ##    ##                    ##    ##        ");
		puts("                ##    ##                    ##    ##        ");
		puts("              ##    ##                    ##    ##          ");
		puts("              ######                      ######            ");
		puts("");

	}


	void StackCrawl(void)
	{
		// stack crawl based on a6 frame pointers. (link/unlink)
		// start with current a6.

		uint32_t a6 = cpuGetAReg(6);

		/*
		 * a6:  previous a6
		 * a6+4 return pc
		 * .... parameters, locals, stack data
		 * previous a6
		 * return pc
		 * ....
		 */
		// todo -- need a function to verify address is w/in stack range.


		if (!a6) return;
		printf("a6: %08x\n", a6);

		while(a6)
		{

			uint32_t prevA6 = ReadLong(a6);
			uint32_t pc = ReadLong(a6+4); // 

			puts("------------");
			printf("a6: %08x\n", prevA6);
			printf("pc: %08x", pc);

			// find the routine name...
			for (const auto &kv : SymbolTable)
			{
				const auto &name = kv.first;
				auto range = kv.second;

				// range end may be 0
				if ((pc == range.first) || (pc >= range.first && pc < range.second))
				{
					uint32_t offset = pc - range.first;
					if (offset)
						printf(" %s+$%x", name.c_str(), offset);
					else
						printf(" %s", name.c_str());
					break;
				}
			}
			puts("");

			// hexdump contents between pc and previous....


			a6 = prevA6;
		}


	}

}