/* Reads from the file and checks consistency.
 * The buffer should all contain the same character!!
 */

#include <syscall.h>
#include <stdio.h>
#include "pfs.h"

char buffer[BIG];

int main(void)
{
	int bytes, j;
	int id;
	int fsize;

	printf("-\n");
	id = open("file.1");

	fsize = filesize(id);
	if (fsize < BIG * TIMES) {
		printf("Invalid filesize\n");
		close(id);
		exit(-1);
	}
	while (tell(id) <= (fsize-BIG))
	{
		bytes = read(id, buffer, BIG);

		if (bytes != BIG)
		{
			printf("Buffer not filled, read %d\n", bytes);
			close(id);
			exit(-1);
		}
		/* now check for consistency */
		for (j = 1; j < BIG; ++j)
		{
			if (buffer[0] != buffer[j])
			{
				/* Ooops, inconsistency */
				printf("INCONSISTENCY\n");
				close(id);
				exit(-1);
			}
		}
	}
	printf("*-\n");
	close(id);
	exit(0);
}
