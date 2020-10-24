#else
void PWResize(w, width, height)
    Widget w;
    Dimension width, height;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XImage *image, *mask_image;
    int resized;

    if (PW->pixmap.zooming)
	ZoomOut(PW);

    image = CreatePixmapImage(PW, width, height);
    mask_image = CreateMaskImage(PW, width, height);

    TransferImageData(PW->pixmap.image, image);
    TransferImageData(PW->pixmap.mask_image, mask_image);
    
    DestroyPixmapImage(&PW->pixmap.image);
    DestroyMaskImage(&PW->pixmap.mask_image);

    PW->pixmap.image = image;
    PW->pixmap.mask_image = mask_image;
    PW->pixmap.width = width;
    PW->pixmap.height = height;

    FixHotSpot(PW);
    FixMark(PW);

    resized = InternalResize(PW);
    if (PW->core.visible && !resized)
	XClearArea(dpy, XtWindow(PW),
		   0, 0, 
		   PW->core.width, PW->core.height,
		   True);
}
