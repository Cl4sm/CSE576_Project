Pixmap Bitmap_To_Pixmap(dpy, d, gc, bitmap, width, height)
     Display *dpy;
     Drawable d;
     GC gc;
     Pixmap bitmap;
     int width, height;
{
  Pixmap pix;
  int x;
  unsigned int i, depth;
  Drawable root;

  if (!XGetGeometry(dpy, d, &root, &x, &x, &i, &i, &i, &depth))
    return(0);

  pix = XCreatePixmap(dpy, d, width, height, (int)depth);

  XCopyPlane(dpy, bitmap, pix, gc, 0, 0, width, height, 0, 0, 1);

  return(pix);
}