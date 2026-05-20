#include "../../../include/cJSON.h"
#include <stdio.h>
#include "ncurses.h"
#include <semaphore.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>

#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

cJSON * get_login_req(char * usuario, char * contra);