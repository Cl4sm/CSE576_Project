#else
void PWUnclip(w)
    Widget w;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XRectangle rectangle;
  
    rectangle.x = InWindowX(PW, 0);
    rectangle.y = InWindowY(PW, 0);
    rectangle.width = InWindowX(PW, PW->pixmap.width) - InWindowX(PW, 0);
    rectangle.height = InWindowY(PW, PW->pixmap.height) - InWindowY(PW, 0);
    XSetClipRectangles(dpy,
		       PW->pixmap.highlighting_gc,
		       0, 0,
		       &rectangle, 1,
		       Unsorted);
    XSetClipRectangles(dpy,
		       PW->pixmap.drawing_gc,
		       0, 0,
		       &rectangle, 1,
		       Unsorted);
    XSetClipRectangles(dpy,
		       PW->pixmap.framing_gc,
		       0, 0,
		       &rectangle, 1,
		       Unsorted);
}
