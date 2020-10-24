void 
XPutStringImage(Context, x, y, s, l, mode)
Sundata *Context;
int x, y;
char *s;
int l, mode;
{
    XImage *xim;
    XFontStruct *font = NULL;
    Pixel pixel = 0;

    int i, j, w, h, dy;
    char u = 0, test;
    
    if (!s || !strlen(s)) return;

    if (mode <= 1) {
       font = Context->gdata->font[COORDFONT];
       pixel = Context->gdata->pixel[PARALLELCOLOR-mode];
    } else
    if (mode == 2) {
       font = Context->gdata->font[CITYFONT];
       pixel = Context->gdata->pixel[CITYNAMECOLOR];
    } else
    if (mode >= 3) {
       font = Context->gdata->font[LABELFONT];
       if (Context->flags.colorlevel <= MANYCOLORS)
          pixel = Context->gdata->pixel[CITYNAMECOLOR];	   
       else
          pixel = Context->vmfpixels[mode-3];
    }

    if (!font) return;
    h = font->max_bounds.ascent + font->max_bounds.descent;
    dy = font->max_bounds.ascent;
    
    w = XTextWidth(font, s, l);
    if (w>textwidth || h>textheight) {
       textwidth = w;
       textheight = h;
       if (textpix) {
	  XFreePixmap(dpy, textpix);
          textpix = 0;
       }
    }

    if (!textpix) {
       textpix = XCreatePixmap(dpy, Context->win, textwidth, textheight, 1);
    }

    XSetForeground(dpy, Context->gdata->pixgc, black);
    XFillRectangle(dpy, textpix, Context->gdata->pixgc, 0, 0, w, h);
    XSetForeground(dpy, Context->gdata->pixgc, white);
    XSetFont(dpy, Context->gdata->pixgc, font->fid);
    if (Context->flags.colorlevel <= MANYCOLORS) {
       XDrawString(dpy, Context->mappix, Context->gdata->pixgc, 
		   x+1, y-dy, s, l);
       return;
    }
    XDrawString(dpy, textpix, Context->gdata->pixgc, 0, dy, s, l);
    xim = XGetImage(dpy, textpix, 0, 0, w, h, 1, XYPixmap);
    if (!xim) return;
    test = (bigendian)? 128 : 1;
    for (j=0; j<h; ++j) {
       if (y-dy+j >= (int)Context->geom.height) break;
       for (i=0; i<w; ++i) {
	  if ((i&7) == 0) u = xim->data[j*xim->bytes_per_line+i/8];
          if (u&test)
	     SetPixelLight(Context, x+i+1, y-dy+j, pixel);
	  u = (bigendian)? u<<1 : u>>1;
       }
    }
    XDestroyImage(xim);
}
