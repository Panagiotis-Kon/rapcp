#include "Header.h"

int main(int argc, char **argv) {

   struct stat statbuf;
   int verbose, statistics[3], i, position = 1;
/* statistics[0] = files/directories in the hierarchy
   statistics[1] = number of entities copied
   statistics[2] = copied bytes */
   double time_sec;
   verbose = 0;
   for(i=0;i<3;i++)
     statistics[i] = 0;
   for (i=0;i<argc;i++)
     if (strcmp(argv[i], "-v") == 0)
       position = i;
   if (position != 1)
     {   printf("Wrong order of arguments :\n\n        The correct order is -v (Source directory) (Target directory)\n\n");
          return -1;
     }
   if (argc < 3)
     {   printf("More arguments are needed!\n");
          return 1;
     }
   if (!strcmp(argv[1], "-v"))
     {   verbose = 1;
          printf("   Information Enabled : %s \n", argv[1]);
     }
   if (verbose)
     {   printf("   Source directory : %s \n", argv[verbose+1]);
          printf("   Target directory : %s \n", argv[verbose+2]);
          printf("---------------------------------------------\n");
     }
   if (stat( argv[verbose+1], &statbuf) == -1)
     {   fprintf(stderr, "cannot open %s \n",argv[verbose+1]);
          return -1;
     }
   if (stat( argv[verbose+2], &statbuf) == -1)
     {  if (mkdir(argv[verbose+2], 0777) != 0)
           {   printf("%s\n",strerror(errno));
                return -1;
           }
         printf("created directory %s \n", argv[verbose+2]);
         if (verbose)
           {   printf("./\n");
                statistics[1] = statistics[1] + 1;
           }
     }
   statistics[0] = statistics[0] + 1;
   chmod(argv[verbose+2], data_info(argv[verbose+1], 0, statistics));

   double t1, t2;
   struct tms tb1, tb2;
   double ticspersec;
   ticspersec = (double) sysconf(_SC_CLK_TCK);
   t1 = (double) times(&tb1);
   reading(argv[verbose+1], argv[verbose+2], verbose, statistics);
   t2 = (double) times(&tb2);
   time_sec = (t2 - t1) / ticspersec;

   clearing(argv[verbose+2], argv[verbose+1]);

   if (statistics[2] == 0)
     {   time_sec = 0;   }
   if (verbose)
     {   printf("\nthere are %d files/directories in the hierarchy\n",statistics[0]);
          printf("number of entities copied is %d\n",statistics[1]);
          printf("copied %d bytes in %lf sec at ", statistics[2], time_sec);
          if ((statistics[2] != 0) && (time_sec != 0))
            printf("%lf bytes/sec\n", statistics[2]/time_sec);
          else
            printf("0.00 bytes/sec\n");
     }
   return 0;
}
