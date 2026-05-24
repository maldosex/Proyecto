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
#include "../structs.h"


int api_login(shm_privada *shm_p, char * usuario, char * contra, char *msg);

int api_register(shm_privada *shm_p, Register_data data);