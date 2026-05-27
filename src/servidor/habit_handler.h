#ifndef HABIT_HANDLER_H
#define HABIT_HANDLER_H
#include "bd/db.h"


#include "contexto_cliente.h"

int handle_get_habits(cliente_contexto * cliente_ctx);
int handle_add_user_habits(cliente_contexto *cliente_ctx,int *ids,int count);
int handle_get_user_habits(cliente_contexto *cliente_ctx);
#endif