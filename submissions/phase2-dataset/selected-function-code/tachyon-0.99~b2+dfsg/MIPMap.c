  int mapindex;
  flt mapflt;
  color col, col1, col2;

  if ((u <= 1.0) && (u >= 0.0) && (v <= 1.0) && (v >= 0.0)) {
    if (d > 1.0) 
      d = 1.0;
    else if (d < 0.0)
      d = 0.0;

    mapflt = d * (mip->levels - 0.9999); /* convert range to mapindex        */
    mapindex = (int) mapflt;             /* truncate to nearest integer      */
    mapflt = mapflt - mapindex;          /* fractional part of mip map level */

    /* interpolate between two nearest image maps */
    if (mapindex < (mip->levels - 2)) {
      col1 = ImageMap(mip->images[mapindex    ], u, v);
      col2 = ImageMap(mip->images[mapindex + 1], u, v);
      col.r = col1.r + mapflt*(col2.r - col1.r);
      col.g = col1.g + mapflt*(col2.g - col1.g);
      col.b = col1.b + mapflt*(col2.b - col1.b);
    }
    else {
      /* if mapindex is too high, use the highest,  */
      /* with no MIP-Map interpolation.             */
      col  = ImageMap(mip->images[mip->levels - 1], u, v);
    }
  } 
  else {
    col.r=0.0;
    col.g=0.0;
    col.b=0.0;
  }

  return col;
}
