#include <stdio.h>

/**
 * From input which can be a mix of uBlox binary (UBX), RTCM3
 * and NMEA0183 allow only NMEA0183 messages to pass from stdin to stdout.
 */
int main (int argc, char **argv) {
	int i, len, c, prevc;
	while (!feof(stdin)) {
		c = fgetc(stdin);
		if (prevc == '$' && c == 'G') {
			fputc('$',stdout);
			fputc('G',stdout);
			while  ( (c = fgetc(stdin)) != '*')  {
				fputc(c,stdout);
			}
			fputc ('*',stdout);
			fputc (fgetc(stdin),stdout);
			fputc (fgetc(stdin),stdout);
			fputc ('\n',stdout);
			fflush(stdout);	
		}
		prevc = c;
	}

}
