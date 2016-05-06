#include "Header.h"

char *naming(char dirname[], char name[]){

   char *newname;
   newname = (char *)malloc(255*sizeof(char));
   strcpy(newname, dirname);
   strcat(newname,"/");
   strcat(newname, name);
   return newname;

}

void printing(char name[]){

   int i=0,j=1;
   char pname[255];
   while(name[i] != '/')
     {   i++;
          j++;
     }
   i = j;
   while(name[i] != '\0')
     {   pname[i-j] = name[i];
          i++;
     }
   pname[i-j] = '\0';
   printf("%s\n",pname);

}

mode_t data_info(char data[], int verbose, int statistics[3]){

   struct stat statbuf;
   if (stat(data, &statbuf) < 0)
     {   perror("Failed to get Info\n");
          exit(-1);
     }
   if (verbose)
     statistics[2] = statistics[2] + (int)statbuf.st_size;
   return statbuf.st_mode;

}

int find_type(char dirname[], char name[]){ // 0 directory   1 file   -1 error

   struct stat statbuf;
   char newname[255];
   strcpy(newname, naming(dirname, name));

   if (stat(newname, &statbuf) < 0)
     {   //perror(newname);
          return -1;
     }
   if ((statbuf.st_mode & S_IFMT) == S_IFDIR )
     {   return 0;   // directory encountered
     }
   else
     {   return 1;   // file encountered
     }
}
