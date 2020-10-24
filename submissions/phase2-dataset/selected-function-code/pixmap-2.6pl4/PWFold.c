#else
void PWFold(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  Position x, y, new_x, new_y;
  Dimension horiz, vert;
  XImage *storage, *mask_storage;
  Pixel color;
  
  storage = CreatePixmapImage(PW, (Dimension) PW->pixmap.image->width, 
			      (Dimension) PW->pixmap.image->height);
  mask_storage = CreateMaskImage(PW, (Dimension) PW->pixmap.mask_image->width, 
				 (Dimension) PW->pixmap.mask_image->height);
  
  TransferImageData(PW->pixmap.image, storage);
  TransferImageData(PW->pixmap.mask_image, mask_storage);
  
  PW->pixmap.fold ^= True;
  horiz = (PW->pixmap.image->width + PW->pixmap.fold) / 2;
  vert = (PW->pixmap.image->height + PW->pixmap.fold) / 2;
  
  for (x = 0; x < PW->pixmap.image->width; x++)
    for (y = 0; y < PW->pixmap.image->height; y++) {
      new_x = (x + horiz) % PW->pixmap.image->width;
      new_y = (y + vert) % PW->pixmap.image->height;
      if(GetPxl(PW, new_x, new_y) != 
	 (color = GetPxlFromImageAndMask(storage, mask_storage, x, y)))
	{
	  DrawPoint(PW, new_x, new_y, color, Lazy);
	}
    }
  
  DestroyPixmapImage(&storage);
  DestroyMaskImage(&mask_storage);
  
  if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y))
    PWSetHotSpot(w, 
		 (Position) 
		 ((PW->pixmap.hot.x + horiz) % PW->pixmap.image->width),
		 (Position)
		 ((PW->pixmap.hot.y + vert) % PW->pixmap.image->height));
  if (PW->pixmap.translateCallback) 
      PW->pixmap.translateCallback( w, horiz, vert );
}
