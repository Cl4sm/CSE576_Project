drawImageToRootWindow(Context, mode)
Sundata * Context;
int mode;
{
Window Vroot = GetVRoot(dpy);
Window win;
int wr = DisplayWidth(dpy,scr);
int hr = DisplayHeight(dpy,scr);

int a, b, c, i, j, k, l, ww, hw, mapped, update = 1;
int dx[5] = { 0, 0, 0, 1, -1};
int dy[5] = { 0, 1, -1, 0, 0};

     if (mode>0) {
        if (do_root < 1) 
	   do_root = 1;
	else
	   do_root = 2;
     }
     if (mode<0) {
        if (do_root > 0)  
           do_root = 0;
	else
           do_root = -1;
     }

     ww = Context->geom.width;
     hw = Context->geom.height;
     if (do_root == 2) hw += Context->hstrip;

     if (abs(Context->time - Context->roottime) >= root_period)
        Context->roottime = Context->time;
     else
        if (do_root == 2 && mode==0 && rootpix) update = 0;

     if (!rootpix)
        rootpix = XCreatePixmap(dpy, Root, wr, hr, DefaultDepth(dpy, scr));

     if (update) {
        XSetForeground(dpy, Context->gdata->wingc, 
                         Context->gdata->pixel[ROOTCOLOR]);
        XFillRectangle(dpy, rootpix, Context->gdata->wingc, 0, 0, wr, hr);
        srandom(Context->time);
        if (random_rootpos) {
           rootdx = (double)(random() % 10001)/10000.0;
           rootdy = (double)(random() % 10001)/10000.0;
        }
     }

     if (ww>=wr-5) a = 0; else a = (wr-ww-5)*rootdx;
     if (hw>=hr-5) b = 0; else b = (hr-hw-5)*rootdy;

     if (do_root >= 0 && update) {
        XSetForeground(dpy, Context->gdata->wingc,
	            Context->gdata->pixel[STARCOLOR]);
        c = random() % 191;
        for (i=0; i<wr; i++)
        for (j=0; j<hr; j++) {
	  if ((i*i+j)%971 == (j*j*j+i)%593 + c) {
	     if (((9*i+j*j)%7) == 0) l=4; else l=0;
	     for (k=0; k<=l; k++)
                XDrawPoint(dpy, rootpix, 
                     Context->gdata->wingc, i+dx[k], j+dy[k]);
	  }
	}
	if (do_root > 0)
        for (i=0; i<=5; i++)
	   drawDottedRectangle(dpy, rootpix, Context->gdata->wingc, 
                               a-i-1, b-i-1, ww+2*i+1, hw+2*i+1,
			       Context->gdata->pixel[MAPBGCOLOR],
			       Context->gdata->pixel[MAPFGCOLOR]);
     }

     if (do_root>=1) {
	win = Context->win;
        Context->win = XCreatePixmap(dpy, Root, ww, hw, DefaultDepth(dpy,scr));
	mapped = Context->flags.mapped;
	if (update) {
	   Context->flags.mapped = 1;
           Context->flags.update = 2;
           updateImage(Context);
           showMapImage(Context);
	}
	if (do_root == 2) {
	   RootCaller = Context;
	   XSetForeground(dpy, Context->gdata->wingc,
                               Context->gdata->pixel[MAPSTRIPBGCOLOR]);
           XFillRectangle(dpy, Context->win, Context->gdata->wingc, 
              0, Context->geom.height, Context->geom.width, Context->hstrip);
           Context->flags.bottom &= 1;
 	   drawBottomline(Context);
	   if (screen_saver) Context->flags.mapped = 1;
           Context->flags.hours_shown = 0;
           writeStrip(Context);
	}
	if (update)
           XCopyArea(dpy, Context->win, rootpix, Context->gdata->wingc,
                     0, 0, ww, hw, a, b);
	else
           XCopyArea(dpy, Context->win, Root, Context->gdata->wingc,
                     0, Context->geom.height, ww, Context->hstrip, 
                     a, b+Context->geom.height);
        XFlush(dpy);
        XFreePixmap(dpy, Context->win);
        Context->win = win;
        Context->flags.mapped = mapped;
     }

     if (update) {
        XSetWindowBackgroundPixmap(dpy, Vroot, rootpix);
        XClearWindow(dpy, Vroot);
     }
     XFlush(dpy);
     if (mode<0) {
        XFreePixmap(dpy, rootpix);
	rootpix = 0;
     }
}
