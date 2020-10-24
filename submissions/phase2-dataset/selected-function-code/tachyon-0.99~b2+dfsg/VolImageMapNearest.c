  color col;
  flt x, y, z;
  int ix, iy, iz;
  int addr;

  x = (img->xres - 1.0) * u;  /* floating point X location */
  ix = (int) x;
  y = (img->yres - 1.0) * v;  /* floating point Y location */
  iy = (int) y;
  z = (img->zres - 1.0) * w;  /* floating point Z location */
  iz = (int) z;

  addr = ((iz * img->xres * img->yres) + (iy * img->xres) + ix) * 3; 
  col.r = img->data[addr    ];
  col.g = img->data[addr + 1];
  col.b = img->data[addr + 2];
 
  return col; 
}
