  color col, colL, colU, colll, colul, colLL, colUL;
  flt x, y, z, px, py, pz;
  int ix, iy, iz, nx, ny, nz;
  unsigned char *llptr, *ulptr, *LLptr, *ULptr;
  int addr;

  /*
   *  Perform trilinear interpolation between 8 closest pixels.
   */
  nx = (img->xres > 1) ? 3 : 0;
  x = (img->xres - 1.0) * u;  /* floating point X location */
  ix = (int) x;               /* integer X location        */
  px = x - ix;                /* fractional X location     */

  ny = (img->yres > 1) ? (img->xres * 3) : 0;
  y = (img->yres - 1.0) * v;  /* floating point Y location */
  iy = (int) y;               /* integer Y location        */
  py = y - iy;                /* fractional Y location     */

  nz = (img->zres > 1) ? (img->xres * img->yres * 3) : 0;
  z = (img->zres - 1.0) * w;  /* floating point Z location */
  iz = (int) z;               /* integer Z location        */
  pz = z - iz;                /* fractional Z location     */

  addr = ((img->xres*img->yres * iz) + (img->xres * iy) + ix) * 3; 

  /* pointer to the lower left lower pixel (Y  ) */
  llptr = img->data + addr;

  /* pointer to the lower left upper pixel (Y+1) */
  ulptr = llptr + ny;

  /* pointer to the upper left lower pixel (Z+1) (Y  ) */
  LLptr = llptr + nz;
  /* pointer to the upper left upper pixel (Z+1) (Y+1) */
  ULptr = LLptr + ny;

  /* interpolate between left and right lower pixels */
  colll.r = (flt) ((flt)llptr[0] + px*((flt)llptr[nx  ] - (flt) llptr[0])); 
  colll.g = (flt) ((flt)llptr[1] + px*((flt)llptr[nx+1] - (flt) llptr[1])); 
  colll.b = (flt) ((flt)llptr[2] + px*((flt)llptr[nx+2] - (flt) llptr[2])); 

  /* interpolate between left and right upper pixels */
  colul.r = ((flt)ulptr[0] + px*((flt)ulptr[nx  ] - (flt)ulptr[0])); 
  colul.g = ((flt)ulptr[1] + px*((flt)ulptr[nx+1] - (flt)ulptr[1])); 
  colul.b = ((flt)ulptr[2] + px*((flt)ulptr[nx+2] - (flt)ulptr[2])); 

  /* interpolate between left and right lower pixels */
  colLL.r = (flt) ((flt)LLptr[0] + px*((flt)LLptr[nx  ] - (flt) LLptr[0])); 
  colLL.g = (flt) ((flt)LLptr[1] + px*((flt)LLptr[nx+1] - (flt) LLptr[1])); 
  colLL.b = (flt) ((flt)LLptr[2] + px*((flt)LLptr[nx+2] - (flt) LLptr[2])); 

  /* interpolate between left and right upper pixels */
  colUL.r = ((flt)ULptr[0] + px*((flt)ULptr[nx  ] - (flt)ULptr[0])); 
  colUL.g = ((flt)ULptr[1] + px*((flt)ULptr[nx+1] - (flt)ULptr[1])); 
  colUL.b = ((flt)ULptr[2] + px*((flt)ULptr[nx+2] - (flt)ULptr[2])); 

  /* interpolate between upper and lower interpolated pixels */
  colL.r = (colll.r + py*(colul.r - colll.r));
  colL.g = (colll.g + py*(colul.g - colll.g));
  colL.b = (colll.b + py*(colul.b - colll.b));

  /* interpolate between upper and lower interpolated pixels */
  colU.r = (colLL.r + py*(colUL.r - colLL.r));
  colU.g = (colLL.g + py*(colUL.g - colLL.g));
  colU.b = (colLL.b + py*(colUL.b - colLL.b));

  /* interpolate between upper and lower interpolated pixels */
  col.r = (colL.r + pz*(colU.r - colL.r)) / 255.0;
  col.g = (colL.g + pz*(colU.g - colL.g)) / 255.0;
  col.b = (colL.b + pz*(colU.b - colL.b)) / 255.0;

  return col;
}
