/*****************************************************************************/
/* cat_sp3.c                           Written by Bob Dulaney, May 1998      */
/*                                                                           */
/* Program to splice two sequential, same-interval SP3 orbit files, where    */
/* exactly one epoch interval separates the last epoch of file 1 from the    */
/* first epoch of file 2.  Satellites unique to one file are set to 0.000000 */
/* where necessary in the joined file, and satellites out of sequence in one */
/* file are sorted into ascending order in the joined file, so they can      */
/* share the same header.  Number of epochs is updated in the header.        */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define DEBUG0  0
#define DEBUG1  1

#define MAX_PRN_ID    35

void sub_str(char input_string[], char output_string[], int first, int last);
void check_header(int index, FILE *ifp[], int num_epochs[],
     double interval[], long start_mjd[], double start_fmjd[], int num_prns[],
     int sat_ids[][MAX_PRN_ID+1], int out_of_order_flag[]);
void write_header(int num_epochs[], int num_prns[], int sat_ids[][MAX_PRN_ID+1],
     FILE *ifp[], FILE *ofp);
void write_epochs(int index, int num_epochs[], int num_prns[],
     int sat_ids[][MAX_PRN_ID+1], FILE *ifp[], FILE* ofp);

main()
{
   int    j, k, index, num_epochs[3] = {0}, num_prns[3] = {0};
   int    sat_ids[3][MAX_PRN_ID + 1] = {0};
   int    out_of_order_flag[2] = {0};
   long   ii;
   long   start_mjd[3];
   double start_fmjd[3], interval[3];
   double eps = 1e-09;           /* Figure eps will be used to compare MJD's */
                                 /* with 15 significant figures, an example  */
                                 /* being 50738.0000000001. Also, note that  */
                                 /* 1 second = .000011574 days.              */

   char   orbit_file_name[60], sp3_line[82]; /* [81] is one too few, because */
                                             /* you need a newline plus '\0' */
   FILE  *ifp[2];
   FILE  *ofp;

   /*                                                                        */
   /* Open orbit files 1 and 2, associating file 1 with FILE *ifp[0] and     */
   /* file 2 with FILE *ifp[1].                                              */
   /*                                                                        */

   for (index = 0; index <= 1; index++) {{
      printf("Enter orbit file %d : ", index + 1);
      gets(orbit_file_name);

   /* Remove trailing blanks from orbit file name.  (Leading blanks  */
   /* will still cause problems.)                                    */
      while(orbit_file_name[strlen(orbit_file_name) - 1] == ' ')
         orbit_file_name[strlen(orbit_file_name) - 1] = '\0';

      ifp[index] = fopen(orbit_file_name, "r");
      if(ifp[index] == NULL) {
         printf("Orbit file %d not found....bye!\n", index + 1);
         exit(2);
      }

      /*                                                             */
      /* Scan header lines in FILE *ifp[0] and FILE *ifp[1].         */
      /*                                                             */

      check_header(index, ifp, num_epochs, interval, start_mjd, start_fmjd,
                   num_prns, sat_ids, out_of_order_flag);

#if DEBUG0
      printf("\nDEBUG: num_epochs as an int = %d\n", num_epochs[index]);
      printf("DEBUG: interval as a double = %f\n", interval[index]);
      printf("DEBUG: start_mjd as a long = %ld\n", start_mjd[index]);
      printf("DEBUG: start_fmjd as a double = %f\n", start_fmjd[index]);
      printf("DEBUG: num_prns as an int = %d\n", num_prns[index]);
#endif

   }}

   /* Check that the sp3 files are suitable for splicing, i.e., that        */
   /* (1) the epoch intervals are the same, and                             */
   /* (2) the start MJD of orbit2 equals the start MJD of orbit1 plus       */
   /*     the common epoch interval times the number of orbit1 epochs.      */
   /*    (Or equivalently, that the start MJD of orbit2 equals the          */
   /*     MJD of the last orbit1 epoch plus the common epoch interval.)     */
   /*                                                                       */

   if(fabs(interval[1] - interval[0]) > eps) {
      printf("\nFiles cannot be joined.  Epoch intervals not the same.\n");
      exit(1);
   }
   if(fabs((start_mjd[1] - start_mjd[0]) + (start_fmjd[1] - start_fmjd[0]) -
           ((interval[0] * num_epochs[0]) / 86400.)) > eps) {
      printf("\nFiles cannot be joined.  First epoch of file 2 must be\n");
      printf("one epoch interval later than the last epoch of file 1,\n");
      printf("based on start MJD, epoch count and interval in header.\n");
      exit(1);
   }

   /* If the input files are suitable for splicing, warn the user if one or */
   /* both files are not in ascending PRN order, based on their headers.    */

   if(out_of_order_flag[0] == 1 || out_of_order_flag[1] == 1) {{
      printf("\n");
      for (index = 0; index <= 1; index++) {
         if(out_of_order_flag[index] == 1)
            printf("Satellites out-of-order in file %d header.\n", index+1);
      }
      printf("Output will be sorted by PRN number.\n");
   }}

   /* Merge sat_ids[0] and sat_ids[1] into sat_ids[2] and count the number  */
   /* of combined satellites.                                               */
   /* Note: The check that all satellite id's are less than or equal to     */
   /* MAX_PRN_ID was made in function check_header(), so no check is neces- */
   /* sary below on num_prns[2].                                            */

   num_prns[2] = 0;            /* Was so initialized in declaration, above. */
   for(j = 1; j <= MAX_PRN_ID; j++) {{ /* Start j at 1, so sv_id = 0 does   */
                                       /* not increment num_prns[2], below. */
      if(sat_ids[0][j] == j || sat_ids[1][j] == j) {
         sat_ids[2][j] = j;
         num_prns[2]++;
      }
   }}

   /* Warn the user if a satellite was found in one file and not the other. */

   if(num_prns[0] < num_prns[2] || num_prns[1] < num_prns[2]) {{
      printf("\n");
      for(j = 0; j <= 1; j++) {
         if(num_prns[j] < num_prns[2]) {
            for(k = 1; k <= MAX_PRN_ID; k++) {
               if(sat_ids[j][k] != k && sat_ids[2][k] == k) {
                  printf("Missing positional values for SV%02d will be set \n"
                         "to 0.0 in \"orbit file %d\" part of joined file.\n",
                         k, j + 1);
               }
            }
         }
      }
      printf("[Per NOAA TM NOS 133 NGS 46: \"Extending the NGS \n"
             "Standard GPS Orbit Formats\", page 69.] \n");
   }}
#if DEBUG0
   for(index = 0; index <= 2; index++) {
      printf("\nDEBUG: num_prns as an int = %d\n", num_prns[index]);
      for(j = 0; j <= MAX_PRN_ID; j++)
         printf("DEBUG: sat_ids[%d][%2d] = %2d\n", index, j, sat_ids[index][j]);
   }
#endif

   /* Determine the combined number of epochs, num_epochs[2], as well as    */
   /* the start MJD for the combined orbit.                                 */

   num_epochs[2] = num_epochs[0] + num_epochs[1];
   interval[2] = interval[0];
   start_mjd[2] = start_mjd[0];
   start_fmjd[2] = start_fmjd[0];
/* num_prns[2] was determined above, when merged sat_ids[0] and sat_ids[1]. */

   /*                                                                        */
   /* Prompt user for name of joined SP3 orbit file ("cat.orb" by default).  */
   /* Write header, with updates, to file "cat.orb".  Complete joined orbit  */
   /* by writing Epoch Header and Satellite Position records, first from     */
   /* FILE *ifp[0] and second from FILE *ifp[1].                             */

   printf("\nEnter output name, or\n");
   printf("<CR> for \"cat.orb\" : ");
   gets(orbit_file_name);
   if(strcmp(orbit_file_name, "") == 0)
      strcpy(orbit_file_name, "cat.orb");
   ofp = fopen(orbit_file_name, "w");

   write_header(num_epochs, num_prns, sat_ids, ifp, ofp);

   for(index = 0; index <= 1; index++)
      write_epochs(index, num_epochs, num_prns, sat_ids, ifp, ofp);

   fprintf(ofp, "EOF\n");

}

