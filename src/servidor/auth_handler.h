#include "../../include/cJSON.h"
#include "bd/db.h"
#include <stdio.h>


#include "contexto_cliente.h"



int handle_log(cliente_contexto * cliente_ctx, Usuario_t usuario_login);

int handle_reg(Usuario_t usuario_login, Respuesta_t *respuesta);