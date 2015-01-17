#include <Resources.h>
#include <Files.h>
#include <stdio.h>
#include <stdlib.h>


/*
 * Test the three (thanks, Apple) ways to create a resource file.
 *
 */

ConstStr255Param fname = (ConstStr255Param)"\pxxx-test-resource-xxx";


void test_fspcreateresfile(void)
{
	OSErr err;
	FSSpec spec;

	err = FSMakeFSSpec(0, 0, fname, &spec);
	if (err) {

		fprintf(stderr, "FSMakeFSSpec failed: %d\n", err);
		exit(3);
	}

	FSpDelete(&spec);
	FSpCreateResFile(&spec, 'TEST', 'BINA', 0);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "FSpCreateResFile failed (File does not exist): %d\n", err);
		exit(1);
	}

	// Verify it doesn't fail if the file/fork already exist.
	FSpCreateResFile(&spec, 'TEST', 'BINA', 0);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "FSpCreateResFile (File/Fork exist) failed: %d\n", err);
		exit(2);
	}

	// Verify it doesn't fail if the file exists w/o a resource fork.
	FSpDelete(&spec);
	FSpCreate(&spec, 'TEST', 'BINA', 0);
	FSpCreateResFile(&spec, 'TEST', 'BINA', 0);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "FSpCreateResFile (File exists) failed: %d\n", err);
		exit(2);
	}

	FSpDelete(&spec);


}

void test_hcreateresfile(void)
{
	OSErr err;

	// will 0, 0 work on real macos?

	HDelete(0, 0, fname);
	HCreateResFile(0, 0, fname);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "HCreateResFile failed (File does not exist): %d\n", err);
		exit(1);
	}

	// Verify it doesn't fail if the file/fork already exist.
	HCreateResFile(0, 0, fname);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "HCreateResFile (File/Fork exist) failed: %d\n", err);
		exit(2);
	}

	// Verify it doesn't fail if the file exists w/o a resource fork.
	HDelete(0, 0, fname);
	HCreate(0, 0, fname, 'TEST', 'BINA');
	HCreateResFile(0, 0, fname);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "HCreateResFile (File exists) failed: %d\n", err);
		exit(2);
	}

	HDelete(0, 0, fname);
}

void test_createresfile(void)
{
	OSErr err;

	FSDelete(fname, 0);
	CreateResFile(fname);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "CreateResFile failed (File does not exist): %d\n", err);
		exit(1);
	}

	// Verify it does fail if the file/fork already exist.
	CreateResFile(fname);
	if ( (err = ResError()) != dupFNErr) {
		fprintf(stderr, "CreateResFile (File/Fork exist) failed: %d\n", err);
		exit(2);
	}

	// Verify it doesn't fail if the file exists w/o a resource fork.
	FSDelete(fname, 0);
	Create(fname, 0, 'TEST', 'BINA');
	CreateResFile(fname);
	if ( (err = ResError()) != 0) {
		fprintf(stderr, "CreateResFile (File exists) failed: %d\n", err);
		exit(2);
	}

	FSDelete(fname, 0);


}

int main(int argc, char **argv)
{

	(void)argc;
	(void)argv;


	test_createresfile();
	test_hcreateresfile();
	test_fspcreateresfile();


	return 0;
}