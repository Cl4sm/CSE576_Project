unsigned char * image_rgb24_from_rgb96f(int xres, int yres, float *fimg) { 
  unsigned char *img;
  int x, y, R, G, B;
  img = (unsigned char *) malloc(xres * yres * 3);

  for (y=0; y<yres; y++) {
    for (x=0; x<xres; x++) {
      int addr = (xres * y + x) * 3;
      R = (int) (fimg[addr    ] * 255.0f); /* quantize float to integer */
      G = (int) (fimg[addr + 1] * 255.0f); /* quantize float to integer */
      B = (int) (fimg[addr + 2] * 255.0f); /* quantize float to integer */

      if (R > 255) R = 255;       /* clamp pixel value to range 0-255      */
      if (R < 0) R = 0;
      img[addr    ] = (byte) R;   /* Store final pixel to the image buffer */

      if (G > 255) G = 255;       /* clamp pixel value to range 0-255      */
      if (G < 0) G = 0;
      img[addr + 1] = (byte) G;   /* Store final pixel to the image buffer */

      if (B > 255) B = 255;       /* clamp pixel value to range 0-255      */
      if (B < 0) B = 0;
      img[addr + 2] = (byte) B;   /* Store final pixel to the image buffer */
    }
  }

  return img;
}
