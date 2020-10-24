#else
void PWStore(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  Dimension width, height;
  
  if (QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y)) {
    
    DestroyPixmapImage(&PW->pixmap.storage);
    DestroyMaskImage(&PW->pixmap.mask_storage);
    
    width = PW->pixmap.mark.to_x - PW->pixmap.mark.from_x + 1;
    height = PW->pixmap.mark.to_y - PW->pixmap.mark.from_y + 1;
    
    PW->pixmap.storage = CreatePixmapImage(PW, width, height);
    PW->pixmap.mask_storage = CreateMaskImage(PW, width, height);
    
    CopyImageData(PW->pixmap.image, PW->pixmap.storage,
		  PW->pixmap.mark.from_x,  PW->pixmap.mark.from_y,
		  PW->pixmap.mark.to_x,  PW->pixmap.mark.to_y,
		  0, 0);
    
    CopyImageData(PW->pixmap.mask_image, PW->pixmap.mask_storage,
		  PW->pixmap.mark.from_x,  PW->pixmap.mark.from_y,
		  PW->pixmap.mark.to_x,  PW->pixmap.mark.to_y,
		  0, 0);
  }
}
