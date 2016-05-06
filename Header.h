#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/times.h>
#include <unistd.h>

char *naming(char dirname[], char name[]);
void printing(char name[]);
mode_t data_info(char data[], int verbose, int statistics[3]);
int find_type(char dirname[], char name[]);
int remove_dir(char *dirname, int removedir);
void reading(char source[], char target[], int verbose, int statistics[3]);
void clearing(char source[], char target[]);

