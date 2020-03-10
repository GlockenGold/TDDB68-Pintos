/* Write on the disk.
 * Each time the buffer is filled with same character.
 * Different character every time!
 */

#include <syscall.h>
#include <stdio.h>
#include <string.h>
#include <random.h>

#include "pfs.h"


char buffer[BIG];

int main(int argc, char* argv[])
{
	int i;
	char c;
	int id;
	int write_count;
	char start;
	char end;

	random_init((int)argv[3][0]);
	printf("+\n");

	if (argc != 4 || strlen(argv[1]) != 1 || strlen(argv[2]) != 1)
		exit(1);

	start = argv[1][0];
	end   = argv[2][0];

	for (i = 0; i < TIMES / (end - start + 1) + 1; ++i)
	{
		for (c = start; c <= end; ++c)
		{
			memset(buffer, (int)c, BIG);

			id = open("file.1");

			seek(id,(random_ulong() % (TIMES-1)) * BIG);
			write_count = write(id, buffer, BIG);

			if ( write_count != BIG )
			{
				printf("TEST ERROR: write() wrote only %d bytes out of %d bytes\n",
						write_count, BIG);
				close(id);
				exit(-1);
			}
			close(id);
		}
	}
	printf("*+\n");
	exit(0);
}