/* This function returns a substring of a given string.  It is
   passed the longer string, the name of the desired substring,
   and the first and last elements of the desired substring. */

void sub_str(char input_string[], char output_string[], int first, int last)
{
   int i, j;

   for(i=first, j=0; i<=last; ++i, ++j)
      output_string[j] = input_string[i];
   output_string[j] = '\0';
}

void check_header(int index, FILE *ifp[], int num_epochs[],
     double interval[], long start_mjd[], double start_fmjd[], int num_prns[],
     int sat_ids[][MAX_PRN_ID+1], int out_of_order_flag[])
{
   long ii;
   char s1[82], sp3_line[82];
   int  j = 1, k;
   int previous_sv_id = 0, sv_id = 0;

#if DEBUG0
   printf("DEBUG: Got to top of check_header()\n");
#endif
   while(fgets(sp3_line, 82, ifp[index]) != NULL) {
      if(sp3_line[0] == '*' || sp3_line[0] == 'P')
         break;
      if(sp3_line[0] == '#' && sp3_line[1] != '#') {
         sub_str(sp3_line, s1, 32, 38);
   /*    printf("DEBUG: num_epochs as a string = %s\n", s1);                 */
         num_epochs[index] = atoi(s1);
      }
      if(sp3_line[0] == '#' && sp3_line[1] == '#') {
         sub_str(sp3_line, s1, 24, 37);
   /*    printf("DEBUG: interval as a string = %s\n", s1);                   */
         interval[index] = atof(s1);
         sub_str(sp3_line, s1, 39, 43);
   /*    printf("DEBUG: start_mjd as a string = %s\n", s1);                  */
         start_mjd[index] = atol(s1);
         sub_str(sp3_line, s1, 45, 59);
   /*    printf("DEBUG: start_fmjd as a string = %s\n", s1);                 */
         start_fmjd[index] = atof(s1);
      }
      if(sp3_line[0] == '+' && sp3_line[1] == ' ') {
         if(isdigit(sp3_line[4]) || isdigit(sp3_line[5])) {
            sub_str(sp3_line, s1, 4, 5);
#if DEBUG0
            printf("DEBUG: num_prns as a string = %s\n", s1);                  
#endif
            num_prns[index] = atoi(s1);
         }
         for(k = 9; j <= num_prns[index] && k <= 57; j++, k = k + 3) {
            sub_str(sp3_line, s1, k, k+2);
#if DEBUG0
            printf("DEBUG: sat_id as a string = %s\n", s1);                    
#endif
            sv_id = atoi(s1);
            if(sv_id > MAX_PRN_ID) {
               printf("\nSV%02d, orbit file %d, exceeds MAX_PRN_ID, "
                      "set at %d.\n", sv_id, index + 1, MAX_PRN_ID);
               exit(1);
            }
            else if(sv_id <= previous_sv_id) {
            /* printf("( WARNING. Satellites out-of-order within )\n"       */
            /*        "( orbit file %d. SV%02d is followed by SV%02d. )\n", */
            /*        index + 1, previous_sv_id, sv_id);                    */
               out_of_order_flag[index] = 1;

            /* exit(1);  <==>  previously fatal, in older program "cat_orb" */
            }
                                           /* instead of initialized */
            sat_ids[index][sv_id] = sv_id; /* array value of 0.      */
            previous_sv_id = sv_id;
         }
      }
   }
}

