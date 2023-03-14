#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "global.h"


//void draw_examples() {
//    XDrawPoint(dis, win, gc, 5, 5);
//    XDrawPoint(dis, win, gc, 5, height-5);
//    XDrawPoint(dis, win, gc, width-5, 5);
//    XDrawPoint(dis, win, gc, width-5, height-5);
//    /* which intersect at point "50,100".                            */
//    XDrawLine(dis, win, gc, 50, 0, 50, 200);
//    XDrawLine(dis, win, gc, 0, 100, 200, 100);
//    /* now use the XDrawArc() function to draw a circle whose diameter */
//    /* is 30 pixels, and whose center is at location '50,100'.         */
//    XDrawArc(dis, win, gc, 50-(30/2), 100-(30/2), 30, 30, 0, 360*64);
//    {
//        XPoint points[] = {
//            {0, 0},
//            {15, 15},
//            {0, 15},
//            {0, 0}
//        };
//        int npoints = sizeof(points)/sizeof(XPoint);
//        /* draw a small triangle at the top-left corner of the window. */
//        /* the triangle is made of a set of consecutive lines, whose   */
//        /* end-point pixels are specified in the 'points' array.       */
//        XDrawLines(dis, win, gc, points, npoints, CoordModeOrigin);
//    }
//
//    /* draw a rectangle whose top-left corner is at '120,150', its width is */
//    /* 50 pixels, and height is 60 pixels.                                  */
//    XDrawRectangle(dis, win, gc, 120, 150, 50, 60);
//
//    /* draw a filled rectangle of the same size as above, to the left of the */
//    /* previous rectangle.                                                   */
//    XFillRectangle(dis, win, gc, 60, 150, 50, 60);
//}


