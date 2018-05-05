#ifndef MENU_H
#define MENU_H
#include <windows.h>
#include "graphics.h"

struct button {
    int x, y, w, h;
    IMAGE* bitmap;
};

void draw_button(button a);
int push_button(button a, int x, int y);
void move_button(button a, int dx, int dy);

void draw_menu(int st);
int menu(int st);
#endif /* MENU_H */
