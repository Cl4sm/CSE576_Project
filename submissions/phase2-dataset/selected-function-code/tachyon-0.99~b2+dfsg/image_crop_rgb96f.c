float * image_crop_rgb96f(int xres, int yres, float *fimg, 
                          int szx, int szy, int sx, int sy) {
  float *cropped;
  int x, y;

  cropped = (float *) malloc(szx * szy * 3 * sizeof(float));
  memset(cropped, 0, szx * szy * 3 * sizeof(float));
  
  for (y=0; y<szy; y++) {
    int oaddr = ((y+sy) * xres + sx) * 3;
    if ((y+sy >= 0) && (y+sy < yres)) {
      for (x=0; x<szx; x++) {
        if ((x+sx >= 0) && (x+sx < xres)) {
          int addr = (szx * y + x) * 3;
          cropped[addr    ] = fimg[oaddr + (x*3)    ];
          cropped[addr + 1] = fimg[oaddr + (x*3) + 1];
          cropped[addr + 2] = fimg[oaddr + (x*3) + 2];
        }
      }
    }
  }

  return cropped;
}
