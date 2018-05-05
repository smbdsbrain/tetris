#ifndef GAME_H
#define GAME_H

#define NOGAME 0
#define GAME 1
#define PAUSE 2

void new_game();
void init_game();
char ask_user(char* s);
void game();
void move_fig(int& y, int& x);
void hide_fig(int y, int x);
int prov_fig(int k, int y, int x, int col);

void open_game();
void save_game();
void draw_pole();
void kv(int i, int j, int color);
void draw_fig(int k, int y, int x, int color);
#endif /* GAME_H */