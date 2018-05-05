#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "graphics.h"
#include <math.h>
#include <algorithm>
#define SLOWDOWN_SPEED 0.05
#define HOLES_COUNT 6
#define BALL_RADIUS 15
#define BALLS_COUNT 16
#define PI acos(-1)
#define HOLE_RADIUS 20
#define STOP_ENERGY 0.5

bool page = 0;

bool is_game = true;

double cue_angle = 0.0;
double cue_power = 10.0;

int player_scores = 0;
int computer_scores = 0;
int curr_player = 0;

struct dot {
    int x, y;
    dot() { x = 0, y = 0; }
    dot(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
} holes[HOLES_COUNT], cue_dots[4];

dot operator+(dot a, dot b)
{
    return dot(a.x + b.x, a.y + b.y);
}

double reflect(double alpha, double reflect_angle)
{
    return 2 * reflect_angle - alpha;
}

dot normalize(dot a, dot b)
{
    return dot(b.x - a.x, b.y - a.y);
}

dot rotate_dot(dot a, dot b, double angle)
{
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
    return dot((cosTheta * (a.x - b.x) - sinTheta * (a.y - b.y) + b.x),
        (sinTheta * (a.x - b.x) + cosTheta * (a.y - b.y) + b.y));
}

struct ball {
    dot coords;
    double alpha;
    double energy;
    int color;
    bool active;
    ball(){};
    ball(dot _coords, double _alpha, double _energy)
    {
        coords = _coords;
        alpha = _alpha;
        energy = _energy;
        color = COLOR(255, 255, 255);
        active = true;
    }

    ball(dot _coords, double _alpha, double _energy, int _color)
    {
        coords = _coords;
        alpha = _alpha;
        energy = _energy;
        color = _color;
        active = true;
    }

    update()
    {
        if (energy > STOP_ENERGY) {
            energy -= SLOWDOWN_SPEED;
            coords = coords + dot(round(1.5 * sqrt(energy) * cos(alpha)), round(1.5 * sqrt(energy) * sin(alpha)));
        }
    }
} balls[BALLS_COUNT];

void new_game()
{
    char c;
    /*
    if (Flag != NOGAME) {
        
        c = ask_user("");
        if (c != 'Y')
            return;
    }*/
    init_game();
    game();
}

void init_game()
{
    cleardevice();
    is_game = true;
    cue_angle = 0.0;
    cue_power = 10.0;

    player_scores = 0;
    computer_scores = 0;
    curr_player = 0;
    srand(time(NULL));
    holes[0] = dot(105, 105);
    holes[1] = dot(105, 495);
    holes[2] = dot(885, 105);
    holes[3] = dot(885, 495);
    holes[4] = dot(490, 105);
    holes[5] = dot(490, 495);

    balls[0] = ball(dot(200, 297), 0, 0.0);

    balls[1] = ball(dot(600, 297), 0, 0.0, COLOR(51, 51, 255));
    balls[2] = ball(dot(635, 315), 0, 0.0, COLOR(148, 0, 0));
    balls[3] = ball(dot(635, 278), 0, 0.0, COLOR(0, 153, 0));
    balls[4] = ball(dot(670, 333), 0, 0.0, COLOR(204, 204, 0));
    balls[5] = ball(dot(670, 260), 0, 0.0, COLOR(148, 0, 0));
    balls[6] = ball(dot(670, 296), 0, 0.0, COLOR(148, 0, 0));
    balls[7] = ball(dot(705, 315), 0, 0.0, COLOR(0, 153, 0));
    balls[8] = ball(dot(705, 278), 0, 0.0, COLOR(51, 51, 255));
    balls[9] = ball(dot(705, 350), 0, 0.0, COLOR(204, 204, 0));
    balls[10] = ball(dot(705, 240), 0, 0.0, COLOR(148, 0, 0));
    balls[11] = ball(dot(740, 333), 0, 0.0, COLOR(0, 153, 0));
    balls[12] = ball(dot(740, 260), 0, 0.0, COLOR(204, 204, 0));
    balls[13] = ball(dot(740, 296), 0, 0.0, COLOR(51, 51, 255));
    balls[14] = ball(dot(740, 225), 0, 0.0, COLOR(204, 204, 0));
    balls[15] = ball(dot(740, 368), 0, 0.0, COLOR(51, 51, 255));
}

char ask_user(char* s)
{
    return 'Y';
}

void draw_hole(dot u)
{
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));
    setlinestyle(EMPTY_FILL, 0, 0);
    fillellipse(u.x, u.y, HOLE_RADIUS, HOLE_RADIUS);
}

