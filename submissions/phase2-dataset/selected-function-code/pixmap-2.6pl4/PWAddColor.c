void PWAddColor(Widget w, PWColorInfo *color)
#else
void PWAddColor(w, color)
     Widget w;
     PWColorInfo *color;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;

  if (depth <= MAX_DEPTH) {
    /* colorTable is an array, pixel is the entry index */
    /* index in colorTable is 0 for transparent and pixel+1 for others */
    int index = (color->pixel == TRANSPARENT(dpy, screen) ? 0 : color->pixel + 1);

    PW->pixmap.colorTable[index] = color;
  } else {
    /* colorTable is a B-tree, find proper entry recursively */
    *(PW->pixmap.colorTable) = AddColor(*(PW->pixmap.colorTable), color);
  }
}
