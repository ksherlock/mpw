#include <Files.h>
#include <stdio.h>
#include <string.h>

	/*
	 *
	 * MacOS/Basilisk:
	 *
	 * GetVol(): vRefNum = -32637
	 * GetVol(MacOS): vRefNum = -32637
	 * HGetVol(): vRefNum = -32637 dirID = 3382
	 * HGetVol(MacOS): vRefNum = -32637 dirID = 3382
	 *
	 */

void gv(StringPtr name)
{
	OSErr err;
	short vRefNum;
	err = GetVol(name, &vRefNum);

	if (err) fprintf(stdout, "GetVol(%s): Error = %d\n", name ? name + 1 : (StringPtr)"", err);
	else fprintf(stdout, "GetVol(%s): vRefNum = %d\n", name ? name + 1 : (StringPtr)"", vRefNum);
}


void hgv(StringPtr name)
{
	OSErr err;
	short vRefNum;
	long dirID;

	err = HGetVol(name, &vRefNum, &dirID);

	if (err) fprintf(stdout, "HGetVol(%s): Error = %d\n", name ? name + 1 : (StringPtr)"", err);
	else fprintf(stdout, "HGetVol(%s): vRefNum = %d dirID = %d\n", 
		name ? name + 1 : (StringPtr)"", vRefNum, dirID);
}



int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	gv(NULL);
	gv((StringPtr)"\pMacOS");


	hgv(NULL);
	hgv((StringPtr)"\pMacOS");


	return 0;
}