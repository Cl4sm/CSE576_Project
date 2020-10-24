    PixmapWidget PW;
    Position     x, y;
    Dimension    width, height;
{
    XRectangle rectangle;

    XDefineCursor(dpy, XtWindow(PW), PW->pixmap.cursor);

    rectangle.x = min(x, InWindowX(PW, InPixmapX(PW, x)));
    rectangle.y = min(y, InWindowY(PW, InPixmapY(PW, y)));
    rectangle.width = max(x + width,
		     InWindowX(PW, InPixmapX(PW, x + width)+1)) - rectangle.x;
    rectangle.height = max(y + height,
		     InWindowY(PW, InPixmapY(PW, y + height)+1)) - rectangle.y;
    
    XClearArea(dpy, XtWindow(PW),
	       rectangle.x, rectangle.y,
	       rectangle.width, rectangle.height,
	       False);

    XSetClipRectangles(dpy,
		       PW->pixmap.framing_gc,
		       0, 0,
		       &rectangle, 1,
		       Unsorted);

    XDrawRectangle(dpy, XtWindow(PW),
		   PW->pixmap.framing_gc,
		   InWindowX(PW, 0) - 1, InWindowY(PW, 0) - 1,
		   InWindowX(PW, PW->pixmap.width) - InWindowX(PW, 0) + 1, 
		   InWindowY(PW, PW->pixmap.height) - InWindowY(PW, 0) + 1);

    PWClip((Widget) PW,
	   InPixmapX(PW, x),InPixmapY(PW, y),
	   InPixmapX(PW, x + width), InPixmapY(PW, y + height));

    PWRedrawSquares((Widget) PW, InPixmapX(PW, x), InPixmapY(PW, y),
		    InPixmapX(PW, x + width), InPixmapY(PW, y + height));
    
    PWRedrawGrid((Widget) PW,
		 InPixmapX(PW, x), InPixmapY(PW, y),
		 InPixmapX(PW, x + width), InPixmapY(PW, y + height));
    PWRedrawMark((Widget) PW);
    PWRedrawHotSpot((Widget) PW);
    if (PW->pixmap.redrawCallback)
        PW->pixmap.redrawCallback( (Widget) PW, Set );	/* redraw extensions */
    PWRedrawAxes((Widget) PW);
    PWUnclip((Widget) PW);
}
