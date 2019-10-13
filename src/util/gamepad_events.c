#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <getopt.h>

#include <sys/time.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>


/**
 * Listen for gamepad events and echo to stdout in format <time> <event-type> <key-code>
 * eg 
 * 1570287474.679514 keydown 288
 * 1570287474.799524 keyup 288
 *
 * Joystick API: https://www.kernel.org/doc/Documentation/input/joystick-api.txt
 */

void usage (char *cmd) {
	fprintf (stderr,"%s <device>\n", cmd);
	fprintf (stderr," eg %s /dev/input/event12\n", cmd);
}


void main (int argc, char **argv) {
	FILE *f;
	struct input_event event;
	int use_js_api = 0;
	char *device;
	int index,c;


	static struct option longopts[] =
	{
		{"help", no_argument, 0, 'h'},
		{"joystick", no_argument, 0, 'j'},
	};


	while (1) {

		index = 0;

		c = getopt_long(argc, argv, "jh", longopts, &index);

		if (c == -1) {
			break;
		}

		switch (c) {

			case 'j': {
				use_js_api = 1;
				break;
			}

		}

	}


	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	device = argv[1]; 


	if (use_js_api) {
		fprintf(stderr,"using Joystick API\n");
	}

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
