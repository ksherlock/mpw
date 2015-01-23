#include <OSUtils.h>
#include <stdio.h>


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	// these are all nops in mpw.  Just verify it doesn't crash.

	SwapInstructionCache(1);
	SwapInstructionCache(0);

	FlushInstructionCache();


	SwapDataCache(1);
	SwapDataCache(0);

	FlushDataCache();
	FlushCodeCache();

	FlushCodeCacheRange(&main, 4);

	// ? selector 6? listed in OSUtils.a, not listed in pascal or c headers.
	//FlushExternalCache(); 

	return 0;
}