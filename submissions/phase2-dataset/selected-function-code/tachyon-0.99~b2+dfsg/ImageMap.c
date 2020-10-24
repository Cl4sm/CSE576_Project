  color col, colx, colx2;
  flt x, y, px, py;
  int ix, iy, nx, ny;
  unsigned char * ptr;

  /*
   *  Perform bilinear interpolation between 4 closest pixels.
   */
  nx = (image->xres > 1) ? 3 : 0;
  x = (image->xres - 1.0) * u; /* floating point X location */
  ix = (int) x;                /* integer X location        */
  px = x - ix;                 /* fractional X location     */

  ny = (image->yres > 1) ? image->xres * 3 : 0;
  y = (image->yres - 1.0) * v; /* floating point Y location */
  iy = (int) y;                /* integer Y location        */
  py = y - iy;                 /* fractional Y location     */

  /* pointer to the left lower pixel */
  ptr  = image->data + ((image->xres * iy) + ix) * 3; 

  /* interpolate between left and right lower pixels */
  colx.r = (flt) ((flt)ptr[0] + px*((flt)ptr[nx  ] - (flt) ptr[0])); 
  colx.g = (flt) ((flt)ptr[1] + px*((flt)ptr[nx+1] - (flt) ptr[1])); 
  colx.b = (flt) ((flt)ptr[2] + px*((flt)ptr[nx+2] - (flt) ptr[2])); 

  /* pointer to the left upper pixel */
  ptr  += ny; 

  /* interpolate between left and right upper pixels */
  colx2.r = ((flt)ptr[0] + px*((flt)ptr[nx  ] - (flt)ptr[0])); 
  colx2.g = ((flt)ptr[1] + px*((flt)ptr[nx+1] - (flt)ptr[1])); 
  colx2.b = ((flt)ptr[2] + px*((flt)ptr[nx+2] - (flt)ptr[2])); 

  /* interpolate between upper and lower interpolated pixels */
  col.r = (colx.r + py*(colx2.r - colx.r)) / 255.0;
  col.g = (colx.g + py*(colx2.g - colx.g)) / 255.0;
  col.b = (colx.b + py*(colx2.b - colx.b)) / 255.0;

  return col;
} 
