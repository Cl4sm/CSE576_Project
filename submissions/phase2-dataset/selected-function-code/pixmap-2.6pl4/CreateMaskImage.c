XImage *CreateMaskImage(PW, width, height)
    PixmapWidget PW;
    Dimension width, height;
{
  XImage *mask_image;

  mask_image = XCreateImage(dpy, DefaultVisual(dpy, screen), 1, ZPixmap, 0, 
			    NULL, width, height, 8, 0);
  if (!mask_image) 
  {
      XtAppWarning(XtWidgetToApplicationContext((Widget) PW),
		   "Pixmap error in creating XImage\n");
      exit(1);
  }
  else mask_image->data = XtCalloc(1,
				   mask_image->bytes_per_line *
				     mask_image->height);

  /* Initialize all pixels to 1: default mask is rectangular */ 
  XAddPixel(mask_image, 1);
  
  return mask_image;
}
