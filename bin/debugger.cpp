
#include <cstdint>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>

#include <readline/readline.h>


#include "loader.h"

#include <debugger/commands.h>



bool ParseLine(const char *iter, Command *command);
extern "C" {

	uint32_t debuggerReadLong(uint32_t address)
	{
		uint32_t tmp = 0;
		for (unsigned i = 0; i < 4; ++i)
		{
			if (address < Flags.memorySize)
				tmp = (tmp << 8) + Flags.memory[address++];
		}

		return tmp;
	}

}

void DebugHelp(const Command &cmd)
{
	printf("help\n");
	printf("break expression\n");
	printf("step");
	printf("continue");
	printf("\n");
	printf("print expression\n");
	printf("list expression\n");
	printf("dump expression\n");
	printf("register=expression\n");
	printf("\n");
	printf("registers: a0-7, d0-7, pc, sp, fp, csr\n");
	printf("\n");
}


void DebugPrint(const Command &cmd)
{
	for (unsigned i = 0; i < cmd.argc; ++i)
	{
		uint32_t data = cmd.argv[i];
		printf("$%08x %12u", data, data);
		if (data & 0x80000000)
			printf(" %12d", (int32_t)data);
		if ((data & 0xffff8000) == 0x8000)
			printf(" %6d", (int16_t)data);

		printf("\n");
	}	
}

void hexdump(const uint8_t *data, ssize_t size, uint32_t address = 0)
{
const char *HexMap = "0123456789abcdef";

    char buffer1[16 * 3 + 1 + 1];
    char buffer2[16 + 1];
    ssize_t offset = 0;
    unsigned i, j;
    
    
    while(size > 0)
    {        
        std::memset(buffer1, ' ', sizeof(buffer1));
        std::memset(buffer2, ' ', sizeof(buffer2));
        
        unsigned linelen = (unsigned)std::min(size, (ssize_t)16);
        
        
        for (i = 0, j = 0; i < linelen; i++)
        {
            unsigned x = data[i];
            buffer1[j++] = HexMap[x >> 4];
            buffer1[j++] = HexMap[x & 0x0f];
            j++;
            if (i == 7) j++;
            
            // isascii not part of std:: and may be a macro.
            buffer2[i] = isascii(x) && std::isprint(x) ? x : '.';
            
        }
        
        buffer1[sizeof(buffer1)-1] = 0;
        buffer2[sizeof(buffer2)-1] = 0;
        
    
        std::printf("%08x:\t%s\t%s\n", address + (unsigned)offset, buffer1, buffer2);
        offset += 16;
        data += 16;
        size -= 16;
    }
    std::printf("\n");
}

void DebugDump(const Command &cmd)
{
	// TODO -- if no address, use previous address.
	if (cmd.argc == 1)
	{
		uint32_t start = cmd.argv[0];
		if (start > Flags.memorySize) return;

		uint32_t end = std::min(start + 512, Flags.memorySize);
		ssize_t size = end - start;

		hexdump(Flags.memory + start, size, start);

	}
}

void DebugShell()
{
	char *cp;

	add_history("!Andy, it still has history!");
	for(;;)
	{
		cp = readline("] ");
		if (!cp)
		{
			printf("\n");
			break; // prompt for exit?
		}
		// parse the command...
		const char *iter = cp;
		while (*iter && isspace(*iter)) ++iter;

		if (*iter)
		{
			Command cmd;
			std::memset(&cmd, 0, sizeof(cmd));
			if (ParseLine(iter, &cmd))
			{
				switch(cmd.action)
				{
					case Print:
						DebugPrint(cmd);
						break;

					case Dump:
						DebugDump(cmd);
						break;

					default:
						DebugHelp(cmd);
						break;
				}
			}

			// todo -- don't add if same as previous command.
			add_history(cp);
		}
		free(cp);
	}

}

