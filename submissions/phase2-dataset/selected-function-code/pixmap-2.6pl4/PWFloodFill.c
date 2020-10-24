#else
void PWFloodFill(w, x, y, value)
     Widget w;
     Position x, y;
     int value;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  Pixel pixel, foreground = PW->pixmap.foreground_pixel;
  Pixel clear = PW->pixmap.clear_pixel;
  
  pixel = GetPxl(PW, x, y);
  
  if (value == Invert)
    FloodLoop(PW, x, y, ((pixel != clear) ? clear : foreground), pixel);
  else if (value == Clear)
    FloodLoop(PW, x, y, clear, pixel);
  else
    FloodLoop(PW, x, y, foreground, pixel); 
}
