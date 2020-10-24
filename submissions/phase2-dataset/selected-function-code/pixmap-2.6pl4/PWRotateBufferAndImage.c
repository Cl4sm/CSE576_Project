#else
void PWRotateBufferAndImage(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  XPoint tmp_hot;
  XImage *tmp_image, *mask_tmp_image;
  Boolean SetHotSpot = False;

  if (QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y)) {
      /* Rotate Image and marked area */
      Dimension width, height;
      XImage *storage, *mask_storage;

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
      tmp_image = PW->pixmap.image;
      mask_tmp_image = PW->pixmap.mask_image;
      tmp_hot = PW->pixmap.hot;
      PW->pixmap.image = storage;
      PW->pixmap.mask_image = mask_storage;

      /* reset all marked area as there are problems with undo */
      PW->pixmap.mark.from_x = PW->pixmap.mark.to_x = NotSet;
      PW->pixmap.mark.from_y = PW->pixmap.mark.to_y = NotSet;
      PW->pixmap.buffer_mark.from_x = PW->pixmap.buffer_mark.to_x = NotSet;
      PW->pixmap.buffer_mark.from_y = PW->pixmap.buffer_mark.to_y = NotSet;
  } else {
      /* Rotate Image and Cut/Insert buffer */
      SetHotSpot = True;
      tmp_image = PW->pixmap.image;
      mask_tmp_image = PW->pixmap.mask_image;
      tmp_hot = PW->pixmap.hot;
      PW->pixmap.image = PW->pixmap.storage;
      PW->pixmap.mask_image = PW->pixmap.mask_storage;
      PW->pixmap.storage = tmp_image;
      PW->pixmap.mask_storage = mask_tmp_image;
  }

  if ((PW->pixmap.image->width != tmp_image->width) ||
      (PW->pixmap.image->height != tmp_image->height)) {
      PW->pixmap.width = PW->pixmap.image->width;
      PW->pixmap.height = PW->pixmap.image->height;
      InternalResize(PW);
  }
  else PWRedraw(w);

  if (SetHotSpot) /* if storage taken from buffer */
    PWSetHotSpot(w, PW->pixmap.storage_hot.x, PW->pixmap.storage_hot.y);
  else {
      DestroyPixmapImage(&tmp_image);
      DestroyMaskImage(&mask_tmp_image);
  }
  
  PW->pixmap.storage_hot = tmp_hot;
}
