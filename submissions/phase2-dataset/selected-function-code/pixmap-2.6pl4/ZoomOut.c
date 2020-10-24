    PixmapWidget PW;
{
    CopyImageData(PW->pixmap.image, PW->pixmap.zoom.image, 
		  0, 0, 
		  PW->pixmap.image->width - 1,
		  PW->pixmap.image->height - 1,
		  PW->pixmap.zoom.at_x, PW->pixmap.zoom.at_y);
    
    CopyImageData(PW->pixmap.mask_image, PW->pixmap.zoom.mask_image, 
		  0, 0, 
		  PW->pixmap.mask_image->width - 1,
		  PW->pixmap.mask_image->height - 1,
		  PW->pixmap.zoom.at_x, PW->pixmap.zoom.at_y);

    DestroyPixmapImage(&PW->pixmap.image);
    DestroyPixmapImage(&PW->pixmap.buffer);
    DestroyMaskImage(&PW->pixmap.mask_image);
    DestroyMaskImage(&PW->pixmap.mask_buffer);
    
    PW->pixmap.image = PW->pixmap.zoom.image;
    PW->pixmap.mask_image = PW->pixmap.zoom.mask_image;
    PW->pixmap.buffer = PW->pixmap.zoom.buffer;
    PW->pixmap.mask_buffer = PW->pixmap.zoom.mask_buffer;
    PW->pixmap.width = PW->pixmap.image->width;
    PW->pixmap.height = PW->pixmap.image->height;
    PW->pixmap.fold = PW->pixmap.zoom.fold;
    PW->pixmap.changed |= PW->pixmap.zoom.changed;
    PW->pixmap.grid = PW->pixmap.zoom.grid;

    if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
	PW->pixmap.hot.x += PW->pixmap.zoom.at_x;
	PW->pixmap.hot.y += PW->pixmap.zoom.at_y;
    }
    else
	PW->pixmap.hot = PW->pixmap.zoom.hot;

    PW->pixmap.mark.from_x = NotSet;
    PW->pixmap.mark.from_y = NotSet;
    PW->pixmap.mark.to_x = NotSet;
    PW->pixmap.mark.to_y = NotSet;
    PW->pixmap.zooming = False;
}    