void write_header(int num_epochs[], int num_prns[], int sat_ids[][MAX_PRN_ID+1],
     FILE *ifp[], FILE *ofp)
{
   char s1[82], s2[82], sp3_line[82];
   int i, j;
   int plus_array[17*5 + 1] = {0};
   int plusplus_array[17*5 + 1] = {0};

   rewind(ifp[0]);
   fgets(sp3_line, 82, ifp[0]);
   sub_str(sp3_line, s1, 0, 31);
   sub_str(sp3_line, s2, 39, 59);
   fprintf(ofp, "%s%7d%s\n", s1, num_epochs[2], s2);
   fgets(sp3_line, 82, ifp[0]);
   fprintf(ofp, "%s", sp3_line);

   plus_array[0] = num_prns[2];
   for(i = 1, j = 1; i < 17*5 + 1; i++, j++) {
      while(j <= MAX_PRN_ID && sat_ids[2][j] == 0)
         j++;
      if(j > MAX_PRN_ID)
         break;
      else
         plus_array[i] = sat_ids[2][j];
   }

   fprintf(ofp, "+   %2d   ", plus_array[0]);
   for(i = 1; i <= 17; i++)
      fprintf(ofp, "%3d", plus_array[i]);
   fprintf(ofp, "\n");
   for(j = 1; j <= 4; j++) {
      fprintf(ofp, "+        ");
      for(i = j*17 + 1; i <= j*17 + 17; i++)
         fprintf(ofp, "%3d", plus_array[i]);
      fprintf(ofp, "\n");
   }

   for(j = 0; j <= 4; j++) {
      fprintf(ofp, "++       ");
      for(i = j*17 + 1; i <= j*17 + 17; i++)
         fprintf(ofp, "%3d", plusplus_array[i]);
      fprintf(ofp, "\n");
   }

   for(i = 0; i <= 1; i++)
      fprintf(ofp, "%%c CC CC CCC CCC CCCC CCCC CCCC CCCC"
                   " CCCCC CCCCC CCCCC CCCCC\n");

   for(i = 0; i <= 1; i++)
      fprintf(ofp, "%%f   .0000000   .000000000   .00000000000"
                   "   .000000000000000\n");

   for(i = 0; i <= 1; i++)
      fprintf(ofp, "%%i    0    0    0    0      0      0"
                   "      0      0         0\n");

   fprintf(ofp, "/* NOTE: CONCATENATED SP3 FILE CREATED WITH PROGRAM "
                "CAT_SP3 \n");
   for(i = 0; i <= 2; i++) {
      fprintf(ofp, "/* ");
      for(j = 1; j <= 57; j++)
         fprintf(ofp, "C");
      fprintf(ofp, "\n");
   }
}

