/* Code to read Garmin GPS 25/35LP binary output
 * V0.1
 * Joe Desbonnet, joe@wombat.ie
 * 4 Nov 1998
 *
 * Thanks to  Sam Storm van Leeuwen <samsvl@nlr.nl> for providing me 
 * with some sample Garmin binary data.
 * 
 * Usage: ./garminbin < garbin.dat
 * Output: none (its up to you to modify to write whatever you want out)
 * 
 * Currently tested only with Linux (RedHat 5.1 on Intel box).
 * 
 * BUGS:
 *  Endian issues... 
 *  Currently assumes that MSB is stored at the higher address (Intel style)
 *  Checksums not working. Checksums are defined in the manual as 
 *  "the addition of bytes between the delimiters should equal 0"
 */

#include <stdlib.h>
#include <stdio.h>

/* Special characters in data stream.  
 * Also used to identify data stream state.  */
#define DAT 0x01
#define ETX 0x03  /* ASCII ETX */
#define DLE 0x10  /* ASCII DLE */

/* Record types. */
#define POS_REC 0x28 /* Position Record Identifier */
#define RCV_REC 0x29 /* Receiver Record Identifier */
#define EPH_REC 0x2a /* Ephemeris recond (currently not implemented) */

#define RCV_REC_LEN 226

/*
 * These structures adapted from Garmin manual. Note that the following
 * architecture is assumed: short = 16 bits, int = 32 bits, double = 16 bits
 * char = 8 bits
 */

typedef struct {
   unsigned int cycles;   /* 32 bits Number of accululated cycles */
   double pr;             /* 64 bits Pseudorange (meters)        */
   unsigned short phase;  /* 16 bits Phase angle (?) 0-2047 represents full cycle */
   char slip;             /*  8 bits 0 = no cycle slip detected, 1 = slip */
   unsigned char snr;     /*  8 bits Signal/Noise ratio (units??) */
   unsigned char svid;    /*  8 bits SV id (0-31). Add 1 to get SV id*/
   unsigned char valid;   /*  8 bits 0 = invalid data, 1 = valid data */
} cpo_rcv_sv_type;

typedef struct {
   double rcvr_tow;        /* 64 bits Time of Week */
   short rcvr_wn;          /* 16 bits Week Number  */
   cpo_rcv_sv_type sv[12];
} cpo_rcv_type;

typedef struct {
   float alt;   /* ellipsoid altitude in meters */
   float epe;   /* estimated position error in meters */
   float eph;   /* horizontal position error in meters */
   float epv;   /* vertical position error in meters */
   short   fix;   /* 0=nofix, 1=nofix, 2=2D fix, 3=3D fix, 4=2Ddiff, 5=3Ddiff */
   double gps_tow; /* GPS time of week */
   /* note lat and lon seem to be mixed up in the Garmin 25LP manual (Rev C) */
   double lat;     /* Latidude in radians  */
   double lon;     /* Longitude in radians */
   float lon_vel;  /* Longitude velocity in meters/sec */
   float lat_vel;  /* Latidude velocity in meters/sec */
   float alt_vel;  /* Altidude velocity in meters/sec */
} cpo_pvt_type;

void display_record (cpo_rcv_type *);
void read_rcv_record (unsigned char *);
void read_pvt_record (unsigned char *);
int verify_checksum (void *memaddr, int len, int expected_checksum);
int retrieve_record (unsigned char *buf, int length, FILE *fp);
int get_data_byte (FILE *fp);

/* Globals: Used to save data stream state and chksum for get_data_byte() */
int state=DAT;
unsigned char chksum=0;

/* Main Program */
void main (int argc, char **argv) {
   unsigned char buf[512];
   int c,rec_len,rec_type,checksum,exit_loop=0;
   cpo_rcv_type rcv_rec;

   do {
      /* Read record type and act accordingly */

      fprintf (stderr,"Checksum = %x\n",(int)chksum);
      rec_type = get_data_byte(stdin);
      rec_len = get_data_byte(stdin);
      switch (rec_type) {
       case RCV_REC:
	 /* fprintf (stderr,"Receiver record found.\n"); */
	 retrieve_record(buf,rec_len,stdin);
	 read_rcv_record(buf);
	 break;
       case POS_REC:
	 /* fprintf (stderr,"Position record found.\n"); */
	 retrieve_record(buf,rec_len,stdin);
	 read_pvt_record(buf);
	 break;
       case EPH_REC:
	 /* fprintf (stderr,"Ephemeris record found.\n"); */
	 retrieve_record(buf,rec_len,stdin);
	 break;
	 
       default:
	 fprintf (stderr,"Unrecognized record type %d\n",c);
	 retrieve_record(buf,rec_len,stdin);
	 exit(2);
	 break;
      }
   
   } while (exit_loop==0);
   fprintf (stderr, "Program terminated.\n");
}

