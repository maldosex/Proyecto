#include <string.h>
#include "../../../include/cJSON.h"
#include "file_manager.h"
#include <pthread.h>
#include <stdlib.h>

#ifndef DB_H
#define DB_H





int db_init(char*filename, char * db_name);
int db_register_user(cJSON *user_data);
static int db_user_exist(const char *usuario);

cJSON * db_get_auth_info();

#endif