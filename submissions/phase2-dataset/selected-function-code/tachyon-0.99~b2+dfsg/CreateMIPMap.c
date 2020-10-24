  mipmap * mip;
  int xlevels, ylevels, zlevels, i; 
  char msgtxt[1024];
  
  if (image == NULL) 
    return NULL;

  mip = (mipmap *) malloc(sizeof(mipmap));
  if (mip == NULL)
    return NULL;

  xlevels = 0;  
  i = abs(image->xres);
  while (i) {
    i >>= 1; 
    xlevels++;
  }
  
  ylevels = 0;  
  i = abs(image->yres);
  while (i) {
    i >>= 1; 
    ylevels++;
  }

  zlevels = 0;  
  i = abs(image->zres);
  while (i) {
    i >>= 1; 
    zlevels++;
  }

  mip->levels = (xlevels > ylevels) ? xlevels : ylevels; 
  if (zlevels > mip->levels)
    mip->levels=zlevels;

  /* XXX at present, the decimation routine will not */
  /* handle volumetric images, so if we get one, we  */
  /* have to clamp the maximum MIP levels to 1       */
  if (image->zres > 1) {
    maxlevels = 1;
  }

  if (maxlevels > 0) {
    if (maxlevels < mip->levels)
      mip->levels = maxlevels;
  }

  sprintf(msgtxt, "Creating MIP Map: xlevels: %d  ylevels: %d  zlevels: %d  levels: %d", xlevels, ylevels, zlevels, mip->levels);
  rt_ui_message(MSG_0, msgtxt);

  mip->images = (rawimage **) malloc(mip->levels * sizeof(rawimage *)); 
  if (mip->images == NULL) {
    free(mip);
    return NULL;
  }

  for (i=0; i<mip->levels; i++) {
    mip->images[i] = NULL;
  } 

  mip->images[0] = image;
  for (i=1; i<mip->levels; i++) {
    mip->images[i] = DecimateImage(mip->images[i - 1]);
  }

  return mip;
}
