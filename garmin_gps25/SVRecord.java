public class SVRecord extends Object {
   int cycles;   /* 32 bits Number of accululated cycles */
   Double pr;             /* 64 bits Pseudorange (meters)        */
   short phase;  /* 16 bits Phase angle (?) 0-2047 represents full cycle */
   byte slip;             /*  8 bits 0 = no cycle slip detected, 1 = slip */
   byte snr;     /*  8 bits Signal/Noise ratio (units??) */
   byte svid;    /*  8 bits SV id (0-31). Add 1 to get SV id*/
   boolean valid;   /*  8 bits 0 = invalid data, 1 = valid data */
   
   SVRecord () {
   }
   
} 
