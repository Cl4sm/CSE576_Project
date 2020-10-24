  png_structp png_ptr;
  png_infop info_ptr;
  png_byte **row_ptrs;
  int i;

  if (!(fp = fopen(fname, "wb"))) {
    fprintf(stderr, "can't open %s for writing\n", fname);
    return -1;
  }

  if (!(png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                          NULL, NULL, NULL))) {
    return -1;
  }

  if (!(info_ptr = png_create_info_struct(png_ptr))) {
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    return -1;
  }

  png_set_write_fn(png_ptr, 0, user_write_data, user_flush_data);
  /* png_init_io(png_ptr, fp); */

  png_set_IHDR(png_ptr, info_ptr, screenshot->width, screenshot->height,
	             SCREENSHOT_PNG_BITDEPTH, PNG_COLOR_TYPE_RGB, 
	             PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
	             PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png_ptr, info_ptr);

  /* get pointers */
  if(!(row_ptrs = (png_byte**) malloc(screenshot->height * sizeof(png_byte*)))) {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return -1;
  }
  
  for(i = 0; i < screenshot->height; i++) {
    row_ptrs[i] = screenshot->pixmap + (screenshot->height - i - 1) 
      * SCREENSHOT_BYTES_PER_PIXEL * screenshot->width;
  }

  png_write_image(png_ptr, row_ptrs);
  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr); 
  
  free(row_ptrs);
  return 0;
}
