#ifndef GLOBAL_H
#define GLOBAL_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "os_linux.h"


typedef struct GlobalContext {
    /* here are our X variables */
    Display *dis;
    int screen;
    Window win;
    GC gc;

    unsigned int width;
    unsigned int height;

    int debug_level;
    unsigned int max_canv_size;

    /* custom variables */
    unsigned int line_color;
    unsigned int background_color;
    unsigned int STEP;
    unsigned int step_division;
    unsigned int x_offset;
    unsigned int y_offset;
    unsigned int SPEED;

    int menu_offset;
    unsigned int menu_color;
    int menu_is_active;
    int menu_is_moving;
    int menu_speed;
    pthread_t menu_thread;
    pthread_mutex_t mutex;

    int mouse_is_pressed;
} GlobalContext;


void init_x(GlobalContext *ctx);
void close_x(GlobalContext *ctx);
void redraw(GlobalContext *ctx);


void init_global_context(GlobalContext *ctx);

void draw_canvas(GlobalContext *ctx);

void toggle_menu(GlobalContext *ctx);
void *draw_menu(void *args);


#endif  // GLOBAL_H
