#include "global.h"


void init_global_context(GlobalContext *ctx) {
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


void init_x(GlobalContext *ctx) {
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


void draw_canvas(GlobalContext *ctx) {
    int line_style = LineSolid;
    int cap_style = CapButt;
    int join_style = JoinBevel;
    int level = ctx->STEP/100;
    int canv_x_offset;
    int canv_y_offset;

    XSetForeground(ctx->dis, ctx->gc, 0x00555555);
    XSetLineAttributes(ctx->dis, ctx->gc, 1, line_style, cap_style, join_style);
    canv_x_offset = ctx->x_offset % level;
    canv_y_offset = ctx->y_offset % level;
    for (unsigned int i = 0; i<ctx->max_canv_size; i+=level) {
        XDrawLine(
            ctx->dis, ctx->win, ctx->gc,
            0, i+canv_y_offset, ctx->max_canv_size, i+canv_y_offset
        );
        XDrawLine(
            ctx->dis, ctx->win, ctx->gc,
            i+canv_x_offset, 0, i+canv_x_offset, ctx->max_canv_size
        );
    }

    XSetForeground(ctx->dis, ctx->gc, 0x00606060);
    XSetLineAttributes(ctx->dis, ctx->gc, 2, line_style, cap_style, join_style);
    level *= 10;
    canv_x_offset = ctx->x_offset % level;
    canv_y_offset = ctx->y_offset % level;
    for (unsigned int i = 0; i<ctx->max_canv_size; i+=level) {
        XDrawLine(
            ctx->dis, ctx->win, ctx->gc,
            0, i+canv_y_offset, ctx->max_canv_size, i+canv_y_offset
        );
        XDrawLine(
            ctx->dis, ctx->win, ctx->gc,
            i+canv_x_offset, 0, i+canv_x_offset, ctx->max_canv_size
        );
    }

    XSetForeground(ctx->dis, ctx->gc, 0x00656565);
    XSetLineAttributes(ctx->dis, ctx->gc, 5, line_style, cap_style, join_style);
    level *= 10;
    canv_x_offset = ctx->x_offset % level;
    canv_y_offset = ctx->y_offset % level;
    for (unsigned int i = 0; i<ctx->max_canv_size; i+=level) {
        XDrawLine(
            ctx->dis, ctx->win, ctx->gc,
            0, i+canv_y_offset, ctx->max_canv_size, i+canv_y_offset
        );
        XDrawLine(
            ctx->dis, ctx->win, ctx->gc,
            i+canv_x_offset, 0, i+canv_x_offset, ctx->max_canv_size
        );
    }

	XSetForeground(ctx->dis, ctx->gc, 0);
	XSetWindowBackground(ctx->dis, ctx->win, ctx->background_color);

//    draw_examples();
}


void toggle_menu(GlobalContext *ctx) {
    ctx->menu_is_active = ctx->menu_is_active == 0 ? 1 : 0;
    ctx->menu_is_moving = 1;
}

static void draw_string(GlobalContext *ctx, int x, int y, const char *text) {
    XSetForeground(ctx->dis, ctx->gc, 0);
    XDrawString(ctx->dis, ctx->win, ctx->gc, x+2, y+2, text, strlen(text));
    XSetForeground(ctx->dis, ctx->gc, 0x00eeeeee);
    XDrawString(ctx->dis, ctx->win, ctx->gc, x, y, text, strlen(text));
}

static void keep_loop_alive(GlobalContext *ctx) {
    XClientMessageEvent dummyEvent;
    memset(&dummyEvent, 0, sizeof(XClientMessageEvent));
    dummyEvent.type = ClientMessage;
    dummyEvent.window = ctx->win;
    dummyEvent.format = 32;
    XSendEvent(ctx->dis, ctx->win, 0, 0, (XEvent*)&dummyEvent);
    XFlush(ctx->dis);
}

void *draw_menu(void *args) {
    GlobalContext *ctx = (GlobalContext*) args;
    struct timespec ts;
    if (ctx->menu_is_moving > 0) {

        if (ctx->menu_is_active > 0) {
            ctx->menu_offset += ctx->menu_speed;
            if (ctx->menu_offset >= ctx->height) {
                ctx->menu_offset = ctx->height;
                ctx->menu_is_moving = 0;
            }
        } else {
            ctx->menu_offset -= ctx->menu_speed;
            if (ctx->menu_offset <= 0) {
                ctx->menu_offset = 0;
                ctx->menu_is_moving = 0;
            }
        }
        XSetForeground(ctx->dis, ctx->gc, ctx->menu_color);
        XFillRectangle(
            ctx->dis, ctx->win, ctx->gc, 50, 50, ctx->width, ctx->menu_offset
        );

        XFlush(ctx->dis);
        XSync(ctx->dis, False);

        keep_loop_alive(ctx);

        int t_step = 5;
        ts.tv_sec = t_step/1000;
        ts.tv_nsec = t_step * 1000 * 1000;
        nanosleep(&ts, &ts);
    }

    if (ctx->menu_is_active > 0) {
        int h_value = 100;
        int h_step = 30;
        XSetForeground(ctx->dis, ctx->gc, ctx->menu_color);
        XFillRectangle(
            ctx->dis, ctx->win, ctx->gc, 50, 50, ctx->width, ctx->height
        );

        draw_string(ctx, 100, h_value, "Example Item 1");
		h_value += h_step;

        draw_string(ctx, 100, h_value, "Example Item 2");
		h_value += h_step;

        draw_string(ctx, 100, h_value, "Example Item 3");
		h_value += h_step;
        draw_string(ctx, 150, h_value, "Example Item 3.1");
		h_value += h_step;
        draw_string(ctx, 150, h_value, "Example Item 3.2");
		h_value += h_step;
        draw_string(ctx, 150, h_value, "Example Item 3.3");
		h_value += h_step;
    }
}


void close_x(GlobalContext *ctx) {
    pthread_mutex_destroy(&ctx->mutex);
	XFreeGC(ctx->dis, ctx->gc);
	XDestroyWindow(ctx->dis, ctx->win);
	XCloseDisplay(ctx->dis);
	exit(1);
};


void redraw(GlobalContext *ctx) {
	XClearWindow(ctx->dis, ctx->win);
};








