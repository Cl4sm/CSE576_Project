#else
void PWUndo(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  Position x, y;
  int tmp_nextensions;
  XpmExtension *tmp_extensions;
  XPoint tmp_hot;
  XImage *tmp_image, *mask_tmp_image;
  PWArea tmp_mark;
  Pixel color;
  
  tmp_image = PW->pixmap.image;
  mask_tmp_image = PW->pixmap.mask_image;
  PW->pixmap.image = PW->pixmap.buffer;
  PW->pixmap.mask_image = PW->pixmap.mask_buffer;
  PW->pixmap.buffer = tmp_image;
  PW->pixmap.mask_buffer = mask_tmp_image;

  tmp_hot = PW->pixmap.hot;
  tmp_mark = PW->pixmap.mark;
  PW->pixmap.mark = PW->pixmap.buffer_mark;
  PW->pixmap.buffer_mark= tmp_mark;

  /* erase extensions then save */
  /* This part is commented now cause it seems to be useless */
/*  if (PW->pixmap.redrawCallback)
      PW->pixmap.redrawCallback( w, Clear ); */
  tmp_nextensions = PW->pixmap.nextensions;
  tmp_extensions  = PW->pixmap.extensions;
  PW->pixmap.nextensions = PW->pixmap.buffer_nextensions;
  PW->pixmap.extensions  = PW->pixmap.buffer_extensions;
  PW->pixmap.buffer_nextensions = tmp_nextensions;
  PW->pixmap.buffer_extensions  = tmp_extensions;

  if ((PW->pixmap.image->width != PW->pixmap.buffer->width) ||
      (PW->pixmap.image->height != PW->pixmap.buffer->height))
    {
      PW->pixmap.width = PW->pixmap.image->width;
      PW->pixmap.height = PW->pixmap.image->height;
      InternalResize(PW);
    }
  else
    {
      for (x = 0; x < PW->pixmap.image->width; x++)
	for (y = 0; y < PW->pixmap.image->height; y++)
	  if (GetPxlFromImageAndMask(PW->pixmap.buffer, PW->pixmap.mask_buffer,
				     x, y) != (color = GetPxl(PW, x, y)))
	    {
	      DrawPoint(PW, x, y, color, NotLazy);
	    }
  
      /* Now treating Undo copy seperatly :
	 when mark is set and is different from buffer one, redraw them */
      if ((QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y)) &&
	  (QuerySet(PW->pixmap.buffer_mark.from_x, 
		    PW->pixmap.buffer_mark.from_y))
	  && ((PW->pixmap.mark.from_x != PW->pixmap.buffer_mark.from_x) ||
	      (PW->pixmap.mark.from_y != PW->pixmap.buffer_mark.from_y) ||
	      (PW->pixmap.mark.to_x != PW->pixmap.buffer_mark.to_x) ||
	      (PW->pixmap.mark.to_y != PW->pixmap.buffer_mark.to_y)))
	{
	  PWRedrawPoints((Widget) PW, PW->pixmap.mark.from_x, 
			 PW->pixmap.mark.from_y, 
			 PW->pixmap.mark.to_x, 
			 PW->pixmap.mark.to_y);
	  PWRedrawPoints((Widget) PW, PW->pixmap.buffer_mark.from_x, 
			 PW->pixmap.buffer_mark.from_y, 
			 PW->pixmap.buffer_mark.to_x, 
			 PW->pixmap.buffer_mark.to_y);
	}
    }

  PWSetHotSpot(w, PW->pixmap.buffer_hot.x, PW->pixmap.buffer_hot.y);
  PW->pixmap.buffer_hot = tmp_hot;

  /* This part is commented now cause it seems to be useless */
/*  if (PW->pixmap.redrawCallback)
      PW->pixmap.redrawCallback( w, Set );*/
}
