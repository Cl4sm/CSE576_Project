int writeppm48(const char *name, int xres, int yres, unsigned char *imgdata) {
  FILE * ofp;
  int y, xbytes;

  xbytes = 6*xres;

  ofp=fopen(name, "wb");
  if (ofp==NULL)
    return IMAGEBADFILE;

  fprintf(ofp, "P6\n");
  fprintf(ofp, "%d %d\n", xres, yres);
  fprintf(ofp, "65535\n"); /* maxval */

  for (y=0; y<yres; y++) {
    if (fwrite(&imgdata[(yres - y - 1)*xbytes], 1, xbytes, ofp) != xbytes) {
      fclose(ofp);
      return IMAGEWRITEERR;
    } 
  }

  fclose(ofp);
  return IMAGENOERR;
}
