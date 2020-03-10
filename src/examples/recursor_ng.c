#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

#define MAX_SPAWN 100

int main (int argc, char *argv[])
{
	char buffer[128];
	pid_t pid[MAX_SPAWN];
	int retval = 0;
	int i, spawn;
	int ret = 0;

	if (argc != 4) 
	{
		printf ("usage: recursor_ng <string> <depth> <waitp>\n");
		exit (1);
	}

	/* Print args. */
	//printf ("%s %s %s %s\n", argv[0], argv[1], argv[2], argv[3]);
	printf("+");
	/* Execute child and wait for it to finish if requested. */
	spawn = atoi (argv[2]);

	if (spawn > MAX_SPAWN) {
		printf("Greater than MAX_SPAWN\n");
		exit(-1);
	}

	if (spawn != 0) 
	{
		for (i = 0; i < spawn;i++) {
			snprintf (buffer, sizeof buffer,
					"recursor_ng %s %d %s", argv[1], atoi (argv[2]) - 1, argv[3]);
			pid[i] = exec (buffer);
		}
		if (atoi (argv[3])) {
			for (i = 0; i < spawn;i++) {
				retval = wait (pid[i]);
				if (retval < 0)
					ret = 1;
			}
		}

		/* Done. */
		if (ret)
			printf("YOU HAVE FAILED\n");
	}
	exit (ret);
}
