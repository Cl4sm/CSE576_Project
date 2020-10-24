     PixmapWidget PW;
     Position x, y;
     Pixel color;
{
  if (color == PW->pixmap.clear_pixel) ClearSquare(PW, x, y);
  else if (color == TRANSPARENT(dpy, screen))
    {
      XFillRectangle(dpy, XtWindow(PW),
		     PW->pixmap.transparent_gc,
		     InWindowX(PW, x), InWindowY(PW, y),
		     PW->pixmap.squareW, PW->pixmap.squareH);
    }
  else
    {
      XSetForeground(dpy, PW->pixmap.drawing_gc, color);
      XFillRectangle(dpy, XtWindow(PW),
		     PW->pixmap.drawing_gc,
		     InWindowX(PW, x), InWindowY(PW, y),
		     PW->pixmap.squareW, PW->pixmap.squareH);
      XSetForeground(dpy,PW->pixmap.drawing_gc,
		     PW->pixmap.foreground_pixel);
    }
}
