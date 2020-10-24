void
drawTextStrip(Context, s, l)
struct Sundata * Context;
char *s;
int l;
{
      XSetBackground(dpy, Context->gdata->wingc, 
          Context->gdata->pixel[CLOCKSTRIPBGCOLOR+Context->wintype]);
      XSetForeground(dpy, Context->gdata->wingc, 
          Context->gdata->pixel[CLOCKSTRIPFGCOLOR+Context->wintype]);
      XSetFont(dpy, Context->gdata->wingc, 
             (Context->wintype)? 
	       Context->gdata->font[MAPSTRIPFONT]->fid : 
	       Context->gdata->font[CLOCKSTRIPFONT]->fid);
      XDrawImageString(dpy, Context->win, Context->gdata->wingc,
           2+2*Context->wintype, Context->geom.height + ((Context->wintype)?
             Context->gdata->font[MAPSTRIPFONT]->max_bounds.ascent + 4 :
             Context->gdata->font[CLOCKSTRIPFONT]->max_bounds.ascent + 3), 
           s+label_shift, l-label_shift);
}
