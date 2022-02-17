#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PI 3.14159265359

/**
 * Decode UBX-NAV-PVT messages.
 * 
 * Input UBX message stream. 
 *
 * Output: one message per line. Columns: GPS-time-of-week, ISO timestamp, fixType, flags, flags2, lat, lng, alt
 *
 * fixType: 0 = no fix; 2 = 2D fix; 3 = 3D fix
 * flags: 0 = no carrier phase solution; 1 = float solution; 2 = fixed solution
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
	int32_t nano;


	// 0 = no-fix, 1 = dead-recon, 2 = 2D, 3=3D
	uint8_t fixType;

	// bits6:7 : 1 = FLOAT, 2 = FIX
	uint8_t flags;

	uint8_t flags2;

	uint8_t numSv;

	// x 1e-7
	int32_t lon;
	int32_t lat;

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
	int nano, second, minute, hour;

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

			// Remark: navpvt.nano can be negative
			// Still have a bug here:
			//492317000  2020-09-25T16:44:58.999Z  3 1 234   53.2825992 -8.9825108 88.294  
			//492318000  2020-09-25T16:45:-1.999Z  3 1 234   53.2825993 -8.9825106 88.298  
			//492319000  2020-09-25T16:45:00.999Z  3 1 234   53.2825995 -8.9825105 88.295
			
			hour = navpvt.hour;
			minute = navpvt.min;
			second = navpvt.sec;
			nano = navpvt.nano;
			
			if (nano < 0) {
				nano += 1000000000;
				second -= 1;
			}
			if (second < 0) {
				second += 60;
				minute -= 1;
			}
			if (minute < 0) {
				minute += 60;
				hour -= 1;
			}
			// TODO: trickle all the way up yo year!

				
			fprintf (stdout, "%u  %04d-%02d-%02dT%02d:%02d:%02d.%03dZ  %d %d %d   %.7f %.7f %.3f  \n", 
				navpvt.iTOW, 
				navpvt.year, navpvt.month, navpvt.day, hour, minute, second, (nano/1000000), 
				navpvt.fixType, navpvt.flags>>6, navpvt.flags2,
				navpvt.lat*1e-7, navpvt.lon*1e-7, navpvt.height*1e-3
				);
			fflush(stdout);
		}
		
	}

}
