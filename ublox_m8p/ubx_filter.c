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

			// Found UBX start of frame 
			fputc(0xB5,stdout);
			fputc(0x62,stdout);
			if (hexout) {
				fprintf (stderr,"\n**B5 62 ");
			}

			// Read class
			c = fgetc(stdin);
			fputc(c,stdout);
			if (hexout) {
				fprintf (stderr,"class=%02x ", c);
			}

			// Read ID
			c = fgetc(stdin);
			fputc(c,stdout);
			if (hexout) {
				fprintf (stderr,"id=%02x ", c);
			}

			// Read length
			len = fgetc(stdin) + fgetc(stdin)*256;
			if (hexout) {
				fprintf(stderr,"len=%d ",len);
			}
			// payload + 2 bytes CRC
			for (int i = 0; i < len+2; i++) {
				c = fgetc(stdin);
				fputc(c,stdout);
				if (hexout) {
					fprintf (stderr,"%02x ", c);
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
