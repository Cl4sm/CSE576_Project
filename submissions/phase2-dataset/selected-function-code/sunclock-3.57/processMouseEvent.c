processMouseEvent(win, x, y, button, evtype)
Window  win;
int     x, y;
int     button;
int     evtype;
{
static int  x0 = -1, y0 = -1, pressed3 = 0;
static int u, v, w = -1, h = -1;
static Pixmap savepix = 0;
struct Sundata * Context = (struct Sundata *) NULL;

        Context = getContext(win);
        if (!Context) return;
        if (Context->flags.colorlevel==FULLCOLORS) {
           if (!Context->xim) return;
        } else {
           if (!Context->mappix) return;
        }

        if (evtype!=MotionNotify) RaiseAndFocus(win);

        if (evtype == ButtonPress) {
	   if (win == Context->win && !Context->wintype) return;
	}

        if (win == Menu) {
	   processMenuAction(MenuCaller, x, y, button, evtype);
	   return;
        }

        if (win == Filesel) {
           processFileselAction(FileselCaller, x, y, evtype);
           return;
        }

        if (win == Zoom) {
           processZoomAction(ZoomCaller, x, y, button, evtype);
           return;
        }

        if (win == Option) {
           processOptionAction(OptionCaller, x, y, button, evtype);
           return;
        }

        if (win == Urban) {
           processUrbanAction(UrbanCaller, x, y, button, evtype);
           return;
        }

        /* Click on bottom strip of window */
        if (y >= Context->geom.height) {
           if (button==1) {
	      if (evtype==ButtonPress) return;
	      if (do_menu && getState(Menu)==IsViewable)
                 processKey(win, XK_o);
	      else
                 processKey(win, XK_h);
              return;
           }
           if (button==2) {
              processKey(win, XK_l);
              return;
           }
           if (button==3) {
              /* Open new window */
              if (evtype==ButtonPress || !focus_in) return;
	      if (pressed3) {
		 y = Context->geom.height-1;
		 goto rect;
	      }
              processKey(win, XK_w);
              return;
           }
        }

        /* Click on the map with button 2*/
        if (button==2) {
           processKey(win, XK_f);
           return;
        }

   	if (!Context->wintype) pressed3 = 0;

        if (evtype == MotionNotify && pressed3 && !do_zoom &&
            x0!=-1 && y0!= -1) {
	   if (w>0 && h>0) {
	      if (savepix) {
                 XCopyArea(dpy, savepix, Context->win, Context->gdata->wingc,
                        0, 0, w+1, h+1, u, v);
		 XFreePixmap(dpy, savepix);
		 savepix = 0;
	      }
	   }
	   if (x0<x) { u = x0; w = x-x0; } else { u = x; w = x0-x; }
	   if (y0<y) { v = y0; h = y-y0; } else { v = y; h = y0-y; }
	   if (w>0 && h>0)
	   savepix = XCreatePixmap(dpy, Root, w+1, h+1, DefaultDepth(dpy,scr));
	   if (savepix)
              XCopyArea(dpy, Context->win, savepix, Context->gdata->wingc,
                        u, v, w+1, h+1, 0, 0);
           XDrawRectangle(dpy, Context->win, Context->gdata->wingc, 
              u, v, w, h);
           Context->flags.update = 4;
	   return;
        }

        /* Click on the map with button 3*/
        if (button==3) {
	   if (!Context->wintype) {
	      if (evtype == ButtonRelease)
                 processKey(win, XK_z);
	      return;
	   }
           if (do_zoom && win==ZoomCaller->win) {
              Context->newzoom.fdx = ((double)(x+Context->zoom.dx))
                         /((double)Context->zoom.width);
              Context->newzoom.fdy = ((double)(y+Context->zoom.dy))
                         /((double)Context->zoom.height);
              setZoomAspect(Context, 3);
              setZoomDimension(Context);
              zoom_mode = 14;
              zoom_lasthint = ' ';
              activateZoom(Context, zoom_active);
	   } else {
	      if (evtype == ButtonPress) {
	         pressed3 = 1;
	         x0 = x;
	         y0 = y;
		 return;
	      }
	      if (x0<x) { u = x0; w = x-x0; } else { u = x; w = x0-x; }
	      if (y0<y) { v = y0; h = y-y0; } else { v = y; h = y0-y; }
	      if (evtype == ButtonRelease) {
		 double fact;
	         pressed3 = 0;
		 if (savepix) {
		    XFreePixmap(dpy, savepix);
		    savepix = 0;
		 }
	         if (x==x0 && y==y0) {
                    /* Open zoom filesel */
                    processKey(win, XK_z);
		    return;
		 }
	       rect:
                 Context->newzoom.fdx += 
                    (((double)(u+w/2)/(double)Context->geom.width)-0.5)/
                       Context->newzoom.fx;
                 Context->newzoom.fdy += 
                    (((double)(v+h/2)/(double)Context->geom.height)-0.5)/
                       Context->newzoom.fy;
                 fact = sqrt( ((double)Context->geom.width)/((double)w) *
                          ((double)Context->geom.height)/((double)h) );
                 Context->newzoom.fx *= fact;
                 Context->newzoom.fy *= fact;
                 setZoomDimension(Context);
                 zoom_mode |= 14;
                 activateZoom(Context, zoom_active);
	         x0 = -1;
	         y0 = -1;
		 return;
	      }
	   }
           return;
        } 
        
        if (evtype == MotionNotify) return;
        /* Click with button 1 on the map */

        /* It's a clock, just execute predefined command */
        if (!Context->wintype) {
           if (ExternAction) 
              system(ExternAction);
	   else
	   if (!do_menu) {
	      menu_lasthint = '\0';
	      menu_newhint = ' ';
	      PopMenu(Context);
	   }
           return;
        }

        /* Otherwise, user wants to get info on a city or a location */
        Context->flags.update = 1;

        /* Set the timezone, marks, etc, on a button press */
        if (evtype==ButtonPress) return;   
        processPoint(Context, x, y);
}
