#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.14159265359

/**
 * Decode UBX-NAV-PVT messages, write WGS84 lat, lng, alt
 *
 * UBX-NAV-PVT frame: [0xB5 0x62 0x01 0x07 [2 byte len] [92 byte nav_pvt_t] ck_a ck_b]
 * 
 * @author Joe Desbonnet
 */

// UBX-NAV-PVT payload structure. 
// Ref UBlox F9P protocol description 5.14.11 UBX-NAV-PVT (0x01 0x07)

typedef struct {
	uint32_t iTOW;

	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;

	uint8_t valid;

	uint32_t tAcc;
	uint32_t nano;

	uint8_t fixType;

	uint8_t flags;
	uint8_t flags2;

	uint8_t numSv;

	// x 1e-7
	int32_t lat;
	int32_t lon;

	// mm
	int32_t height;

	int32_t hMSL;

	uint32_t hAcc;
	uint32_t vAcc;


	// mm/s
	int32_t velN;
	int32_t velE;
	int32_t velD;

	int32_t gSpeed;

	// heading of motion x 1e-5
	int32_t headMot;

	uint32_t sAcc;
	uint32_t headAcc;

	uint16_t pDOP;
	uint8_t reserved1[6];

	// x 1e-5
	int32_t headVeh;

	// x 1e-2
	int16_t magDec;
	uint16_t magAcc;
} nav_pvt_t;

int main (int argc, char **argv) {
	int i, len, c, prevc;

	int hexout = 0;
	for (i = 0; i < argc; i++) {
		if (argv[i][0]=='-' && argv[i][1]=='v') {
			hexout=1;
		}
	}

	// Shift register to look for start of frame pattern
	uint32_t shift=0;


	nav_pvt_t navpvt;


	while (!feof(stdin)) {
		c = fgetc(stdin) & 0xff;

		shift<<=8;
		shift |= c;

		// Check for start of UBX-NAV-PVT message
		if (shift == 0xB5620107) {

			// Skip 2 byte length field
			c = fgetc(stdin);
			c = fgetc(stdin);

			// Read NAV-PVT payload
			fread (&navpvt, sizeof(nav_pvt_t), 1,stdin);

			fprintf (stdout, "%u    %f %f %f  \n", navpvt.iTOW, 
				navpvt.lat/1e-7, navpvt.lon/1e-7, navpvt.height/1e-3
				);
		}
		
	}

}
