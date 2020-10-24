void 
drawButton(win, n, clicked)
Window win;
int n;
int clicked;
{
int i, j=0, b, d, x1=0, y1=0, x2=0, y2=0, x, y, w0=0;
struct Sundata * Context=NULL;
char c[2] = { '\0', '\0'};
char *s = c;
char *keys=NULL;
int nkeys=0;
int charspace;

    getWinParams(win, &Context, &keys, &nkeys, &y1, &w0);
    if (!Context) return;
    charspace = Context->gdata->charspace;
    if (win==Filesel) charspace = 2*charspace;

    b = 0;
    for (i=0; i<=n; i++) {
        j = i*charspace + b;
        if (win!=Filesel)
	   b += 5*((i==nkeys-1) || (keys[2*i+1]==';'));
    }
    x1 = j;
    y2 = y1+Context->gdata->menustrip;
    if (n>=0 && n<nkeys) {
        if (win==Filesel)
	   s = FileselBanner[n];
	else {
           c[0] = keys[2*n];
	}
        x2 = j+charspace-1;
    } else {
        if (win==Zoom && n==nkeys+1) {
	   c[0] = (zoom_active)?'+':'-';
           x1 = areah+170, 
           y1 = 21;
           x2 = x1+charspace;
           y2 = y1+18;        
        } else {
           s = Label[L_ESCAPE];
           x2 = w0 - 1;
	}
    }

    if (clicked<=-1) {
       XSetForeground(dpy, Context->gdata->wingc, 
	              Context->gdata->pixel[(clicked==-2)?
                                  BUTTONCOLOR:MENUFGCOLOR]);
       for (y=y1+2; y<=y2-3; y+=y2-y1-5)
           for (x=x1+4; x<=x2-3; x++) 
              if ((x+y)&1) XDrawPoint(dpy, win, 
			              Context->gdata->wingc, x, y);
       for (y=y1+2; y<=y2-3; y++)
           for (x=x1+4; x<=x2-3; x+=x2-x1-7) 
              if ((x+y)&1) XDrawPoint(dpy, win, 
			              Context->gdata->wingc, x, y);
       return;
    }

    drawBox(win, Context->gdata->wingc, 
            Context->gdata->pixel, x1, y1, x2, y2, clicked);

    d = (5*Context->gdata->charspace)/12;
    XSetBackground(dpy, Context->gdata->wingc, 
		   Context->gdata->pixel[BUTTONCOLOR]);

    if (win==Menu && do_dock && (s[0]==c[0]) && index(WeakChars,c[0]))
        XSetForeground(dpy, Context->gdata->wingc, 
                            Context->gdata->pixel[WEAKCOLOR]);
    else
        XSetForeground(dpy, Context->gdata->wingc, 
                            Context->gdata->pixel[MENUFGCOLOR]);

    XDrawImageString(dpy, win, Context->gdata->wingc, 
                     x1+d, 
                     y1+Context->gdata->font[MENUFONT]->max_bounds.ascent + 4, 
                     s, strlen(s));
}
