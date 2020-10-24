int readimage(rawimage * img) {
  int rc;
  int xres, yres, zres;
  unsigned char * imgdata;
  char * name = img->name;
  char msgtxt[2048];
 
  xres=1;
  yres=1;
  zres=1;

  if (strstr(name, ".ppm")) { 
    rc = readppm(name, &xres, &yres, &imgdata);
  } else if (strstr(name, ".tga")) {
    rc = readtga(name, &xres, &yres, &imgdata);
  } else if (strstr(name, ".jpg")) {
    rc = readjpeg(name, &xres, &yres, &imgdata);
  } else if (strstr(name, ".png")) {
    rc = readpng(name, &xres, &yres, &imgdata);
  } else if (strstr(name, ".gif")) {
    rc = IMAGEUNSUP; 
  } else if (strstr(name, ".tiff")) {
    rc = IMAGEUNSUP; 
  } else if (strstr(name, ".rgb")) {
    rc = IMAGEUNSUP; 
  } else if (strstr(name, ".xpm")) {
    rc = IMAGEUNSUP; 
  } else {
    rc = readppm(name, &xres, &yres, &imgdata);
  } 

  switch (rc) {
    case IMAGEREADERR:
      sprintf(msgtxt, "Short read encountered while loading image %s", name);
      rt_ui_message(MSG_0, msgtxt);
      rc = IMAGENOERR; /* remap to non-fatal error */
      break;

    case IMAGEUNSUP:
      sprintf(msgtxt, "Cannot read unsupported format for image %s", name);
      rt_ui_message(MSG_0, msgtxt);
      break;
  }    

  /* If the image load failed, create a tiny white colored image to fake it */ 
  /* this allows a scene to render even when a file can't be loaded */
  if (rc != IMAGENOERR) {
    rc = fakeimage(name, &xres, &yres, &imgdata);
  }

  /* If we succeeded in loading the image, return it. */
  if (rc == IMAGENOERR) { 
    img->xres = xres;
    img->yres = yres;
    img->zres = zres;
    img->bpp = 3;  
    img->data = imgdata;
  }

  return rc;
}
