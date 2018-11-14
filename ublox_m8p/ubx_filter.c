#include <stdio.h>

/**
 * From input which can be a mix of uBlox binary (UBX), RTCM3
 * and NMEA0183 allow only proper UBX messages to pass from stdin to stdout.
 * UBX frame: [sync class id len payload crc]
 * sync: 0xB5 0x62
 * class: 8 bits
 * id: 8 bits
 * len: 16 bits: length of payload in bytes. Little endian
 * payload
 * crc: 16 bits
 */
int main (int argc, char **argv) {
	int i, len, c, prevc;

	int hexout = 0;
	for (i = 0; i < argc; i++) {
		if (argv[i][0]=='-' && argv[i][1]=='v') {
			hexout=1;
		}
	}

	while (!feof(stdin)) {
		c = fgetc(stdin);
		if (prevc == 0xB5 && c == 0x62) {
			fputc(0xB5,stdout);
			fputc(0x62,stdout);
			if (hexout) {
				fprintf (stderr,"B5 62 ");
			}
			c = fgetc(stdin);
			fputc(c,stdout);
			if (hexout) {
				fprintf (stderr,"%02x ", c);
			}
			c = fgetc(stdin);
			fputc(c,stdout);
			if (hexout) {
				fprintf (stderr,"%02x ", c);
			}
			len = fgetc(stdin)*256 + fgetc(stdin);
			// payload + 2 bytes CRC
			for (int i = 0; i < len+2; i++) {
				c = fgetc(stdin);
				fputc(c,stdout);
				if (hexout) {
					fprintf (stderr," %02x", c);
				}
			}
			fflush(stdout);	
			if (hexout) {
				fprintf(stderr,"\n");
				fflush(stderr);
			}
		}
		prevc = c;
	}

}
