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
	uint8_t reserved1;
	
	/* bits 0:7 mode (0 = disabled, 1 = survey in, 2 = position supplied); 
	 * bit 8: pos in lat,lon,alt, else ECEF X,Y,Z used */
	uint16_t flags;   


	int32_t ecefXOrLat;
	int32_t ecefYOrLon;
	int32_t ecefZOrAlt;


	int8_t ecefXOrLatHP;
	int8_t ecefYOrLonHP;
	int8_t ecefZOrAltHP;
	uint8_t reserved2;

	uint32_t fixedPosAcc; /* fixed position 3D accuracy 0.1mm units */
	uint32_t svinMinDur;  /* Survey-in minimum duration seconds */
	uint32_t svinAccLimit; 


	uint8_t reserved3[8];

} cfg_tmode3_t;

uint32_t ubx_checksum (uint8_t payload[], int length) {
	int i;
	uint32_t A=0;
	uint32_t B=0;

	for (i = 0; i < length; i++) {
		A += payload[i];
		B += A;
	}

	A &= 0xff;
	B &= 0xff;

	return ((A<<8)|B);
}

int main (int argc, char **argv) {
	int i, len, c, prevc;

	// Initialize all zero
	cfg_tmode3_t tmode3 = {0};

	tmode3.flags = 1;
	tmode3.svinMinDur = 128;
	tmode3.svinAccLimit = 16*10000;






	struct {
		uint8_t msg_class;
		uint8_t msg_id;
		uint16_t length;
		cfg_tmode3_t payload;
	} ubx_frame;
	ubx_frame.msg_class = 0x06;
	ubx_frame.msg_id    = 0x71;
	ubx_frame.length = sizeof(tmode3);
	ubx_frame.payload = tmode3;

	uint8_t *buf = (uint8_t *)&ubx_frame;

	uint32_t checksum = ubx_checksum(buf, sizeof(ubx_frame));

	for (int i = 0; i < sizeof(ubx_frame); i++) {
		fprintf (stdout, "%02x ", buf[i]);
	}
	fprintf (stdout, "checksum=%x",checksum);
	fprintf (stdout,"\n");
}
