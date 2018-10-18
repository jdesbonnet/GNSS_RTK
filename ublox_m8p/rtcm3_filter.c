#include <stdio.h>

/**
 * From input which can be a mix of uBlox binary (UBX), RTCM3
 * and NMEA0183 allow only RTCM3 messages to pass from stdin to stdout.
 * RTCM3 frame: [sync len payload crc]
 * sync: 0xD3
 * len: length of payload in bytes. 16 bits, only 10 least significant used.
 * payload
 * crc: 24 bits (CRC-24Q)
 */
int main (int argc, char **argv) {
	int i, len, c, prevc;
	while (!feof(stdin)) {
		c = fgetc(stdin);
		if (prevc == 0xD3 && (c&0xfc) == 0) {
			fputc(0xD3,stdout);
			fputc(c,stdout);
			len = c*256 + fgetc(stdin);
			fprintf (stderr," D3 %02x %02x", c, len&0xff);
			fputc(len&0xff,stdout);
			for (int i = 0; i < len+3; i++) {
				c = fgetc(stdin);
				fputc(c,stdout);
				fprintf (stderr," %02x", c);
			}
			fflush(stdout);	
			fprintf(stderr,"\n");
		}
		prevc = c;
	}

}
