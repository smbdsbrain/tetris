#include "menu.h"
#include "game.h"
#include <stdlib.h>
#include <windows.h>
#include "graphics.h"

void init();
void load_screen();
void about();
void game_over();

int flag_game = 0;
extern button BB[5];

int main()
{
    int st = 1;
    init();
    load_screen();

    //DELETE THIS.
    //new_game();

    do {
        st = menu(st);



        switch (st) {
        case 1:
            new_game();
            break;
        case 2:
            open_game();
            break;
        case 3:
            save_game();
            break;
        case 4:
            about();
            break;
        case 5:
            game_over();
            break;
        }
    } while (st != 5);
    return 0;
}
//==============
void init()
{
    int i, j, k;
    FILE* f;
    initwindow(1000, 600);
    for (i = 0; i < 5; i++) {
        BB[i].x = 50;
        BB[i].y = 30 + i * 100;
        BB[i].w = 250;
        BB[i].h = 150;
    }
    
    if ((f = fopen("figure.txt", "rt")) == NULL)
    {
        printf("Cannot open input ");
        return;
    }
}
void load_screen()
{
    IMAGE* bitmap = loadBMP("zastavka1.jpg");
    BB[0].bitmap = loadBMP("new_game.jpg");
    BB[1].bitmap = loadBMP("open_game.jpg");
    BB[2].bitmap = loadBMP("save_game.jpg");
    BB[3].bitmap = loadBMP("about.jpg");
    BB[4].bitmap = loadBMP("game_over.jpg");
    putimage(0, 0, bitmap, COPY_PUT);

    //UNCOMMENT THIS!
    //Sleep(1500);
    freeimage(bitmap);
    cleardevice();
}

void about()
{

}