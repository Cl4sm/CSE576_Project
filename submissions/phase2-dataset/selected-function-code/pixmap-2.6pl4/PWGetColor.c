#else
PWColorInfo *PWGetColor(w, pixel)
     Widget w;
     Pixel pixel;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;

  if (depth <= MAX_DEPTH) {
    /* colorTable is an array, pixel is the entry index */
    /* index in colorTable is 0 for transparent and pixel+1 for others */
    int index = (pixel == TRANSPARENT(dpy, screen) ? 0 : pixel + 1);

    return (PW->pixmap.colorTable[index]);
  } else {
    /* colorTable is a B-tree, find proper entry recursively */
    return GetColorWithPixel(*(PW->pixmap.colorTable), pixel);
  }
}
