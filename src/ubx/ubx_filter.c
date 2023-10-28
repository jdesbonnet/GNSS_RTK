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
	int i, len, c, prevc, msg_type;

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
			if (hexout) {
				fprintf (stdout,"B5 62 ");
			} else {
				fputc(0xB5,stdout);
				fputc(0x62,stdout);
			}


			// Read class
			c = fgetc(stdin);
			if (hexout) {
				fprintf (stdout,"%02X ", c);
			} else {
				fputc(c,stdout);
			}
			msg_type = c << 8;

			// Read ID
			c = fgetc(stdin);
			if (hexout) {
				fprintf (stdout,"%02X ", c);
			} else {
				fputc(c,stdout);
			}
			msg_type |= c;

			if (hexout) {
				switch (msg_type) {
					case 0x107: fprintf (stdout, " (NAV-PVT) "); break;
					case 0x213: fprintf (stdout, " (UBX-RXM-SFRBX) "); break;
					case 0x214: fprintf (stdout, " (RXM-RAW) "); break;
					case 0x215: fprintf (stdout, " (RXM-RAWX) "); break;
					case 0x500: fprintf (stdout, " (ACK-NAK) "); break;
					case 0x501: fprintf (stdout, " (ACK-ACK) "); break;
					default: fprintf (stdout, " (UNK=%X) ",msg_type); break;
				}
			}

			// Read length
			len = fgetc(stdin) + fgetc(stdin)*256;

			if (hexout) {
				fprintf(stdout,"len=%d ",len);
			} else {
				fputc(len & 0xff, stdout);
				fputc((len>>8) & 0xff, stdout);
			}

			// payload + 2 bytes CRC
			for (int i = 0; i < len+2; i++) {
				c = fgetc(stdin);
				if (hexout) {
					fprintf (stdout,"%02X ", c);
				} else {
					fputc(c,stdout);
				}
			}

			if (hexout) {
				fprintf(stdout,"\n");
			}

			fflush(stdout);	
		}
		prevc = c;
	}

}
