setupZoom(mode)
int mode;
{
    int i, j, k;
    int zoomx, zoomy, zoomw, zoomh;
    char *num[] = { "1", "2", "5", "10", "20", "50", "100"};
    char *synchro = Label[L_SYNCHRO];
    char s[80];

    if (!do_zoom) return;

    BasicSettings(ZoomCaller);
    XSetWindowColormap(dpy, Zoom, ZoomCaller->gdata->cmap);
    XSetWindowBackground(dpy, Zoom, ZoomCaller->gdata->pixel[MENUBGCOLOR]);

    areaw = ZoomGeom.width - 74;
    areah = ZoomGeom.height - 2*ZoomCaller->gdata->menustrip - 65;

    if (mode == -1) {
       XClearArea(dpy, Zoom,  0,0, ZoomGeom.width, ZoomGeom.height, False);
       XDrawImageString(dpy, Zoom, ZoomCaller->gdata->wingc, 
          160-XTextWidth(ZoomCaller->gdata->font[MENUFONT], 
          synchro, strlen(synchro))+areah, 
          24+ ZoomCaller->gdata->font[MENUFONT]->max_bounds.ascent, 
          synchro, strlen(synchro));

       for (i=0; i<=N_ZOOM; i++)
	  drawButton(Zoom, i, 0);

       BasicSettings(ZoomCaller);

       for (i=0; i<=6; i++) {
          j = 63 + (int) ( (areah-6)*log(atof(num[i]))/log(100.0));
          k = j - ZoomCaller->gdata->charspace*strlen(num[i])/10;
          XDrawImageString(dpy, Zoom, ZoomCaller->gdata->wingc, k,
             ZoomCaller->gdata->font[MENUFONT]->max_bounds.ascent + 3, 
             num[i], strlen(num[i]));
          k = j + ZoomCaller->gdata->font[MENUFONT]->max_bounds.ascent/2 - 10;
          XDrawImageString(dpy, Zoom, ZoomCaller->gdata->wingc,
                24-ZoomCaller->gdata->charspace*strlen(num[i])/4, k , 
                num[i], strlen(num[i]));
          XDrawLine(dpy, Zoom, ZoomCaller->gdata->wingc, j, 20, j, 24);
          XDrawLine(dpy, Zoom, ZoomCaller->gdata->wingc, 30, j-10, 34, j-10);
       }

       XDrawLine(dpy, Zoom, ZoomCaller->gdata->wingc, 60, 22, areah+60, 22);
       XDrawLine(dpy, Zoom, ZoomCaller->gdata->wingc, 32, 50, 32, areah+50);
    }

    XSetWindowBackground(dpy, Zoom, ZoomCaller->gdata->pixel[ZOOMBGCOLOR]);
    if ((mode & 1) && !ZoomCaller->newzoom.mode)
       XClearArea(dpy, Zoom,  41, 31, 9, 9, False);
    if (mode & 2)
       XClearArea(dpy, Zoom,  61, 31, areah, 9, False);
    if (mode & 4)
       XClearArea(dpy, Zoom,  41, 51, 9, areah, False);

    if (!zoompix) {
       int oldlevel, oldfill, code, num;

       oldlevel = ZoomCaller->flags.colorlevel;
       oldfill = ZoomCaller->flags.fillmode;
       ZoomCaller->flags.colorlevel = MONOCHROME;
       ZoomCaller->flags.fillmode = 1;
       i = ZoomCaller->geom.width;
       j = ZoomCaller->geom.height;
       ZoomCaller->newzoom = ZoomCaller->zoom;
       ZoomCaller->zoom.width = ZoomCaller->geom.width = areaw;
       ZoomCaller->zoom.height = ZoomCaller->geom.height = areah;
       ZoomCaller->zoom.dx = ZoomCaller->zoom.dy = 0;

       code = readVMF(Zoom_img_file, ZoomCaller);
       if (code) {
	  if (ZoomCaller->bits) free(ZoomCaller->bits);
	  ZoomCaller->bits = NULL;
       }
       if (!ZoomCaller->bits) {
          num = ((ZoomCaller->geom.width+7)/8)*
	                       ZoomCaller->geom.height*sizeof(char);
	  ZoomCaller->bits = (char *) salloc(num);
       }
       if (code && ZoomCaller->bits)
	  memset(ZoomCaller->bits, 0xFF, num);
       if (ZoomCaller->bits) {
          zoompix = XCreatePixmapFromBitmapData(dpy, Root,
                     ZoomCaller->bits, ZoomCaller->geom.width,
                     ZoomCaller->geom.height, 0, 1, 1);
          free(ZoomCaller->bits);
       }
       
       ZoomCaller->zoom = ZoomCaller->newzoom;
       ZoomCaller->geom.width = i;
       ZoomCaller->geom.height = j;
       ZoomCaller->flags.colorlevel = oldlevel;
       ZoomCaller->flags.fillmode = oldfill;
    }

    XSetWindowBackground(dpy, Zoom, ZoomCaller->gdata->pixel[CHOICECOLOR]);
    XSetBackground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[CHOICECOLOR]);
    XSetForeground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[CHANGECOLOR]);

    if (mode & 2) {
       i = (int) ( (double)(areah-6)*log(ZoomCaller->zoom.fx)/log(100.00));
       XClearArea(dpy, Zoom,  61+i, 31, 6, 9, False);
       if (ZoomCaller->newzoom.fx != ZoomCaller->zoom.fx) {
         i = (int) ((double)(areah-6)*log(ZoomCaller->newzoom.fx)/log(100.00));
         XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, 61+i, 31, 5, 8);
       }
    }
    if (mode & 4) {
       j = (int) ( (double)(areah-6)*log(ZoomCaller->zoom.fy)/log(100.0));
       XClearArea(dpy, Zoom,  41, 51+j, 9, 6, False);
       if (ZoomCaller->newzoom.fy != ZoomCaller->zoom.fy) {
         j = (int) ((double)(areah-6)*log(ZoomCaller->newzoom.fy)/log(100.0));
         XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, 41, 51+j, 8, 5);
       }
    }

    if (mode & 8) {
       zoomw = (areaw*ZoomCaller->geom.width)/ZoomCaller->zoom.width;
       zoomh = (areah*ZoomCaller->geom.height)/ZoomCaller->zoom.height;
       zoomx = (areaw*ZoomCaller->zoom.dx)/ZoomCaller->zoom.width;
       zoomy = (areah*ZoomCaller->zoom.dy)/ZoomCaller->zoom.height;
       i = areaw-zoomx-zoomw-1;
       j = areah-zoomy-zoomh-1;

       XSetBackground(dpy, ZoomCaller->gdata->wingc, 
                      ZoomCaller->gdata->pixel[ZOOMBGCOLOR]);
       XSetForeground(dpy, ZoomCaller->gdata->wingc, 
                      ZoomCaller->gdata->pixel[ZOOMFGCOLOR]);
       
       if (zoomy)
          XCopyPlane(dpy, zoompix, Zoom, ZoomCaller->gdata->wingc, 
                0, 0, areaw-1, zoomy, 61, 51, 1);
       if (j>0)
          XCopyPlane(dpy, zoompix, Zoom, ZoomCaller->gdata->wingc, 
                0, zoomy+zoomh+1, areaw, j, 61, 52+zoomy+zoomh, 1);
       if (zoomx)
          XCopyPlane(dpy, zoompix, Zoom, ZoomCaller->gdata->wingc, 
                0, 0, zoomx, areah-1, 61, 51, 1);
       if (i>0)
          XCopyPlane(dpy, zoompix, Zoom, ZoomCaller->gdata->wingc, 
                zoomx+zoomw+1, 0, i, areah, 62+zoomx+zoomw, 51, 1);

       if (ZoomCaller->flags.colorlevel == MONOCHROME) {
          XSetBackground(dpy, ZoomCaller->gdata->wingc, 
                      ZoomCaller->gdata->pixel[ZOOMFGCOLOR]);
          XSetForeground(dpy, ZoomCaller->gdata->wingc, 
                      ZoomCaller->gdata->pixel[ZOOMBGCOLOR]);
       } else
          XSetBackground(dpy, ZoomCaller->gdata->wingc, 
                      ZoomCaller->gdata->pixel[CHOICECOLOR]);

       XCopyPlane(dpy, zoompix, Zoom, ZoomCaller->gdata->wingc, 
                  zoomx, zoomy, zoomw+1, zoomh+1, 61+zoomx, 51+zoomy, 1);

       if (ZoomCaller->newzoom.fx!=ZoomCaller->zoom.fx || 
           ZoomCaller->newzoom.fy!=ZoomCaller->zoom.fy || 
           ZoomCaller->newzoom.fdx!=ZoomCaller->zoom.fdx || 
           ZoomCaller->newzoom.fdy!=ZoomCaller->zoom.fdy) {
          zoomw = (areaw*ZoomCaller->geom.width)/ZoomCaller->newzoom.width;
          zoomh = (areah*ZoomCaller->geom.height)/ZoomCaller->newzoom.height;
          zoomx = (areaw*ZoomCaller->newzoom.dx)/ZoomCaller->newzoom.width;
          zoomy = (areah*ZoomCaller->newzoom.dy)/ZoomCaller->newzoom.height;
	  if (ZoomCaller->flags.colorlevel==MONOCHROME)
	     drawDottedRectangle(dpy, Zoom, ZoomCaller->gdata->wingc,
		61+zoomx, 51+zoomy, zoomw, zoomh,
		ZoomCaller->gdata->pixel[ZOOMFGCOLOR],
                ZoomCaller->gdata->pixel[ZOOMBGCOLOR]);
	  else {
             XSetForeground(dpy, ZoomCaller->gdata->wingc,
		   ZoomCaller->gdata->pixel[CHANGECOLOR]);
             XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc,
                61+zoomx, 51+zoomy, zoomw, zoomh);
	  }
       }

       i = 60 + (int) (areaw * ZoomCaller->newzoom.fdx);
       j = 50 + (int) (areah * ZoomCaller->newzoom.fdy);

       XSetForeground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[CHANGECOLOR]);
       if (i<60+areaw && j<50+areah)
          XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, i, j, 1, 1);

       XSetForeground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[MENUFGCOLOR]);
       XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, 60, 50, areaw+1, areah+1);
    }

    XSetWindowBackground(dpy, Zoom, ZoomCaller->gdata->pixel[MENUBGCOLOR]);
    XSetBackground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[MENUBGCOLOR]);
    XSetForeground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[MENUFGCOLOR]);

    if (mode & 1) {
       XClearArea(dpy, Zoom,  33, 23, 17, 17, False);
       s[0] = '0'+ZoomCaller->newzoom.mode;
       s[1] = '\0';
       XDrawImageString(dpy, Zoom, ZoomCaller->gdata->wingc,
	  39, 25+ZoomCaller->gdata->font[MENUFONT]->max_bounds.ascent, s, 1);
    }

    if (mode & 16) {
       drawButton(Zoom, N_ZOOM+1, 0);
    }

    XSetForeground(dpy, ZoomCaller->gdata->wingc, ZoomCaller->gdata->pixel[MENUFGCOLOR]);

    if (mode == -1) {
       XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, 60, 30, areah+1, 10);
       XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, 40, 50, 10, areah+1);
       XDrawRectangle(dpy, Zoom, ZoomCaller->gdata->wingc, 32, 22, 18, 18);
       drawButton(Zoom, N_ZOOM+1, 0);
    }

    zoom_lasthint = '\0';
    showZoomHint(-1);
}
