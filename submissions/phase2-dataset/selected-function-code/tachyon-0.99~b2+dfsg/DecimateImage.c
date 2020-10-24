rawimage * DecimateImage(const rawimage * image) {
  rawimage * newimage;
  int x, y, addr, addr2;

  x = (int) image->xres >> 1;
  if (x == 0)
    x = 1;

  y = (int) image->yres >> 1;
  if (y == 0)
    y = 1;

  newimage = NewImage(x, y, 1);

  if (image->xres > 1 && image->yres > 1) {
    for (y=0; y<newimage->yres; y++) {
      for (x=0; x<newimage->xres; x++) {
        addr = (newimage->xres*y + x)*3;
        addr2 = (image->xres*y + x)*3*2;
        newimage->data[addr] = (int)
          (image->data[addr2] + 
           image->data[addr2 + 3] +
           image->data[addr2 + image->xres*3] + 
           image->data[addr2 + (image->xres + 1)*3]) >> 2; 
        addr++;
        addr2++;
        newimage->data[addr] = (int)
          (image->data[addr2] + 
           image->data[addr2 + 3] +
           image->data[addr2 + image->xres*3] + 
           image->data[addr2 + (image->xres + 1)*3]) >> 2; 
        addr++;
        addr2++;
        newimage->data[addr] = (int)
          (image->data[addr2] + 
           image->data[addr2 + 3] +
           image->data[addr2 + image->xres*3] + 
           image->data[addr2 + (image->xres + 1)*3]) >> 2; 
      }
    }
  }
  else if (image->xres == 1) {
    for (y=0; y<newimage->yres; y++) {
      addr = y*3;
      addr2 = y*3*2;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
    }
  }
  else if (image->yres == 1) {
    for (x=0; x<newimage->xres; x++) {
      addr = x*3;
      addr2 = x*3*2;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
      addr++;
      addr2++;
      newimage->data[addr] = (int)
        (image->data[addr2] + 
         image->data[addr2 + 3]) >> 1;
    }
  }

  return newimage;
}
