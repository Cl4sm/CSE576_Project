rawimage * AllocateImageFile(const char * filename) { 
  rawimage * newimage = NULL;
  int i, len, intable;

  intable=0;
  if (numimages!=0) {
    for (i=0; i<numimages; i++) {
      if (!strcmp(filename, imagelist[i]->name)) {
        newimage=imagelist[i];
        intable=1;
      }
    }
  }

  if (!intable) {
    newimage=malloc(sizeof(rawimage));
    newimage->loaded=0;
    newimage->xres=0;
    newimage->yres=0;
    newimage->zres=0;
    newimage->bpp=0;
    newimage->data=NULL;
    len=strlen(filename);
    if (len > 80) 
      return NULL;
    strcpy(newimage->name, filename);

    imagelist[numimages]=newimage;  /* add new one to the table       */ 
    numimages++;                    /* increment the number of images */
  }
 
  return newimage;
}
