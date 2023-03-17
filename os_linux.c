#include "os_linux.h"


void init_graphics_context(GraphicsContext *ctx) {
    ctx->line_color = 0x00606060;
    ctx->background_color = 0x00505050;
    ctx->STEP = 1000;
    ctx->step_division = 10;
    ctx->x_offset = 0;
    ctx->y_offset = 0;
    ctx->SPEED = 10;

    ctx->mouse_is_pressed = 0;

    ctx->menu_offset = 0;
    ctx->menu_is_active = 0;
    ctx->menu_is_moving = 0;
    ctx->menu_speed = 15;
    ctx->menu_color = 0x00404040;

	int screen_num = DefaultScreen(ctx->dis);
    unsigned int dis_width = DisplayWidth(ctx->dis, screen_num);
    unsigned int dis_height = DisplayHeight(ctx->dis, screen_num);
    ctx->max_canv_size = (dis_width > dis_height) ? dis_width : dis_height;
    ctx->max_canv_size *= 2;

    ctx->width = (dis_width/3);
    ctx->height = (dis_height/3);

    ctx->debug_level = 0;

    pthread_mutex_init(&ctx->mutex, NULL);
}


void init_graphics(GraphicsContext *ctx) {
	unsigned long black, white;

	ctx->dis=XOpenDisplay((char *)0);
   	ctx->screen=DefaultScreen(ctx->dis);
	black=BlackPixel(ctx->dis, ctx->screen),
	white=WhitePixel(ctx->dis, ctx->screen);
   	ctx->win=XCreateSimpleWindow(
   	    ctx->dis,DefaultRootWindow(ctx->dis), 0, 0, 300, 300, 5, black, white
    );
	XSetStandardProperties(
	    ctx->dis, ctx->win, "Howdy", "Hi", None, NULL, 0, NULL
    );
	XSelectInput(
	    ctx->dis, ctx->win,
	    ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask |
	    KeyReleaseMask | PointerMotionMask
    );
    ctx->gc=XCreateGC(ctx->dis, ctx->win, 0, 0);
	XSetBackground(ctx->dis, ctx->gc, white);
	XSetForeground(ctx->dis, ctx->gc, black);
	XClearWindow(ctx->dis, ctx->win);
	XMapRaised(ctx->dis, ctx->win);
};


void close_graphics(GraphicsContext *ctx) {
    pthread_mutex_destroy(&ctx->mutex);
	XFreeGC(ctx->dis, ctx->gc);
	XDestroyWindow(ctx->dis, ctx->win);
	XCloseDisplay(ctx->dis);
	exit(1);
};


void draw_rectangle(
    GraphicsContext *ctx,
    int color,
    int x, int y, int w, int h
) {
    XSetForeground(ctx->dis, ctx->gc, color);
    XFillRectangle(ctx->dis, ctx->win, ctx->gc, x, h, w, h);

    XFlush(ctx->dis);
    XSync(ctx->dis, False);
}
