/**
 * Read lines of data from stdin, echo to stdout with a time tag
 *
 * Author: Joe Desbonnet, May 2019.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void main (int argc, char **argv) {

	int fd;

	// Parse command line arguments. See usage() for details.
	int c;
	while ((c = getopt(argc, argv, "b:d:f:g:hio:qs:tv")) != -1) {
		switch(c) {

/*
			case 'd':
				debug_level = atoi (optarg);
				break;



			case 'f':
				if (optarg[0] == 'b') {
					stream_format = FORMAT_BINARY;
				} else if (optarg[0] == 'r') {
					stream_format = FORMAT_RAW;
				}
				break;
			

			case 'h':
				version();
				usage(argv[0]);
				exit(EXIT_SUCCESS);
*/
		}
	}

	
	struct timespec ts;
	char buf[4096];

	while (fgets(buf, sizeof buf, stdin)!=0) {

		// Get timestamp of record
		clock_gettime(CLOCK_REALTIME, &ts);

		if (strlen(s)==0) {
			continue;
		}

		printf ("%d.%09d %s\n", (int)ts.tv_sec, (int)ts.tv_nsec, buf);
		fflush(stdout);
	}


}
