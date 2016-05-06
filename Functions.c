#include "Header.h"

int remove_dir(char *dirname, int removedir){

   DIR *dir;
   struct dirent *dirent;
   int type;

   if ( ( dir = opendir( dirname ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",dirname);
         return -1;
     }
   while ( ( dirent=readdir(dir) ) != NULL )
     {   if (dirent->d_ino == 0 ) continue;
          if ((strcmp(dirent->d_name,".") != 0) && (strcmp(dirent->d_name,"..") != 0))
            {   type = find_type(dirname, dirent->d_name); //type = 0  directory encountered  |||  type = 1  file encountered

                 char newname[255];
                 strcpy(newname, naming(dirname, dirent->d_name));

                 if (type == -1)
                   {   continue;
                   }
                 else if (type == 0) // if directory then call remove_dir
                   {   if (remove_dir(newname, 1) != 0)
                          {   perror(newname);
                               continue; 
                          }
                   }
                 else // if file then remove
                   {   if (remove(newname) != 0)
                          {   perror(newname);
                               continue; 
                          }
                   }
            }
     }
   closedir(dir);
   if ((remove(dirname) != 0) && (removedir == 1))
     {   perror(dirname);
          return -1;
     }
   return 0;
}

void reading(char source[], char target[], int verbose, int statistics[3]){

   DIR *dir_source, *dir_target;
   struct dirent *direntsource, *direnttarget;
   int type_s, type_t;

   if ( ( dir_source = opendir( source ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",source);
         exit(-1);
     }
   while ( ( direntsource=readdir(dir_source) ) != NULL )
     {   if (direntsource->d_ino == 0 ) continue;
          if ((strcmp(direntsource->d_name,".") != 0) && (strcmp(direntsource->d_name,"..") != 0))
            {   type_s = find_type(source, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered 
                 if (type_s == -1)
                   {   continue;
                   }
                 type_t = find_type(target, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered 
                 char newname_s[255], newname_t[255];
                 strcpy(newname_s, naming(source, direntsource->d_name));
                 strcpy(newname_t, naming(target, direntsource->d_name));

                 if (verbose)
                   statistics[0] = statistics[0] + 1;
/**************************************************************************************************/
                 if ( type_t == 0 )
                   {   if (type_s == 0)  //Idio onoma kai einai kai ta 2 directories
                          {   reading(newname_s, newname_t, verbose, statistics);
                          }
/**************************************************************************************************/
                        else  //Idio onoma alla sto source einai file kai sto target directory
                          {   if (remove_dir(newname_t, 1) != 0) //call remove_dir to remove the directory
                                 {   perror(newname_t);
                                      continue; 
                                 }

                               if (verbose)
                                 {   statistics[1] = statistics[1] + 1;
                                      printing(newname_s);
                                 }

                               //dhmioyrgia arxeiou
                               int filedes_s, filedes_t;
                               ssize_t nread;
                               char buffer[1];
                               if ((filedes_s = open(newname_s, O_RDONLY)) < 0)
                                 {   perror("Failed to open the file.\n");
                                      continue;
                                 }
                               if ((filedes_t = open(newname_t, O_RDWR | O_CREAT, 0666)) < 0)
                                 {   perror("Failed to open the file.\n");
                                      continue;
                                 }
                               chmod(newname_t, data_info(newname_s, verbose, statistics));
                               while ((nread = read(filedes_s, buffer, 1)) >0)
                                 {  if (write(filedes_t, buffer, 1) == -1)
                                       {   perror("Error Coping File\n");
                                            exit(-1);
                                       }
                                 }
                               close(filedes_s);
                               close(filedes_t);
                          }
                   }
/**************************************************************************************************/
/**************************************************************************************************/
                 else if (type_t == 1)
                   {   if (type_s == 0)  //Idio onoma sto source einai directory kai sto target einai file
                          {   if (remove(newname_t) != 0) //call remove to remove file
                                 {   perror(newname_t);
                                      continue; 
                                 }

                               if (verbose)
                                 {   statistics[1] = statistics[1] + 1;
                                      printing(newname_s);
                                 }

                               if (mkdir(newname_t, 0777) != 0)
                                 {   printf("%s\n",strerror(errno));
                                      continue; 
                                 }
                               chmod(newname_t, data_info(newname_s, verbose, statistics));
                               reading(newname_s, newname_t, verbose, statistics);
                          }
/**************************************************************************************************/
                        else  //Idio onoma kai einai kai ta 2 files
                          {   struct stat statbuf;
                               int size_s, size_target, mtime_s, mtime_t, different = 0;
                               if (stat(newname_s, &statbuf) < 0)
                                 {   perror(newname_s);
                                      continue;
                                 }
                               size_s = (int)statbuf.st_size;
                               mtime_s= (int)statbuf.st_mtime;
                               if (stat(newname_t, &statbuf) < 0)
                                 {   perror(newname_t);
                                      continue;
                                 }
                               size_target = (int)statbuf.st_size;
                               mtime_t= (int)statbuf.st_mtime;
                               // chechk if size and modification time are different
                               if (size_s != size_target)
                                 {   different++;
                                 }
                               if (mtime_s > mtime_t)
                                 {   different++;
                                 }
                               if (different > 0)
                                 {   // remove file
                                     if (remove(newname_t) != 0)
                                       {   perror(newname_t);
                                           continue; 
                                       }

                                     if (verbose)
                                       {   statistics[1] = statistics[1] + 1;
                                            statistics[2] = statistics[2] + size_target;
                                            printing(newname_s);
                                       }

                                     //dhmioyrgia arxeiou
                                     int filedes_s, filedes_t;
                                     ssize_t nread;
                                     char buffer[1];
                                     if ((filedes_s = open(newname_s, O_RDONLY)) < 0)
                                       {   perror("Failed to open the file.\n");
                                            continue;
                                       }
                                     if ((filedes_t = open(newname_t, O_RDWR | O_CREAT, 0666)) < 0)
                                       {   perror("Failed to open the file.\n");
                                            continue;
                                       }
                                     chmod(newname_t, data_info(newname_s, verbose, statistics));
                                     while ((nread = read(filedes_s, buffer, 1)) >0)
                                       {   if (write(filedes_t, buffer, 1) == -1)
                                              {   perror("Error Coping File\n");
                                                   exit(-1);
                                              }
                                       }
                                     close(filedes_s);
                                     close(filedes_t);
                                 }
                          }
                   }
/**************************************************************************************************/
/**************************************************************************************************/
                 else if (type_t == -1)
                   {   if (type_s == 0)  //Den yparxei o fakelos sto target
                          {   if (mkdir(newname_t, 0777) != 0)
                                 {   printf("%s\n",strerror(errno));
                                      continue; 
                                 }
                               chmod(newname_t, data_info(newname_s, verbose, statistics));

                               if (verbose)
                                 {   statistics[1] = statistics[1] + 1;
                                      printing(newname_s);
                                 }

                               reading(newname_s, newname_t, verbose, statistics);
                          }
/**************************************************************************************************/
                        if (type_s == 1)  //Den yparxei to arxeio sto target
                          {   //dhmioyrgia arxeiou
                               int filedes_s, filedes_t;
                               ssize_t nread;
                               char buffer[1];

                               if (verbose)
                                 {   statistics[1] = statistics[1] + 1;
                                      printing(newname_s);
                                 }

                               if ((filedes_s = open(newname_s, O_RDONLY)) < 0)
                                 {   perror("Failed to open the file.\n");
                                      continue;
                                 }
                               if ((filedes_t = open(newname_t, O_RDWR | O_CREAT, 0666)) < 0)
                                 {   perror("Failed to open the file.\n");
                                      continue;
                                 }
                               chmod(newname_t, data_info(newname_s, verbose, statistics));
                               while ((nread = read(filedes_s, buffer, 1)) >0)
                                 {   if (write(filedes_t, buffer, 1) == -1)
                                        {   perror("Error Coping File\n");
                                             exit(-1);
                                        }
                                 }
                               close(filedes_s);
                               close(filedes_t);
                          }
                   }
/**************************************************************************************************/
            }
     }
   closedir(dir_source);

}

void clearing(char source[], char target[]){

   DIR *dir_source, *dir_target;
   struct dirent *direntsource, *direnttarget;
   int type_s, type_t;

   if ( ( dir_source = opendir( source ) ) == NULL )
     {  fprintf(stderr, "cannot open %s \n",source);
         exit(-1);
     }
   while ( ( direntsource=readdir(dir_source) ) != NULL )
     {   if (direntsource->d_ino == 0 ) continue;
          if ((strcmp(direntsource->d_name,".") != 0) && (strcmp(direntsource->d_name,"..") != 0))
            {   type_s = find_type(source, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered 
                 if (type_s == -1)
                   {   continue;
                   }
                 type_t = find_type(target, direntsource->d_name); //type = 0  directory encountered  |||  type = 1  file encountered
                 if (type_t == -1)
                   {   char newname[255];
                        strcpy(newname, naming(source, direntsource->d_name));
                        if (type_s == 0)
                          {   if (remove_dir(newname, 1) != 0) //remove the directory if not exists in source
                                 {   perror(newname);
                                      continue; 
                                 }
                          }
                        if (type_s == 1) //remove the file if not exists in source
                          {   if (remove(newname) != 0)
                                 {   perror(newname);
                                      continue; 
                                 }
                          }
                   }
                 else
                   {   if (type_s == 0) //search subdirectories if not exist in source
                          {   char newname_s[255], newname_t[255];
                               strcpy(newname_s, naming(source, direntsource->d_name));
                               strcpy(newname_t, naming(target, direntsource->d_name));
                               clearing(newname_s, newname_t);
                          }
                   }
            }
     }
   closedir(dir_source);

}

