int writergb(char *name, int xres, int yres, unsigned char *imgdata) {
  FILE * ofp;
  char iname[80];               /* Image name */
  int x, y, i;

  if ((ofp = fopen(name, "wb")) != NULL) {
    putshort(ofp, 474);         /* Magic                       */
    putbyte(ofp, 0);            /* STORAGE is VERBATIM         */
    putbyte(ofp, 1);            /* BPC is 1                    */
    putshort(ofp, 3);           /* DIMENSION is 3              */
    putshort(ofp, (unsigned short) xres);        /* XSIZE      */
    putshort(ofp, (unsigned short) yres);        /* YSIZE      */
    putshort(ofp, 3);           /* ZSIZE                       */
    putint(ofp, 0);             /* PIXMIN is 0                 */
    putint(ofp, 255);           /* PIXMAX is 255               */

    for(i=0; i<4; i++)          /* DUMMY 4 bytes               */
      putbyte(ofp, 0);

    strcpy(iname, "Tachyon Ray Tracer Image");
    fwrite(iname, 80, 1, ofp);  /* IMAGENAME                   */
    putint(ofp, 0);             /* COLORMAP is 0               */

    for(i=0; i<404; i++)        /* DUMMY 404 bytes             */
      putbyte(ofp,0);

    for(i=0; i<3; i++)
      for(y=0; y<yres; y++)
        for(x=0; x<xres; x++)
          fwrite(&imgdata[(y*xres + x)*3 + i], 1, 1, ofp);

    fclose(ofp);
  }

  return IMAGENOERR;
}
