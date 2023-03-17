#ifndef OS_LINUX_H
#define OS_LINUX_H

#include <pthread.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>



typedef struct GraphicsContext {
    /* here are our X variables */
    Display *dis;
    int screen;
    Window win;
    GC gc;

    unsigned int width;
    unsigned int height;
    unsigned int max_canv_size;

    int debug_level;

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
} GraphicsContext;


void init_graphics(GraphicsContext *ctx);
void init_graphics_context(GraphicsContext *ctx);
void close_graphics(GraphicsContext *ctx);

void draw_rectangle(
    GraphicsContext *ctx,
    int color,
    int x, int y, int w, int h
);

#endif  // OS_LINUX_H
