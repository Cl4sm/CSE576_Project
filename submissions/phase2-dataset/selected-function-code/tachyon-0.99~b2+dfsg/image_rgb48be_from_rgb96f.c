  int x, y, R, G, B;
  unsigned char *img = (unsigned char *) malloc(xres * yres * 6);

  for (y=0; y<yres; y++) {
    for (x=0; x<xres; x++) {
      int faddr = (xres * y + x) * 3;
      int iaddr = faddr *  2;

      R = (int) (fimg[faddr    ] * 65535.0f); /* quantize float to integer */
      G = (int) (fimg[faddr + 1] * 65535.0f); /* quantize float to integer */
      B = (int) (fimg[faddr + 2] * 65535.0f); /* quantize float to integer */

      if (R > 65535) R = 65535;   /* clamp pixel value to range 0-65535    */
      if (R < 0) R = 0;
      img[iaddr    ] = (byte) ((R >> 8) & 0xff);
      img[iaddr + 1] = (byte) (R & 0xff);

      if (G > 65535) G = 65535;   /* clamp pixel value to range 0-65535    */
      if (G < 0) G = 0;
      img[iaddr + 2] = (byte) ((G >> 8) & 0xff);
      img[iaddr + 3] = (byte) (G & 0xff);

      if (B > 65535) B = 65535;   /* clamp pixel value to range 0-65535    */
      if (B < 0) B = 0;
      img[iaddr + 4] = (byte) ((B >> 8) & 0xff);
      img[iaddr + 5] = (byte) (B & 0xff);
    }
  }

  return img;
}
