  int format, width, height, w1, w2, h1, h2, depth, flags;
  int imgsize, bytesread, i, tmp;
  FILE * ifp;

  ifp=fopen(name, "r");  
  if (ifp==NULL) {
    return IMAGEBADFILE; /* couldn't open the file */
  }

  /* read the targa header */
  getc(ifp); /* ID length */
  getc(ifp); /* colormap type */
  format = getc(ifp); /* image type */
  getc(ifp); /* color map origin */
  getc(ifp); /* color map origin */
  getc(ifp); /* color map length */
  getc(ifp); /* color map length */
  getc(ifp); /* color map entry size */
  getc(ifp); /* x origin */
  getc(ifp); /* x origin */
  getc(ifp); /* y origin */
  getc(ifp); /* y origin */
  w1 = getc(ifp); /* width (low) */
  w2 = getc(ifp); /* width (hi) */
  h1 = getc(ifp); /* height (low) */
  h2 = getc(ifp); /* height (hi) */
  depth = getc(ifp); /* image pixel size */
  flags = getc(ifp); /* image descriptor byte */

  if ((format != 2) || (depth != 24)) {
    fclose(ifp);
    return IMAGEUNSUP; /* unsupported targa format */
  }
    

  width = ((w2 << 8) | w1);
  height = ((h2 << 8) | h1);

  imgsize = 3 * width * height;
  *imgdata = malloc(imgsize);
  bytesread = fread(*imgdata, 1, imgsize, ifp);
  fclose(ifp);

  /* flip image vertically */
  if (flags == 0x20) {
    int rowsize = 3 * width;
    unsigned char * copytmp;

    copytmp = malloc(rowsize);

    for (i=0; i<height / 2; i++) {
      memcpy(copytmp, &((*imgdata)[rowsize*i]), rowsize);
      memcpy(&(*imgdata)[rowsize*i], &(*imgdata)[rowsize*(height - 1 - i)], rowsize);
      memcpy(&(*imgdata)[rowsize*(height - 1 - i)], copytmp, rowsize);
    }

    free(copytmp);       
  }


  /* convert from BGR order to RGB order */
  for (i=0; i<imgsize; i+=3) {
    tmp = (*imgdata)[i]; /* Blue */
    (*imgdata)[i] = (*imgdata)[i+2]; /* Red */
    (*imgdata)[i+2] = tmp; /* Blue */    
  }

  *xres = width;
  *yres = height;

  if (bytesread != imgsize) 
    return IMAGEREADERR;

  return IMAGENOERR;
}