double len(dot a)
{
    return sqrt(a.x * a.x + a.y + a.y);
}

double len(dot a, dot b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void draw_table()
{
    setfillstyle(SOLID_FILL, COLOR(168, 216, 255));
    bar(0, 0, 1000, 600);

    setcolor(COLOR(255, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(19, 136, 8));
    setlinestyle(SOLID_LINE, 0, 40);
    bar(100, 100, 900, 500);
    rectangle(80, 80, 920, 520);

    for (int i = 0; i < HOLES_COUNT; i++) {
        draw_hole(holes[i]);
    }

    char t[10];
    int xt = player_scores;
    int it = 0;
    while (xt) {
        t[it++] = (xt % 10) + '0';
        xt /= 10;
    }
    if (it == 0)
        t[it++] = '0';
    t[it] = 0;

    setbkcolor(COLOR(168, 216, 255));
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 14);
    outtextxy(20, 20, "Player scores: ");
    outtextxy(20 + textwidth("Player scores: "), 20, t);

    xt = computer_scores;
    it = 0;
    while (xt) {
        t[it++] = (xt % 10) + '0';
        xt /= 10;
    }
    if (it == 0)
        t[it++] = '0';
    t[it] = 0;
    outtextxy(500, 20, "Computer scores: ");
    outtextxy(500 + textwidth("Computer scores: "), 20, t);
}

void draw_ball(ball* b)
{
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, b->color);
    setlinestyle(EMPTY_FILL, 0, 0);
    fillellipse(b->coords.x, b->coords.y, BALL_RADIUS, BALL_RADIUS);
}

void update_ball(ball* b)
{
    b->update();
}

void draw_balls()
{
    for (int i = 0; i < BALLS_COUNT; i++) {
        draw_ball(balls + i);
    }
}

void update_balls()
{
    for (int i = 0; i < BALLS_COUNT; i++) {
        update_ball(balls + i);
    }
}

void check_hits_with_borders()
{
    for (int i = 0; i < BALLS_COUNT; i++) {
        if (balls[i].active) {
            if (balls[i].coords.x - BALL_RADIUS <= 100 || balls[i].coords.x + BALL_RADIUS >= 900) {
                balls[i].alpha = PI - balls[i].alpha;
                if (balls[i].coords.x - BALL_RADIUS <= 100)
                    balls[i].coords.x = 101 + BALL_RADIUS;
                else
                    balls[i].coords.x = 899 - BALL_RADIUS;
            }
            else if (balls[i].coords.y - BALL_RADIUS <= 100 || balls[i].coords.y + BALL_RADIUS >= 500) {
                balls[i].alpha = reflect(balls[i].alpha, PI);
                if (balls[i].coords.y - BALL_RADIUS <= 100)
                    balls[i].coords.y = 101 + BALL_RADIUS;
                else
                    balls[i].coords.y = 499 - BALL_RADIUS;
            }

            //¯ \ _ (ツ) _ / ¯
            if (balls[i].alpha <= -PI)
                balls[i].alpha += 2 * PI;
            else if (balls[i].alpha >= PI)
                balls[i].alpha -= 2 * PI;
        }
    }
}

void check_hits_with_other_ball()
{
    for (int i = 0; i < BALLS_COUNT; i++) {
        for (int j = 0; j < BALLS_COUNT; j++) {
            if (i != j)
                if (len(balls[i].coords, balls[j].coords) <= BALL_RADIUS * 2) {
                    ball* a = balls + i, * b = balls + j;
                    if (a->energy <= 0.5) {
                        ball* c = a;
                        a = b;
                        b = c;
                    }

                    printf("0\n");
                    dot v = normalize(a->coords, b->coords);
                    double alph = atan2(v.y, v.x), m = len(v);
                    double ralph = alph + PI / 2.0;

                    a->energy /= 2.0;
                    a->alpha = reflect(a->alpha, ralph);

                    b->alpha = alph;
                    b->energy = a->energy;

                    //just to pretend sticking
                    int id = 0;
                    while (len(balls[i].coords, balls[j].coords) <= BALL_RADIUS * 2) {
                        printf("1\n");
                        id++;
                        b->coords = b->coords + dot((b->energy) * cos(b->alpha), (b->energy) * sin(b->alpha));
                        a->coords = a->coords + dot((a->energy) * cos(a->alpha), (a->energy) * sin(a->alpha));
                        if (b->energy < 2.5)
                            b->energy = 2.5;
                        if (a->energy < 2.5)
                            a->energy = 2.5;
                        if (id > 100) {
                            a->active = false;
                            a->energy = 0;
                            a->coords = dot(-100, -100);
                            if (curr_player)
                                player_scores++;
                            else
                                computer_scores++;
                        }
                    }
                }
        }
    }
}

