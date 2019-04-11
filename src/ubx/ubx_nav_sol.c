#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "swiftnav/coord_system.h" 

#define PI 3.14159265359

/**
 * Decode UBX-NAV-SOL messages, write ECEF X,Y,Z and WGS84 lat, lng, alt
 *
 * Dependency: https://github.com/swift-nav/libswiftnav/
 *
 * UBX-NAV-SOL frame: [0xB5 0x62 0x01 0x06 [2 byte len] [52 byte nav_sol_t] ck_a ck_b]
 * 
 * @author Joe Desbonnet
 */

// UBX-NAV-SOL payload structure. 
// Ref: uBlox 6 Receiver Description, 35.9.1 Navigation Solution Information
typedef struct {
	uint32_t iTOW;

	int32_t fTOW;

	uint16_t week;

	// Fix type: 0=no fix, 1=dead-recon, 2=2D, 3=3D, 5=time-only
	uint8_t gpsFix;
	uint8_t flags;

	// ECEF coordinates (cm)
	int32_t ecefX;
	int32_t ecefY;
	int32_t ecefZ;

	// position accuracy (cm)
	uint32_t pAcc;

	// speed (cm/s)
	int32_t ecefVX;
	int32_t ecefVY;
	int32_t ecefVZ;

	// speed accuracy (cm/s)
	uint32_t sAcc;

	uint16_t pDOP;
	uint8_t reserved1;

	// number of SV in solution
	uint8_t numSV;

	uint8_t reserved2;
} nav_sol_t;

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


	nav_sol_t navsol;

	// ECEF in meters
	double ecef[3];

	// WGS84 coords (in radians)
	double llh[3];

	while (!feof(stdin)) {
		c = fgetc(stdin) & 0xff;

		shift<<=8;
		shift |= c;

		// Check for start of UBX-NAV-SOL message
		if (shift == 0xB5620106) {

			// Skip 2 byte length field
			c = fgetc(stdin);
			c = fgetc(stdin);

			// Read NAV-SOL payload
			fread (&navsol, sizeof(nav_sol_t), 1,stdin);

			// Convert to m (in cm in nav_sol_t)
			ecef[0] = navsol.ecefX / 100.0;
			ecef[1] = navsol.ecefY / 100.0;
			ecef[2] = navsol.ecefZ / 100.0;

			// Convert to WGS84 lat, lng, alt
			wgsecef2llh(ecef, llh);

			fprintf (stdout, "%u    %f %f %f    %f %f %f\n", navsol.iTOW, 
				ecef[0], ecef[1], ecef[2], 
				llh[0]*180/PI, llh[1]*180/PI, llh[2]);
		}
		
	}

}
