void doPngScreenShot(Visual *display) {
  screenshot_info_t screenshot;
  static int last_png_num; /* store last free file index. */
  char *path;
  path = getNextFilename(".png", &last_png_num);
  if(path != NULL) {
    if (captureScreenToPixmap(&screenshot, display) != 0) {
      fprintf(stderr, "Error capturing screenshot\n");
      return;
    }
  
    if (writePixmapToPng(&screenshot, path) != 0) {
      fprintf(stderr, "Error writing screenshot %s\n", path);
    } else {
      fprintf(stderr, "Screenshot written to %s\n", path);
    }
    free(screenshot.pixmap);
    free(path);
  }
}
