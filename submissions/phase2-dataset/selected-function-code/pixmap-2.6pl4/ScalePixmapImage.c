XImage *ScalePixmapImage(PW, src, scale_x, scale_y)
     PixmapWidget PW;
     XImage *src;
     double scale_x, scale_y;
{
  XImage *dst;
  Table table;    
  Position x, y;
  Dimension width, height, w, h;
  Pixel pixel;
  
  width = max(roundint(scale_x * src->width), 1);
  height = max(roundint(scale_y * src->height), 1);
  
  dst = CreatePixmapImage(PW, width, height);
  
  /*
   * It would be nice to check if width or height < 1.0 and
   * average the skipped pixels. But, it is slow as it is now.
   */
  if (scale_x == 1.0 && scale_y == 1.0)
    CopyImageData(src, dst, 0, 0, width-1 , height-1, 0, 0);
  else {
    table.x = (Position *) XtMalloc(sizeof(Position) * src->width);
    table.y = (Position *) XtMalloc(sizeof(Position) * src->height);
    table.width = (Dimension *) XtMalloc(sizeof(Dimension) * src->width);
    table.height = (Dimension *) XtMalloc(sizeof(Dimension) * src->height);
    
    for (x = 0; x < src->width; x++) {
      table.x[x] = roundint(scale_x * x);
      table.width[x] = roundint(scale_x * (x + 1)) - roundint(scale_x * x);
    }
    for (y = 0; y < src->height; y++) {
      table.y[y] = roundint(scale_y * y);
      table.height[y] = roundint(scale_y * (y + 1)) - roundint(scale_y * y);
    }
    
    for (x = 0; x < src->width; x++)
      for (y = 0; y < src->height; y++) {
	pixel = XGetPixel(src, x, y);
	for (w = 0; w < table.width[x]; w++)
	  for (h = 0; h < table.height[y]; h++)
	    if (pixel != PW->pixmap.clear_pixel) XPutPixel(dst, 
							   table.x[x] + w, 
							   table.y[y] + h,
							   pixel);
      }
    
    XtFree((char *)table.x);
    XtFree((char *)table.y);
    XtFree((char *)table.width);
    XtFree((char *)table.height);
  }
  
  return (dst);
}
