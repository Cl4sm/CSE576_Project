void minmax_rgb96f(int xres, int yres, const float *fimg, 
                   float *min, float *max) {
  int i, sz;
  float minval, maxval;

  minval=maxval=fimg[0];

  sz = xres * yres * 3;
  for (i=0; i<sz; i++) {
    if (fimg[i] > maxval)
      maxval=fimg[i];
    if (fimg[i] < minval)
      minval=fimg[i];
  }

  if (min != NULL)
    *min = minval;

  if (max != NULL)
    *max = maxval;
}
