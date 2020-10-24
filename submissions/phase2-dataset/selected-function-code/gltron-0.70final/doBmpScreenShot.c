void doBmpScreenShot(Visual *display) {
  screenshot_info_t screenshot;
  static int last_bmp_num; /* store last free file index. */
  char *path;
  path = getNextFilename(".bmp", &last_bmp_num);
  if(path != NULL) {
    if (captureScreenToPixmap(&screenshot, display) != 0) {
      fprintf(stderr, "Error capturing screenshot\n");
      return;
    }
  
    if (SystemWriteBMP(path, screenshot.width, screenshot.height,
		       screenshot.pixmap) != 0) {
      fprintf(stderr, "Error writing screenshot %s\n", path);
    } else {
      fprintf(stderr, "Screenshot written to %s\n", path);
    }
  
    free(screenshot.pixmap);
    free(path);
  }
}
