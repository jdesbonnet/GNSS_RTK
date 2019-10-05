#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>


/**
 * Listen for gamepad events and echo to stdout in format <time> <event-type> <key-code>
 * eg 
 * 1570287474.679514 keydown 288
 * 1570287474.799524 keyup 288
 */

void usage (char *cmd) {
	fprintf (stderr,"%s <device>\n", cmd);
	fprintf (stderr," eg %s /dev/input/event12\n", cmd);
}


void main (int argc, char **argv) {
	FILE *f;
	struct input_event event;
	char *device;

	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	device = argv[1]; 

	f = fopen (device,"rb");

	if (! f) {
		fprintf (stderr, "error opening device %s", device);
		exit(EXIT_FAILURE);
	}

	while (!feof(f)) {
		fread (&event, sizeof(event), 1, f);
		//fprintf (stdout,"raw %ld.%06ld %d %d %d\n",event.time.tv_sec, event.time.tv_usec, event.type, event.code, event.value);

		if (event.type == EV_KEY) {
			fprintf (stdout, "%ld.%06ld %s %d\n",  event.time.tv_sec, event.time.tv_usec, (event.value == 1 ? "keydown" : "keyup"), event.code);
		}
		fflush(stdout);
	}
}
