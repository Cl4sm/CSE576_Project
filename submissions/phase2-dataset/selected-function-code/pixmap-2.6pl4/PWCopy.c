#else
void PWCopy(w, at_x, at_y, value)
     Widget w;
     Position     at_x, at_y;
     int          value;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  XImage *storage, *mask_storage;
  Dimension width, height;
  
  if (QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y)) {
    
    width = PW->pixmap.mark.to_x - PW->pixmap.mark.from_x + 1;
    height = PW->pixmap.mark.to_y - PW->pixmap.mark.from_y + 1;
    
    storage = CreatePixmapImage(PW, width, height);
    mask_storage = CreateMaskImage(PW, width, height);
    
    CopyImageData(PW->pixmap.image, storage,
		  PW->pixmap.mark.from_x,  PW->pixmap.mark.from_y,
		  PW->pixmap.mark.to_x,  PW->pixmap.mark.to_y,
		  0, 0);
    CopyImageData(PW->pixmap.mask_image, mask_storage,
		  PW->pixmap.mark.from_x,  PW->pixmap.mark.from_y,
		  PW->pixmap.mark.to_x,  PW->pixmap.mark.to_y,
		  0, 0);
    
    DrawImageData(PW, storage, mask_storage, at_x, at_y, value, NotLazy);
    
    PWMark(w, at_x, at_y,
	   at_x + PW->pixmap.mark.to_x - PW->pixmap.mark.from_x,
	   at_y + PW->pixmap.mark.to_y - PW->pixmap.mark.from_y); 

    DestroyPixmapImage(&storage);
    DestroyMaskImage(&mask_storage);
  }
}
