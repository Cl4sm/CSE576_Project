int readppm(const char * name, int * xres, int * yres, unsigned char **imgdata) {
  char data[256];  
  FILE * ifp;
  int i, bytesread;
  int datasize;
 
  ifp=fopen(name, "r");  
  if (ifp==NULL) {
    return IMAGEBADFILE; /* couldn't open the file */
  }
  fscanf(ifp, "%s", data);
 
  if (strcmp(data, "P6")) {
     fclose(ifp);
     return IMAGEUNSUP; /* not a format we support */
  }

  *xres=getint(ifp);
  *yres=getint(ifp);
      i=getint(ifp); /* eat the maxval number */
  fread(&i, 1, 1, ifp); /* eat the newline */ 
  datasize = 3 * (*xres) * (*yres);

  *imgdata=malloc(datasize); 

  bytesread=fread(*imgdata, 1, datasize, ifp);   

  fclose(ifp);

  if (bytesread != datasize) 
    return IMAGEREADERR;
  
  return IMAGENOERR;
}
