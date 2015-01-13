#include <MacMemory.h>
#include <stdio.h>
#include <string.h>


void test_new_pointer(unsigned size)
{
	unsigned long total = 0;
	unsigned long count = 0;

	for(;;) {

		void *p = NewPtr(size);
		if (!p) {
			fprintf(stdout, "memory error: %d\n", MemError());
			break;
		}

		total += size;
		count++;
	}

	fprintf(stdout, "%ld pointers allocated\n", count);
	fprintf(stdout, "%ld bytes allocated\n", total);
}

int main(void)
{
	test_new_pointer(1024 * 1024);
	return 0;
}
