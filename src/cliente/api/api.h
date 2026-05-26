#include "../../../include/cJSON.h"

#include "../structs.h"
#include "../../../modelo/modelo.h"
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


int api_login(shm_privada *shm_p, char * usuario, char * contra, char *msg);

int api_register(shm_privada *shm_p, Register_data data, char *msg);

int api_get_habits(shm_privada *shm_p, Habito * habitos, int *count);