int retrieve_record (unsigned char *buf, int length, FILE *fp) 
{
   unsigned char sum=0,checksum,c;
   int i;

   /* reset checksum counter used in get_data_byte (this is a global!) */
   chksum=0;
   
   for (i=0; i<length; i++) {
      buf[i] = get_data_byte (fp);
      /* fprintf (stderr,"%02X  ",buf[i]); */
   }
   
   checksum = get_data_byte (fp);
   
}


/* Read receiver record from a memory buffer to an
 * instance of the cpo_rcv_type structure.
 */
void read_rcv_record (unsigned char *buf)
{
   int i,offset;

   double *tow,*pseudorange;
   short  *wn,*phase;
   int    *cycles;
   
   cpo_rcv_type rcv_rec;
   
   tow = (double *)&buf[0];
   wn = (short *)&buf[8];
   for (i=0; i<12; i++) {
      offset = 10+i*18;
      rcv_rec.sv[i].cycles = *((int *)&buf[offset]);
      rcv_rec.sv[i].pr = *((double *)&buf[offset + 4]);
      rcv_rec.sv[i].phase = *((short *)&buf[offset + 12]);
      rcv_rec.sv[i].slip = buf[offset+14];
      rcv_rec.sv[i].snr  = buf[offset+15];
      rcv_rec.sv[i].svid = buf[offset+16];
      rcv_rec.sv[i].valid = buf[offset+17];

      if (rcv_rec.sv[i].valid ==0) {
	 continue;
      }

      /*
      fprintf (stderr,"%1d sv[%2d] SV=%2d cycles=%10d phase=%4d pr=%16lf snr=%2d\n",
	      rcv_rec.sv[i].valid,
	      i,
	      rcv_rec.sv[i].svid,
	      rcv_rec.sv[i].cycles, 
	      (int)rcv_rec.sv[i].phase,
	      rcv_rec.sv[i].pr,
	      (int)rcv_rec.sv[i].snr);
       */
   }
}

/* Read PVT (Position Velocity Time) record into an instance of the
 * cpo_pvt_type structure
 */
void read_pvt_record (unsigned char *buf)
{
   cpo_pvt_type pvt_rec;

   pvt_rec.alt = *((float *)&buf[0]);
   pvt_rec.epe = *((float *)&buf[4]);
   pvt_rec.eph = *((float *)&buf[8]);
   pvt_rec.epv = *((float *)&buf[12]);
   pvt_rec.fix = *((short *)&buf[16]);
   pvt_rec.gps_tow = *((double *)&buf[18]);
   pvt_rec.lon = *((double *)&buf[26]);
   pvt_rec.lat = *((double *)&buf[34]);
   pvt_rec.lon_vel = *((float *)&buf[42]);
   pvt_rec.lat_vel = *((float *)&buf[46]);
   pvt_rec.alt_vel = *((float *)&buf[50]);

   /*
   fprintf (stderr,"alt=%10f epe=%10f eph=%10f epv=%10f\n",
	    pvt_rec.alt,
	    pvt_rec.epe,
	    pvt_rec.eph,
	    pvt_rec.epv);
    */
   printf ("%lf %d %lf %lf %f %f %f\n",
	   pvt_rec.gps_tow,
	   pvt_rec.fix,
	   pvt_rec.lat*180,pvt_rec.lon*180,pvt_rec.alt,
	   pvt_rec.lat_vel, pvt_rec.lon_vel);
   
}


void display_record (cpo_rcv_type *rcv_rec)
{
   int i;
   cpo_rcv_sv_type sv_rec[12];
   
   fprintf (stderr,"display_record::\n");
   printf ("Receiver time of week: %lf\n",rcv_rec->rcvr_tow);
   printf ("Receiver week number: %d\n",rcv_rec->rcvr_wn);
   
   for (i=0; i<12; i++) {
      printf ("SV=%2d: %10d %10lf %4d\n",
	      (int)rcv_rec->sv[i].svid,
	      (int)rcv_rec->sv[i].cycles,
	      (double)rcv_rec->sv[i].pr,
	      (int)rcv_rec->sv[i].phase);
   }
   
	      
}

/*
 * Read next byte from the data stream taking care of the DLE and ETX
 * escaping.
 * 
 * Uses two global integers: chksum for checksum calculcations and
 * 'state' to preserve data stream state. Don't particularly like using
 * globals but 'till do for the moment.
 * 
 * This code is taken almost verbatin from the Garmin GPS 25LP manual.
 */

int get_data_byte (FILE *fp) 
{
   int c;
   while (!feof(fp)) {
      c = getc(stdin);
      chksum +=c ;
      if (state == DAT) {
	 if (c == DLE) {
	    state=DLE;
	 } else {
	    return c;
	 }
      } else if (state == DLE) {
	 if (c == ETX) {
	    state=ETX;
	 } else {
	    state=DAT;
	    return c;
	 }
      } else if (state == ETX) {
	 if (c == DLE) {
	    state=DLE;
	 }
      }
   }
}
