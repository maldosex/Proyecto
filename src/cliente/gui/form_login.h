#include <form.h>


char *trim(char *str);

static void print_in_middl(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
int form_login(char * usuario, char * contra) ;