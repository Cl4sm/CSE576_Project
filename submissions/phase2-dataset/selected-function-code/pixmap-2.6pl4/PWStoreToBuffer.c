#else
void PWStoreToBuffer(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  
  /* first check available space and resize if necessary */
  if ((PW->pixmap.image->width != PW->pixmap.buffer->width) ||
      (PW->pixmap.image->height != PW->pixmap.buffer->height))
    {
      XImage *buffer = CreatePixmapImage(PW, PW->pixmap.image->width, 
					 PW->pixmap.image->height);
      XImage *mask_buffer = CreateMaskImage(PW, PW->pixmap.mask_image->width, 
					    PW->pixmap.mask_image->height);
      
      DestroyPixmapImage(&PW->pixmap.buffer);
      DestroyMaskImage(&PW->pixmap.mask_buffer);
      PW->pixmap.buffer = buffer;
      PW->pixmap.mask_buffer = mask_buffer;
    }
  
  CopyImageData(PW->pixmap.image, PW->pixmap.buffer, 0, 0, 
		PW->pixmap.image->width-1, PW->pixmap.image->height-1, 0, 0);
  CopyImageData(PW->pixmap.mask_image, PW->pixmap.mask_buffer, 0, 0, 
		PW->pixmap.mask_image->width-1,
		PW->pixmap.mask_image->height-1, 0, 0);
  
  PW->pixmap.buffer_hot  = PW->pixmap.hot;
  PW->pixmap.buffer_mark = PW->pixmap.mark;

  /* save extensions to undo buffer */
  XpmFreeExtensions( PW->pixmap.buffer_extensions, 
                     PW->pixmap.buffer_nextensions );
  CopyExtensions( &PW->pixmap.buffer_extensions, 
                  &PW->pixmap.buffer_nextensions,
                   PW->pixmap.extensions,
                   PW->pixmap.nextensions );
}
