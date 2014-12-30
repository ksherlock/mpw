
#include <cstdint>
#include <ctime>


#include <toolbox/os.h>
#include <macos/errors.h>

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

		tm = ::localtime(&t);
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M:%S %p", tm);
		puts(buffer);
	}


	void Moof(void)
	{

		puts("");
		puts("          ##                                            ");
		puts("        ##  ##  ####                                    ");
		puts("        ##  ####  ##                                    ");
		puts("      ##          ##                                    ");
		puts("    ##    ##    ##                              ##      ");
		puts("  ##            ##                              ####    ");
		puts("##                ##                          ##  ##    ");
		puts("  ########          ####                    ##    ##    ");
		puts("          ##            ####################      ##    ");
		puts("          ##            ##############          ##      ");
		puts("            ####          ############        ##        ");
		puts("            ######            ######          ##        ");
		puts("            ######                          ##          ");
		puts("            ####                            ##          ");
		puts("            ##                              ##          ");
		puts("            ##      ################        ##          ");
		puts("            ##    ##                ##        ##        ");
		puts("            ##    ##                  ##      ##        ");
		puts("            ##    ##                    ##    ##        ");
		puts("            ##    ##                    ##    ##        ");
		puts("          ##    ##                    ##    ##          ");
		puts("          ######                      ######            ");
		puts("");

	}

}