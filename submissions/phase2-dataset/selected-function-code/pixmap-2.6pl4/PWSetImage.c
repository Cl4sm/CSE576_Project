#else
void PWSetImage(w, image, mask_image)
    Widget w;
    XImage *image, *mask_image;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XImage *buffer, *mask_buffer;
    int resized;
    
    buffer = CreatePixmapImage(PW, 
			       (Dimension) image->width, 
			       (Dimension) image->height);
    mask_buffer = CreateMaskImage(PW, 
				  (Dimension) image->width, 
				  (Dimension) image->height);
    
    TransferImageData(PW->pixmap.image, buffer);
    TransferImageData(PW->pixmap.mask_image, mask_buffer);
    
    DestroyPixmapImage(&PW->pixmap.image);
    DestroyPixmapImage(&PW->pixmap.buffer);
    DestroyMaskImage(&PW->pixmap.mask_image);
    DestroyMaskImage(&PW->pixmap.mask_buffer);
    
    PW->pixmap.image = image;
    PW->pixmap.mask_image = mask_image;
    PW->pixmap.buffer = buffer;
    PW->pixmap.mask_buffer = mask_buffer;
    PW->pixmap.width = image->width;
    PW->pixmap.height = image->height;
    
    resized = InternalResize(PW);

    if (PW->core.visible && !resized) {
	XClearArea(dpy, XtWindow(PW),
		   0, 0, 
		   PW->core.width, PW->core.height,
		   True);    
    }
}
