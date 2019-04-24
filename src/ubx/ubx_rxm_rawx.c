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


static uint32_t A,B;

void checksum_reset () {
	A = B = 0;
}
void checksum_update (uint8_t c) {
	A += c;
	B += A;
}
void checksum_update_block(uint8_t *buf, int len) {
	int i = 0;
	for (i = 0; i < len; i++) {
		checksum_update(*buf);
		buf++;
	}
}
uint16_t checksum_get() {
	return ((A&0xff)<<8) | (B&0xff);	
}

int main (int argc, char **argv) {
	int i, len, c, prevc;
	uint16_t checksum;
	uint32_t frame_count = 0;
	uint32_t checksum_fail_count = 0;

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

			frame_count++;

			checksum_reset();
			checksum_update(0x02);
			checksum_update(0x15);

			c = fgetc(stdin);
			checksum_update(c);
			len = c;
			c = fgetc(stdin);
			checksum_update(c);
			len |= c<<8;

			//fprintf (stderr,"len=%d nmeas=%d\n", len, (len-16)/32);

			// Read RXM-RAWX header
			fread (&rxm_rawx_header, sizeof(ubx_rxm_rawx_header_t), 1, stdin);
			checksum_update_block((uint8_t*)&rxm_rawx_header,sizeof(ubx_rxm_rawx_header_t));

			//fprintf (stdout,"tow=%f nmeas=%d\n", rxm_rawx_header.rcvTow, rxm_rawx_header.numMeas);

			// TODO: check if numMeas>128

			fread (&rxm_rawx_meas, sizeof(ubx_rxm_rawx_meas_t), rxm_rawx_header.numMeas, stdin);
			checksum_update_block((uint8_t*)&rxm_rawx_meas, sizeof(ubx_rxm_rawx_meas_t) * rxm_rawx_header.numMeas);

			// Read frame checksum and compare with computed value
                        checksum = fgetc(stdin)<<8;
                        checksum |= fgetc(stdin);

			if (checksum != checksum_get()) {
				fprintf (stderr,"ERROR checksum mismatch: %x vs %x\n", checksum, checksum_get());
				checksum_fail_count++;
				continue;
			}


			for (i = 0; i < rxm_rawx_header.numMeas; i++) {
				fprintf(stdout, "%f sv=%d gnssId=%d %d  %f %f %f\n", 
					rxm_rawx_header.rcvTow,
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

	fprintf (stderr,"frames=%d checksum_fail_count=%d\n", frame_count, checksum_fail_count);

}
