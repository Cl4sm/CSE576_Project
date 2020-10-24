readJPEG(path, Context)
char *path;
Sundata * Context;
{
    struct jpeg_decompress_struct cinfo;
    struct error_mgr jerr;
    FILE *input_file;
    double ratio;
    int i, k, l, m, prev, next, size;
    JSAMPROW scanline[1];
    char *scan, *c;
    char pix[RANGE];
    XColor xc;

    if ((input_file = fopen(path, "r")) == NULL) return 1;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = error_exit;

    if (setjmp(jerr.setjmp_buffer)) {
	/* If we get here, the JPEG code has signaled an error.
	 * We need to clean up the JPEG object, close the input file,
	 * and return.
	 */
        jpeg_destroy_decompress(&cinfo);
	fclose(input_file);
        Context->xim = 0;
	return 2;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, input_file);
    jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE) return 3;
   
    ratio = ((double) cinfo.image_width/(double) Context->zoom.width + 
             (double) cinfo.image_height/(double) Context->zoom.height )/1.8;
    if (ratio>=8.0) 
      cinfo.scale_denom = 8;
    else
    if (ratio>=4.0) 
      cinfo.scale_denom = 4;
    else
    if (ratio>=2.0) 
      cinfo.scale_denom = 2;
    else
      cinfo.scale_denom = 1;

    jpeg_start_decompress(&cinfo);

    Context->xim = XCreateImage(dpy, visual, 
              DefaultDepth(dpy, scr), ZPixmap, 0, NULL, 
              Context->geom.width, Context->geom.height, color_pad, 0);
    XFlush(dpy);
    if (!Context->xim) return 4;

    bytes_per_pixel = (Context->xim->bits_per_pixel/8);
    size = Context->xim->bytes_per_line * Context->geom.height;
    Context->xim->data = (char *) salloc(size);
    scan = (char *) salloc(3 * cinfo.output_width * sizeof(char));

    if (verbose)
       fprintf(stderr, "Loading %s\n"
	    "Rescaling JPEG data by 1/%d,  "
            "%d %d  -->  %d %d,  %d bytes per pixel\n",
	    path, cinfo.scale_denom,
            cinfo.image_width, cinfo.image_height, 
            Context->geom.width, Context->geom.height, 
            bytes_per_pixel);

    prev = -1;
    scanline[0] = (JSAMPROW) scan;

    if (color_depth<=8) 
      for (l=0; l<RANGE; l++) {
        lr[l] = lg[l] = lb[l] = lnum[l] = 0;
      }

    while (cinfo.output_scanline < cinfo.output_height) {
      (void) jpeg_read_scanlines(&cinfo, scanline, (JDIMENSION) 1);
      next = ((2*cinfo.output_scanline - 1) * Context->zoom.height)/
                  (2*(int)cinfo.output_height) - Context->zoom.dy;
      if (next>=0) {
	if (next>=Context->geom.height) {
	   next = Context->geom.height - 1;
	   /* get loop to stop at next iteration ! */
	   cinfo.output_scanline = cinfo.output_height;
	}
	for (l = prev+1; l<= next; l++) {
	  c = Context->xim->data + l * Context->xim->bytes_per_line;
          fill_line(scan, c, Context->geom.width,  Context->zoom.width,
                    cinfo.output_width, Context->zoom.dx);
	}
        prev = next;
      }
    }

    free(scan);
    jpeg_destroy_decompress(&cinfo);

    fclose(input_file);

    if (jerr.pub.num_warnings > 0) {	
	longjmp(jerr.setjmp_buffer, 1);
    }

    if (color_depth<=8) {
      xc.flags = DoRed | DoGreen | DoBlue;
      k = 0;
      for (m=0; m<RANGE; m++) if (lnum[m]) {
        xc.red = (lr[m]/lnum[m])*257;
        xc.green = (lg[m]/lnum[m])*257;
        xc.blue = (lb[m]/lnum[m])*257;
	if (!XAllocColor(dpy, tmp_cmap, &xc)) 
           color_alloc_failed = 1;
	pix[m] = (char) xc.pixel;
	Context->daypixel[k] = (unsigned char) xc.pixel;
	++k;
      }
      Context->ncolors = k;
      for (i=0; i<size; i++) 
	 Context->xim->data[i] = pix[(unsigned char)Context->xim->data[i]];
    }

    return 0;
}
