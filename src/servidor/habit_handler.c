#include "habit_handler.h"


int get_habits(cJSON *json_solicitud, cJSON *json_respuesta){
    Habito habitos[50];
    int count;

    db_habits_get(habitos, &count);
    cJSON_AddNumberToObject(json_respuesta, "estatus", 0);
    cJSON * arreglo = cJSON_AddArrayToObject(json_respuesta, "habitos");

    for (int i = 0; i < count; i++)
    {
        cJSON_AddItemToArray(arreglo, habito_to_json(habitos[i]));
    }

    return 0;
}

int add_user_habit(cJSON * solicitud_json, cJSON * respuesta_json){



}