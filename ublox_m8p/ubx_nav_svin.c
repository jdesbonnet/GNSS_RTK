#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.14159265359

/**
 * Decode UBX-NAV-SVIN messages
 *
 * Dependency: https://github.com/swift-nav/libswiftnav/
 *
 * UBX-NAV-SOL frame: [0xB5 0x62 0x01 0x3B [2 byte len] [????] ck_a ck_b]
 * 
 * @author Joe Desbonnet
 */

// UBX-NAV-SVIN payload structure. 
typedef struct {
	uint32_t reserved;

	uint32_t iTOW;

	uint32_t dur;

	// ECEF coordinates (cm)
	int32_t meanX;
	int32_t meanY;
	int32_t meanZ;

	int8_t meanXHP;

} nav_svin_t;

int main (int argc, char **argv) {
	int i, len, c, prevc;

	int hexout = 0;
	for (i = 0; i < argc; i++) {
		if (argv[i][0]=='-' && argv[i][1]=='v') {
			hexout=1;
		}
	}

	// Shift register to look for start of UBX-NAV-SOL pattern
	uint32_t shift=0;


	nav_svin_t navsvin;


	while (!feof(stdin)) {
		c = fgetc(stdin) & 0xff;

		shift<<=8;
		shift |= c;

		// Check for start of UBX-NAV-SVIN message
		if (shift == 0xB562013B) {

			// Skip 2 byte length field
			c = fgetc(stdin);
			c = fgetc(stdin);

			// Read NAV-SOL payload
			fread (&navsvin, sizeof(nav_svin_t), 1,stdin);


			fprintf (stdout, "%u  %d  %f %f %f %d\n", 
				navsvin.iTOW, 
				navsvin.dur,
				navsvin.meanX, navsvin.meanY, navsvin.meanZ,
				navsvin.meanXHP);
		}
		
	}

}
