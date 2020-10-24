                      int x1, int y1, int x2, int y2, unsigned int *rndval) {
  int x, y;

  if (((x2 - x1) < 2) && ((y2 - y1) < 2)) { return; }

  x=(x1 + x2) / 2;
  y=(y1 + y2) / 2;

  adjust(base, xres, yres, wx, wy, x1, y1, x, y1, x2, y1, rndval);
  adjust(base, xres, yres, wx, wy, x2, y1, x2, y, x2, y2, rndval);
  adjust(base, xres, yres, wx, wy, x1, y2, x, y2, x2, y2, rndval);
  adjust(base, xres, yres, wx, wy, x1, y1, x1, y, x1, y2, rndval);

 
  if (base[x + xres*y]==0.0) {
    base[x + (xres * y)]=(base[x1 + xres*y1] + base[x2 + xres*y1] +
                          base[x2 + xres*y2] + base[x1 + xres*y2]   )/4.0;
  }
 
  subdivide(base, xres, yres, wx, wy, x1, y1 ,x ,y, rndval);
  subdivide(base, xres, yres, wx, wy, x, y1, x2, y, rndval);
  subdivide(base, xres, yres, wx, wy, x, y, x2, y2, rndval);
  subdivide(base, xres, yres, wx, wy, x1, y, x, y2, rndval);
}
