#include "form_register.h"




int form_register(Register_data *data){
	FIELD *field[6];
    FORM  *my_form;
    WINDOW *my_form_win;
    int ch, rows, cols;

    

	init_pair(1, COLOR_CYAN, COLOR_BLACK);    // titulo
	init_pair(2, COLOR_BLACK, COLOR_WHITE);   // campo activo
	init_pair(3, COLOR_WHITE, COLOR_BLACK);   // texto
	init_pair(4, COLOR_RED, COLOR_BLACK);     // borde

    // Definición de campos
    field[0] = new_field(1, 20, 2, 14, 0, 0); // Usuario
    field[1] = new_field(1, 20, 5, 14, 0, 0); // Contraseña
    field[2] = new_field(1, 20, 8, 14, 0, 0); // Contraseña
    field[3] = new_field(1, 20, 11, 14, 0, 0);
    field[4] = new_field(1, 20, 14, 14, 0, 0);
    field[5] = NULL;

	for(int i = 0; i<5; i++){
		set_field_back(field[i], COLOR_PAIR(2));
		set_field_fore(field[i], COLOR_PAIR(3));
		field_opts_off(field[i], O_AUTOSKIP);
	}
    
    // Ocultar caracteres en el campo de contraseña
    field_opts_off(field[1], O_PUBLIC); 

    my_form = new_form(field);
    scale_form(my_form, &rows, &cols);

    // Crear ventana centrada
    my_form_win = newwin(20, 50, (LINES-20)/2, (COLS-50)/2);
    keypad(my_form_win, TRUE);

	wattron(my_form_win, COLOR_PAIR(3));
    box(my_form_win, 0, 0);
	wattroff(my_form_win, COLOR_PAIR(3));

    set_form_win(my_form, my_form_win);
    set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

    print_in_middle(my_form_win, 1, 0, 50, "Register", COLOR_PAIR(1));
    
    post_form(my_form);

    
    // Etiquetas de texto
    mvwprintw(my_form_win, 4, 4, "Usuario:");
    mvwprintw(my_form_win, 7, 4, "Contrasea:");
    mvwprintw(my_form_win, 10, 4, "Nombre:");
    mvwprintw(my_form_win, 13, 4, "Apellido:");
    mvwprintw(my_form_win, 16, 4, "Correo:");

	set_current_field(my_form, field[0]);
	pos_form_cursor(my_form);

	mvprintw(LINES-2, 2, "ENTER = Registrar   F1 = Salir");
	refresh();
    
    wrefresh(my_form_win);
    

	int enviar = 0;
    while((ch = wgetch(my_form_win)) != KEY_F(1)) {
        switch(ch) {
			case KEY_BACKSPACE:
            case 127:
            case 8:
                // Le ordena al formulario borrar el carácter a la izquierda del cursor
                form_driver(my_form, REQ_DEL_PREV);
                break;
            case KEY_DOWN:
				form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
				break;
            case KEY_UP:
                form_driver(my_form, REQ_PREV_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
            case 10: // Enter para aceptar
                form_driver(my_form, REQ_VALIDATION);
				if(strcmp(trim(field_buffer(field[0], 0)), "") == 0){
					mvwprintw(my_form_win, 18, 4, "No deje espacios vacios");
					break;
				}
				enviar = 1;[[]]
				goto fin;
                break;
            default:
                form_driver(my_form, ch);
                break;
        }
    }
	fin:
        if(enviar) {
				printf("Bien\n");
				strcpy(data->usuario, trim(field_buffer(field[0], 0)));
				strcpy(data->contra, trim(field_buffer(field[1], 0)));
				strcpy(data->nombre, trim(field_buffer(field[2], 0)));
				strcpy(data->apellido, trim(field_buffer(field[3], 0)));
				strcpy(data->correo, trim(field_buffer(field[4], 0)));
		}
        // Limpieza de ncurses
        unpost_form(my_form);
        free_form(my_form);
        free_field(field[0]);
        free_field(field[1]);
        clear();
        refresh();
        return enviar; // Devolvemos 1 si quiere loguearse, 0 si canceló con F1
}