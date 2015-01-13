#include <MacMemory.h>
#include <stdio.h>

enum {
	TimeLM = 0x020C
};

/* arc4random */

struct {
	unsigned char i;
	unsigned char j;
	unsigned char s[256];
} rs;

void arc4_init(void)
{
	int n;

	for (n = 0; n < 256; n++)
		rs.s[n] = n;

	rs.i = 0;
	rs.j = 0;
}

void arc4_addrandom(const unsigned char *dat, int datlen)
{
	int n;
	unsigned char si;

	rs.i--;
	for (n = 0; n < 256; n++) {
		rs.i = (rs.i + 1);
		si = rs.s[rs.i];
		rs.j = (rs.j + si + dat[n % datlen]);
		rs.s[rs.i] = rs.s[rs.j];
		rs.s[rs.j] = si;
	}
	rs.j = rs.i;
}


unsigned char arc4_getbyte(void)
{
	unsigned char si, sj;

	rs.i = (rs.i + 1);
	si = rs.s[rs.i];
	rs.j = (rs.j + si);
	sj = rs.s[rs.j];
	rs.s[rs.i] = sj;
	rs.s[rs.j] = si;
	return (rs.s[(si + sj) & 0xff]);
}

unsigned long arc4_get24(void)
{
	unsigned long val;

	val |= arc4_getbyte() << 16;
	val |= arc4_getbyte() << 8;
	val |= arc4_getbyte();

	return val;
}

void test(void)
{

	unsigned i,j;
	unsigned errors = 0;
	unsigned success = 0;

	for (i = 0; i < 10000; ++i)
	{
		Handle h[5];
		for (j = 0; j < 5; ++j) {
			unsigned long size = arc4_get24() >> 3;
			Handle hh = NewHandle(size);

			if (hh) {
				success++;
			} else {
				fprintf(stdout, "NewHandle failed (%u): %d\n", size, MemError());
				errors++;
			}

			h[j] = hh;
		}

		for (j = 0; j < 5; ++j) {
			DisposeHandle(h[j]);
		}

	}
	fprintf(stdout, "NewHandle failed: %u\n", errors);
	fprintf(stdout, "NewHandle succeeded: %u\n", success);
}

int main(void)
{

	// init with the time.
	arc4_init();
	arc4_addrandom((const unsigned char *)TimeLM, 4);

	test();
	return 0;
}