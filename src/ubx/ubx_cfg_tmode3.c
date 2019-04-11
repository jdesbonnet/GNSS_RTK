#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.14159265359

/**
 * Enter base station mode using CFG-TMODE3
 *
 * Dependency: https://github.com/swift-nav/libswiftnav/
 *
 * 
 * @author Joe Desbonnet
 */

// UBX-CFG-TMODE3 payload structure. 
typedef struct {
	uint8_t version;
	uint8_t reserved0;	
	uint16_t flags;


	int32_t ecefXOrLat;
	int32_t ecefYOrLon;
	int32_t ecefZOrAlt;
} cfg_tmode3_t;

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


			fprintf (stdout, "t=%u  dur=%d  x=%d y=%d z=%d  acc=%d obs=%d valid=%d active=%d\n", 
				navsvin.iTOW, 
				navsvin.dur,
				navsvin.meanX, navsvin.meanY, navsvin.meanZ,
				navsvin.meanAcc, navsvin.obs, navsvin.valid,navsvin.active);
		}
		
	}

}
