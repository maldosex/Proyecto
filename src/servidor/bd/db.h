#include <string.h>
#include "../../../include/cJSON.h"
#include "file_manager.h"
#include <pthread.h>

#ifndef DB_H
#define DB_H

typedef struct usuario{
    int id;
} user;

#endif


int db_init(char*filename, char * db_name);

cJSON * db_get_auth_info();