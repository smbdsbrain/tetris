#include "menu.h"
#include <windows.h>
#include "graphics.h"

button BB[3];

void draw_menu(int st)
{
    int i;
    for (i = 0; i < 5; i++)
        draw_button(BB[i]);
}

int menu(int st)
{
    int x, y, i;
    do {
        draw_menu(st);
        while (mousebuttons() != 1) {
            x = mousex();
            y = mousey();
        }
        for (i = 0; i < 5; i++)
            if (push_button(BB[i], x, y))
                return i + 1;
    } while (1);
    return st;
}
void draw_button(button a)
{
    putimage(a.x, a.y, a.bitmap, COPY_PUT);
}
int push_button(button a, int x, int y)
{
    if (x >= a.x && x <= a.x + a.w
        && y >= a.y && y <= a.y + a.h)
        return 1;
    else
        return 0;
}
void move_button(button a, int dx, int dy)
{
}