int main () {

	XEvent event;		/* the XEvent declaration !!! */
	KeySym key;		/* a dealie-bob to handle KeyPress Events */
	char text[255];		/* a char buffer for KeyPress Events */

	GlobalContext ctx;
	init_x(&ctx);
	init_global_context(&ctx);

	int mouse_x;
	int mouse_y;


	while(1) {
	    redraw(&ctx);
	    draw_canvas(&ctx);

//        pthread_create(&ctx.menu_thread, NULL, draw_menu, (void*)&ctx);
//        pthread_join(ctx.menu_thread, NULL);
//        pthread_create(&menu_thread, NULL, myThreadFun, NULL);
        draw_menu( (void*)&ctx );

        if (ctx.mouse_is_pressed) {
			int x = event.xmotion.x;
			int y = event.xmotion.y;

			ctx.x_offset += (x - mouse_x);
			ctx.y_offset += (y - mouse_y);

			mouse_x = x;
			mouse_y = y;

            XDrawArc(
                ctx.dis, ctx.win, ctx.gc, x, y, 30, 30, 0, 360*64
            );

            XFlush(ctx.dis);
            XSync(ctx.dis, False);
        }

		/* get the next event and stuff it into our event variable.
		   Note:  only events we set the mask for are detected!
		*/
//		if (XPending(ctx.dis) > 0)
        XNextEvent(ctx.dis, &event);

		if (event.type==Expose && event.xexpose.count==0) {
		/* the window was exposed redraw it! */
			redraw(&ctx);
		}

        unsigned long color = (unsigned long) (rand()%(1<<31));

		if (
		    event.type==KeyPress &&
		    XLookupString(&event.xkey, text, 255, &key, 0) == 0
        ) {
		    if (key == XK_Up) {
		        ctx.y_offset = (
		            (ctx.y_offset+ctx.SPEED) < (ctx.STEP*ctx.step_division)) ?
		            (ctx.y_offset + ctx.SPEED) : 0;
		    }
		    if (key == XK_Right) {
		        ctx.x_offset = ctx.x_offset > ctx.SPEED ?
		            ctx.x_offset - ctx.SPEED : (ctx.STEP - ctx.SPEED);
		    }
		    if (key == XK_Down) {
		        ctx.y_offset = ctx.y_offset > ctx.SPEED ?
                    ctx.y_offset - ctx.SPEED : (ctx.STEP - ctx.SPEED);
		    }
		    if (key == XK_Left) {
		        ctx.x_offset = (ctx.x_offset+ctx.SPEED) < ctx.STEP ?
		            (ctx.x_offset + ctx.SPEED) : 0;
		    }
        }

		if (event.type==KeyPress&&
		    XLookupString(&event.xkey,text,255,&key,0)==1) {
		/* use the XLookupString routine to convert the invent
		   KeyPress data into regular text.  Weird but necessary...
		*/
			if (text[0]=='q') {
				close_x(&ctx);
			}
			if (text[0]=='e') {
				toggle_menu(&ctx);
			}
			if (text[0]=='u') {
				ctx.debug_level += 1;
				printf("Debug level set to %d\n", ctx.debug_level);
			}
			if (text[0]=='d') {
				ctx.debug_level -= 1;
				printf("Debug level set to %d\n", ctx.debug_level);
			}
			if (text[0]=='c') {
                printf("Color %lu\n", color);
                XClearWindow(ctx.dis, ctx.win);
                XSetWindowBackground(ctx.dis, ctx.win, color);
			}
			if (text[0]=='k') {
		        ctx.y_offset = (ctx.y_offset+ctx.SPEED) < ctx.STEP ?
		            (ctx.y_offset + ctx.SPEED) : 0;
		    }
			if (text[0]=='l') {
		        ctx.x_offset = ctx.x_offset > ctx.SPEED ?
		            ctx.x_offset - ctx.SPEED : (ctx.STEP - ctx.SPEED);
		    }
			if (text[0]=='j') {
		        ctx.y_offset = ctx.y_offset > ctx.SPEED ?
                    ctx.y_offset - ctx.SPEED : (ctx.STEP - ctx.SPEED);
		    }
			if (text[0]=='h') {
		        ctx.x_offset = (ctx.x_offset + ctx.SPEED) < ctx.STEP ?
		            (ctx.x_offset + ctx.SPEED) : 0;
		    }
			if (text[0]=='K') {
                ctx.STEP = (ctx.STEP*(1.1)) < ctx.max_canv_size ?
                    (ctx.STEP*(1.1)+1): 1;
			}
			if (text[0]=='J') {
                ctx.STEP = (ctx.STEP*(0.9)) > 1 ?
                    ctx.STEP*(0.9): ctx.max_canv_size;
			}
			if (text[0]=='p') {
				printf("We toggle menu: %d\n", ctx.menu_is_active);
			}
			redraw(&ctx);
		}
		if (event.type == MotionNotify) {
			int x = event.xmotion.x;
			int y = event.xmotion.y;

            XDrawArc(
                ctx.dis, ctx.win, ctx.gc, x, y, 30, 30, 0, 360*64
            );

            XFlush(ctx.dis);
            XSync(ctx.dis, False);
		}
		if (event.type == ButtonRelease) {
		    ctx.mouse_is_pressed = 0;
		}
		if (event.type == ButtonPress) {
		    ctx.mouse_is_pressed = 1;

			int x = event.xbutton.x;
			int y = event.xbutton.y;

			mouse_x = x;
			mouse_y = y;

            XDrawArc(
                ctx.dis, ctx.win, ctx.gc, x-(30/2), y-(30/2), 30, 30, 0, 360*64
            );
//            XDrawArc(dis, win, gc, x-(30/2), y-(30/2), 30, 30, 0, 360*64);
            XFlush(ctx.dis);
            XSync(ctx.dis, False);

			strcpy(text,"X is FUN!");
			XSetForeground(ctx.dis, ctx.gc, color);
			XDrawString(ctx.dis, ctx.win, ctx.gc, x, y, text, strlen(text));
		}
	}
    return 0;
}



