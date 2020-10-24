SCM make_xwindow(display, screen_number, win, pxmp, rootp)
     SCM display;
     int screen_number;
     Drawable win;
     char pxmp, rootp;
{
  SCM z;
  struct xs_Window *xsw;
  DEFER_INTS;
  z = must_malloc_cell((long)sizeof(struct xs_Window),
		       (SCM)(tc16_xwindow | OPN
			     | (pxmp ? PXMP : 0L)
			     | (rootp ? SCROOT : 0L)),
		       s_window);
  xsw = WINDOW(z);
  xsw->display = display;
  xsw->dpy = XDISPLAY(display);
  xsw->screen_number = screen_number;
  if (pxmp) xsw->p.pm = (Pixmap)win;
  else xsw->p.win = (Window)win;
  ALLOW_INTS;
  return z;
}
