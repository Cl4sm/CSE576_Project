	      Position to_x, Position to_y)
#else
void PWZoomIn(w, from_x, from_y, to_x, to_y)
    Widget w;
    Position from_x, from_y, to_x, to_y;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XImage *image, *buffer, *mask_image, *mask_buffer;    
    Dimension width, height, coreWidth, coreHeight;
    int resized;

    if (PW->pixmap.zooming)
	PWZoomOut(w);
    
    QuerySwap(from_x, to_x);
    QuerySwap(from_y, to_y);
    from_x = max(0, from_x);
    from_y = max(0, from_y);
    to_x = min(PW->pixmap.width - 1, to_x);
    to_y = min(PW->pixmap.height - 1, to_y);
    
    width = to_x - from_x + 1;
    height = to_y - from_y + 1;

    image = CreatePixmapImage(PW, width, height);
    buffer = CreatePixmapImage(PW, width, height);
    mask_image = CreateMaskImage(PW, width, height);
    mask_buffer = CreateMaskImage(PW, width, height);

    CopyImageData(PW->pixmap.image, image, from_x, from_y, to_x, to_y, 0, 0);
    CopyImageData(PW->pixmap.buffer, buffer, from_x, from_y, to_x, to_y, 0, 0);
    CopyImageData(PW->pixmap.mask_image, mask_image,
		  from_x, from_y, to_x, to_y, 0, 0);
    CopyImageData(PW->pixmap.mask_buffer, mask_buffer,
		  from_x, from_y, to_x, to_y, 0, 0);
    
    PW->pixmap.zoom.image = PW->pixmap.image;
    PW->pixmap.zoom.buffer = PW->pixmap.buffer;
    PW->pixmap.zoom.mask_image = PW->pixmap.mask_image;
    PW->pixmap.zoom.mask_buffer = PW->pixmap.mask_buffer;
    PW->pixmap.zoom.at_x = from_x;
    PW->pixmap.zoom.at_y = from_y;
    PW->pixmap.zoom.fold = PW->pixmap.fold;
    PW->pixmap.zoom.changed = PW->pixmap.changed;
    PW->pixmap.zoom.hot = PW->pixmap.hot;
    PW->pixmap.zoom.grid = PW->pixmap.grid;

    PW->pixmap.image = image;
    PW->pixmap.buffer = buffer;
    PW->pixmap.mask_image = mask_image;
    PW->pixmap.mask_buffer = mask_buffer;
    PW->pixmap.width = width;
    PW->pixmap.height = height;
    PW->pixmap.changed = False;
    PW->pixmap.hot.x -= from_x;
    PW->pixmap.hot.y -= from_y;
    PW->pixmap.mark.from_x = NotSet;
    PW->pixmap.mark.from_y = NotSet;
    PW->pixmap.mark.to_x = NotSet;
    PW->pixmap.mark.to_y = NotSet;
    PW->pixmap.zooming = True;
    PW->pixmap.grid = True; /* potencially true, could use a resource here */

    FixHotSpot(PW);

    coreWidth = PW->core.width;
    coreHeight = PW->core.height;
    resized = InternalResize(PW);
    if (PW->core.visible && !resized)
	    Refresh(PW, 0, 0, PW->core.width, PW->core.height);
/*
	XClearArea(dpy, XtWindow(PW),
		   0, 0, 
		   coreWidth, coreHeight,
		   True);
*/
}
