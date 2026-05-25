#include "../../include/cJSON.h"
#include "bd/db.h"
#include <stdio.h>

int handle_login(cJSON * json_request, cJSON *json_respuesta);
int handle_register(cJSON *json_solicitud, cJSON *json_respuesta);