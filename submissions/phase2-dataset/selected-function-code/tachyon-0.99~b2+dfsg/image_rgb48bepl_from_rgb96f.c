unsigned char * image_rgb48bepl_from_rgb96f(int xres, int yres, float *fimg) { 
  int x, y, R, G, B, sz;
  unsigned char *img = (unsigned char *) malloc(xres * yres * 6);

  sz = xres * yres * 2;
  for (y=0; y<yres; y++) {
    for (x=0; x<xres; x++) {
      int addr = xres * y + x;
      int faddr = addr * 3;
      int iaddr = addr * 2;
      int raddr = iaddr;
      int gaddr = iaddr + sz;
      int baddr = iaddr + (sz * 2);

      R = (int) (fimg[faddr    ] * 65535.0f); /* quantize float to integer */
      G = (int) (fimg[faddr + 1] * 65535.0f); /* quantize float to integer */
      B = (int) (fimg[faddr + 2] * 65535.0f); /* quantize float to integer */

      if (R > 65535) R = 65535;   /* clamp pixel value to range 0-65535    */
      if (R < 0) R = 0;
      img[raddr    ] = (byte) ((R >> 8) & 0xff);
      img[raddr + 1] = (byte) (R & 0xff);

      if (G > 65535) G = 65535;   /* clamp pixel value to range 0-65535    */
      if (G < 0) G = 0;
      img[gaddr    ] = (byte) ((G >> 8) & 0xff);
      img[gaddr + 1] = (byte) (G & 0xff);

      if (B > 65535) B = 65535;   /* clamp pixel value to range 0-65535    */
      if (B < 0) B = 0;
      img[baddr    ] = (byte) ((B >> 8) & 0xff);
      img[baddr + 1] = (byte) (B & 0xff);
    }
  }

  return img;
}
