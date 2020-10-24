void PWClip(Widget w, Position from_x, Position from_y, 
	    Position to_x, Position to_y)
#else
void PWClip(w, from_x, from_y, to_x, to_y)
    Widget w;
    Position from_x, from_y, to_x, to_y;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XRectangle rectangle;
  
    QuerySwap(from_x, to_x);
    QuerySwap(from_y, to_y);
    from_x = max(0, from_x);
    from_y = max(0, from_y);
    to_x = min(PW->pixmap.width - 1, to_x);
    to_y = min(PW->pixmap.height - 1, to_y);

    rectangle.x = InWindowX(PW, from_x);
    rectangle.y = InWindowY(PW, from_y);
    rectangle.width = InWindowX(PW, to_x  + 1) - InWindowX(PW, from_x);
    rectangle.height = InWindowY(PW, to_y + 1) - InWindowY(PW, from_y);
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
