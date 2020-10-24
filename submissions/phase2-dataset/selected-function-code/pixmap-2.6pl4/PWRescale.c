void PWRescale(Widget w, Dimension width, Dimension height)
#else
void PWRescale(w, width, height)
    Widget w;
    Dimension width, height;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XImage *image, *mask_image;
    int resized;

    if (PW->pixmap.zooming)
	ZoomOut(PW);
        
    image = ScalePixmapImage(PW, PW->pixmap.image, 
		       (double) width / (double) PW->pixmap.image->width,
		       (double) height / (double) PW->pixmap.image->height);
    mask_image = ScaleMaskImage(PW, PW->pixmap.mask_image, 
		     (double) width / (double) PW->pixmap.mask_image->width,
		     (double) height / (double) PW->pixmap.mask_image->height);

    DestroyPixmapImage(&PW->pixmap.image);
    DestroyMaskImage(&PW->pixmap.mask_image);
    
    PW->pixmap.image = image;
    PW->pixmap.mask_image = mask_image;
    PW->pixmap.width = image->width;
    PW->pixmap.height = image->height;
    
    FixHotSpot(PW);
    FixMark(PW);

    resized = InternalResize(PW);
    if (PW->core.visible && !resized)
	XClearArea(dpy, XtWindow(PW),
		   0, 0, 
		   PW->core.width, PW->core.height,
		   True);
}