void close_game()
{
    is_game = false;
}

void game_over()
{
    setactivepage(page = !page);
    close_game();
    setbkcolor(COLOR(168, 255, 255));
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 30);
    outtextxy(300, 300, "YOU LOSE");
    setvisualpage(page);
    delay(3000);
}

void win()
{
    close_game();
    setactivepage(page = !page);
    setbkcolor(COLOR(255, 140, 255));
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 40);
    outtextxy(300, 300, "YOU WIN");
    setvisualpage(page);
    delay(3000);
}

void check_hits_with_holes()
{
    int active_count = 0;
    for (int i = 0; i < HOLES_COUNT; i++)
        for (int j = 0; j < BALLS_COUNT; j++) {
            if (balls[j].active && len(holes[i], balls[j].coords) <= 15) {

                if (j == 0) {
                    if (curr_player)
                        game_over();
                    else
                        win();
                }

                balls[j].active = false;
                balls[j].energy = 0;
                balls[j].coords = dot(-100, -100);
                if (curr_player)
                    player_scores++;
                else
                    computer_scores++;
            }

            if (balls[j].active)
                active_count++;
        }

    if (active_count == 1) {
        if (computer_scores >= player_scores)
            game_over();
        else
            win();
    }
}

void check_hits()
{
    check_hits_with_borders();
    check_hits_with_other_ball();
    check_hits_with_holes();
}

bool check_all_are_stopped()
{
    for (int i = 0; i < BALLS_COUNT; i++)
        if (balls[i].energy >= STOP_ENERGY)
            return false;
    return true;
}

void draw_cue()
{
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(140, 140, 140));
    setlinestyle(SOLID_FILL, 0, 1);
    const int n = 8;

    dot mod = dot(-(cue_power + 230), 0);
    cue_dots[0] = balls[0].coords + mod;
    cue_dots[1] = dot(balls[0].coords.x + 200, balls[0].coords.y) + mod;
    cue_dots[2] = dot(balls[0].coords.x + 200, balls[0].coords.y + 5) + mod;
    cue_dots[3] = dot(balls[0].coords.x, balls[0].coords.y + 5) + mod;

    for (int i = 0; i < 4; i++)
        cue_dots[i] = rotate_dot(cue_dots[i], balls[0].coords, cue_angle);

    int t[n];
    for (int i = 0; i < n; i += 2)
        t[i] = cue_dots[i / 2].x, t[i + 1] = cue_dots[i / 2].y;

    fillpoly(4, t);
}

void game()
{
    draw_table();
    draw_balls();

    do {
        setactivepage(page = !page);

        draw_table();
        draw_balls();
        update_balls();
        check_hits();
        char key;
        if (check_all_are_stopped()) {
            draw_cue();

            if (curr_player) {
                cue_power = rand() % 60 + 10;
                cue_angle = (rand() % (int)(PI * 2 * 100)) / 100.0;
                setactivepage(page = !page);
                draw_cue();
                setvisualpage(page);
                delay(500);
                balls[0].energy = cue_power;
                balls[0].alpha = cue_angle;
                curr_player = (curr_player + 1) % 2;
            }
            else if (kbhit()) {
                key = getch();
                if (key == 0) {
                    key = getch();
                    switch (key) {
                    case 75:
                        cue_angle -= PI / 54.0;
                        break;
                    case 77:
                        cue_angle += PI / 54.0;
                        break;
                    case 72:
                        cue_power = std::max(--cue_power, 10.0);
                        break;
                    case 80:
                        cue_power = std::min(++cue_power, 80.0);
                        break;
                    default:
                        printf("%c\n", key);
                        break;
                    }
                }
                else if (key == 32 || key == 13) {
                    balls[0].energy = cue_power;
                    balls[0].alpha = cue_angle;
                    curr_player = (curr_player + 1) % 2;
                }
            }
        }

        setvisualpage(page);
        delay(10);
    } while (is_game);
}

void open_game() {
    setactivepage(page = !page);
    setbkcolor(COLOR(255, 140, 255));
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 30);
    outtextxy(300, 300, "GAME LOADED FROM FILE");
    setvisualpage(page);
    delay(3000);
  }
void save_game() {
    setactivepage(page = !page);
    setbkcolor(COLOR(255, 140, 255));
    setcolor(COLOR(0, 0, 0));
    setfillstyle(SOLID_FILL, COLOR(0, 0, 0));
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 30);
    outtextxy(300, 300, "GAME SAVED TO FILE");
    setvisualpage(page);
    delay(3000);
}