/* After writing out header, write Epoch Header and Satellite Position   */
/* records from FILE *ifp[0] and FILE *ifp[1] to complete joined orbit.  */
/* Confirm that num_epochs[index] from each input orbit file's header    */
/* matches total number of cc1='*' Epoch Header records. Within each     */
/* individual epoch, confirm that num_prns[index] matches number of      */
/* cc1='P' Satellite Position records. Sort out-of-order Satellite       */
/* Position records by reading each epoch individually into an array     */
/* of type struct P_line, dimensioned at MAX_PRN_ID + 1. Write out       */
/* Position records from this array, in ascending PRN order. Fill in     */
/* missing Satellite Position records where needed, e.g., in the first   */
/* half of the joined orbit file if the satellite was in orbit file 2    */
/* but not 1. (Absent positional values are set to 0.000000 and absent   */
/* clock values are set to 999999.999999, per pg. 69 of "Extending the   */
/* National Geodetic Survey Standard GPS Orbit Formats", NOAA TM NOS     */
/* 133 NGS 46.)                                                          */

void write_epochs(int index, int num_epochs[], int num_prns[],
     int sat_ids[][MAX_PRN_ID+1], FILE *ifp[], FILE* ofp)
{
   typedef struct {
      int  prn;
      char cc5_cc60[57];
   } P_Line;

   P_Line p_lines[MAX_PRN_ID + 1] = {0};

   int  i, j;
   int  epoch_cnt = 0, sat_cnt = 0, sv_id;
   char s1[82], sp3_line[82];

   rewind(ifp[index]);

   while(fgets(sp3_line, 82, ifp[index]) != NULL)
      if(sp3_line[0] == '*')   
         break;    /* Skip header, stopping at first Epoch Header record. */

   while(sp3_line[0] == '*') {{
      fprintf(ofp, "%s", sp3_line);      /* Print Epoch Header record. */
      epoch_cnt++;
      sat_cnt = 0;

   /* Until next Epoch Header record or EOF, read PRN Position records     */
   /* into a structure so they can be printed later in member prn order.   */
      while(fgets(sp3_line, 82, ifp[index]) != NULL && sp3_line[0] != '*') {
         if(sp3_line[0] == 'P') {
            sat_cnt++;
            sub_str(sp3_line, s1, 1, 3);
            sv_id = atoi(s1);
            sub_str(sp3_line, s1, 4, 59);
            p_lines[sv_id].prn = sv_id;
            strcpy(p_lines[sv_id].cc5_cc60, s1);
         }
      }
      if(num_prns[index] != sat_cnt) {
         printf("\nFatal Error in Input! Number of satellites read in orbit \n"
                "file %d <%d> does not agree with count from third line of \n"
                "header <%d>. Check last epoch written to output file.\n",
                index + 1, sat_cnt, num_prns[index]);
         exit(1);
      }
   /* Print PRN Position records in ascending PRN order, including filler  */
   /* records where necessary.                                             */
      for(j = 1; j <= MAX_PRN_ID; j++) {
         if(sat_ids[index][j] == j)
            fprintf(ofp, "P%3d%s\n", p_lines[j].prn, p_lines[j].cc5_cc60);
         else if(sat_ids[2][j] == j)
            fprintf(ofp, "P%3d%14.6f%14.6f%14.6f%14.6f\n", j, 0.0, 0.0, 0.0,
                    999999.999999);
      }
   }}
   if(num_epochs[index] != epoch_cnt) {
      printf("\nFatal Error in Input! Number of epochs read in orbit \n"
             "file %d <%d> does not agree with count from first line of \n"
             "header <%d>. Number written to output file WILL NOT AGREE \n"
             "with total count in output file header!\n",
             index + 1, epoch_cnt, num_epochs[index]);
      exit(1);
   }
}
