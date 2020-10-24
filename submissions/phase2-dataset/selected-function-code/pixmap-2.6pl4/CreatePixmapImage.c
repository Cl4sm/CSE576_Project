    PixmapWidget PW;
    Dimension width, height;
{
  int bitmap_pad;
  XImage *image;
  Pixel wp;
 
  if (depth <= 8) bitmap_pad = 8;
  else if (depth <= 16) bitmap_pad = 16;
  else bitmap_pad = 32;
  
  image = XCreateImage(dpy,
		       DefaultVisual(dpy, screen),
		       depth,
		       ZPixmap, 0,
		       NULL, width, height,
		       bitmap_pad, 0);

  if (!image) 
    {
      XtAppWarning(XtWidgetToApplicationContext((Widget) PW),
		   "Pixmap error in creating XImage\n");
      exit(1);
    }
  else image->data = XtCalloc(1, image->bytes_per_line * height);
  wp = WhitePixelOfColormap(dpy, PW->core.colormap);
  if (wp != 0)			/* to clear the image, hope white is or 0 
				       or greater then black */
    XAddPixel(image, wp);	/* because image pixels are initialized to 0 */
  
  if (_PWDEBUG) {
    printf("bytes_per_line %d\n", image->bytes_per_line);
    printf("size data %d\n", image->bytes_per_line * height);
    printf("bitmap pad %d\n", image->bitmap_pad);
  }
     
  return image;
}
