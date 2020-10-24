unsigned char * image_crop_rgb24(int xres, int yres, unsigned char *img, 
                                 int szx, int szy, int sx, int sy) {
  unsigned char *cropped;
  int x, y;

  cropped = (unsigned char *) malloc(szx * szy * 3 * sizeof(unsigned char));
  memset(cropped, 0, szx * szy * 3 * sizeof(unsigned char));
  
  for (y=0; y<szy; y++) {
    int oaddr = ((y+sy) * xres + sx) * 3;
    if ((y+sy >= 0) && (y+sy < yres)) {
      for (x=0; x<szx; x++) {
        if ((x+sx >= 0) && (x+sx < xres)) {
          int addr = (szx * y + x) * 3;
          cropped[addr    ] = img[oaddr + (x*3)    ];
          cropped[addr + 1] = img[oaddr + (x*3) + 1];
          cropped[addr + 2] = img[oaddr + (x*3) + 2];
        }
      }
    }
  }

  return cropped;
}
