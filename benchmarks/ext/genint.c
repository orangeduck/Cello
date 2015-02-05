/* This program generates 5 million recurrent integers. Each distinct integer
 * occurs 4 times in average.
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i, n = 5000000; // by default, output 5 million integers
	if (argc > 1) n = atoi(argv[1]);
	srand48(11);
	for (i = 0; i < n; ++i)
		printf("%u\n", (unsigned)((lrand48() % (n/4))  * 271828183u)); // 4 times in average
	return 0;
}
