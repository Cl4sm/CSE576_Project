               int imgbufferformat, int fileformat) {
  if (img == NULL) 
    return IMAGENULLDATA;

  if (imgbufferformat == RT_IMAGE_BUFFER_RGB24) {
    unsigned char *imgbuf = (unsigned char *) img;
 
    switch (fileformat) {
      case RT_FORMAT_PPM:
        return writeppm(name, xres, yres, imgbuf);
    
      case RT_FORMAT_SGIRGB:
        return writergb(name, xres, yres, imgbuf);

      case RT_FORMAT_JPEG:
        return writejpeg(name, xres, yres, imgbuf);

      case RT_FORMAT_PNG:
        return writepng(name, xres, yres, imgbuf);

      case RT_FORMAT_WINBMP:
        return writebmp(name, xres, yres, imgbuf);

      case RT_FORMAT_TARGA:
        return writetga(name, xres, yres, imgbuf);       

      default:
        printf("Unsupported image format combination\n");
        return IMAGEUNSUP;
    } 
  } else {
    unsigned char *imgbuf = (unsigned char *) img;
    int rc;

    switch (fileformat) {
      case RT_FORMAT_PPM:
        imgbuf = image_rgb24_from_rgb96f(xres, yres, img);
        rc = writeppm(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   
 
      case RT_FORMAT_SGIRGB:
        imgbuf = image_rgb24_from_rgb96f(xres, yres, img);
        rc = writergb(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   

      case RT_FORMAT_JPEG:
        imgbuf = image_rgb24_from_rgb96f(xres, yres, img);
        rc = writejpeg(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   

      case RT_FORMAT_PNG:
        imgbuf = image_rgb24_from_rgb96f(xres, yres, img);
        rc = writepng(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   

      case RT_FORMAT_WINBMP:
        imgbuf = image_rgb24_from_rgb96f(xres, yres, img);
        rc = writebmp(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   

      case RT_FORMAT_TARGA:
        imgbuf = image_rgb24_from_rgb96f(xres, yres, img);
        rc = writetga(name, xres, yres, imgbuf);       
        free(imgbuf);
        return rc;   

      case RT_FORMAT_PPM48:
        imgbuf = image_rgb48be_from_rgb96f(xres, yres, img);
        rc = writeppm48(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   

      case RT_FORMAT_PSD48:
        imgbuf = image_rgb48bepl_from_rgb96f(xres, yres, img);
        rc = writepsd48(name, xres, yres, imgbuf);
        free(imgbuf);
        return rc;   

      default:
        printf("Unsupported image format combination\n");
        return IMAGEUNSUP;
    } 
  } 
}
