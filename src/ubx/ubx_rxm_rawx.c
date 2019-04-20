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
	double pr;
	double cp;
	float doppler;
	uint8_t gnssId;
	uint8_t svId;
	uint8_t sigId;
	uint8_t gloFreqId;
	uint16_t locktime;
	uint8_t cno;
	uint8_t prstd;
	uint8_t cpstd;
	uint8_t trkStat;
	uint8_t reserved2;
} ubx_rxm_rawx_meas_t;

typedef struct {
	double rcvTow;
	uint16_t week;
	uint8_t leapSeconds;
	uint8_t numMeas;
	uint8_t recvStat;
	uint8_t version;
	uint8_t reserved[2];
} ubx_rxm_rawx_header_t;

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


	ubx_rxm_rawx_header_t rxm_rawx_header;
	ubx_rxm_rawx_meas_t rxm_rawx_meas[128];



	while (!feof(stdin)) {
		c = fgetc(stdin) & 0xff;

		shift<<=8;
		shift |= c;

		// Check for start of UBX-RXM-RAWX message
		if (shift == 0xB5620215) {

			// Skip 2 byte length field
			len = fgetc(stdin);
			len |= fgetc(stdin)<<8;

			fprintf (stderr,"len=%d nmeas=%d\n", len, (len-16)/32);

			// Read RXM-RAWX header
			fread (&rxm_rawx_header, sizeof(ubx_rxm_rawx_header_t), 1,stdin);

			fprintf (stderr,"tow=%f nmeas=%d\n", rxm_rawx_header.rcvTow, rxm_rawx_header.numMeas);

			fread (rxm_rawx_meas, sizeof(ubx_rxm_rawx_meas_t), rxm_rawx_header.numMeas,stdin);

			for (i = 0; i < rxm_rawx_header.numMeas; i++) {
				fprintf(stdout, "%d %d %d  %f %f %f\n", 
					rxm_rawx_meas[i].svId,
					rxm_rawx_meas[i].gnssId,
					rxm_rawx_meas[i].trkStat,
					rxm_rawx_meas[i].pr,
					rxm_rawx_meas[i].cp,
					rxm_rawx_meas[i].doppler
		
				);
			}

		}
		
	}

}
