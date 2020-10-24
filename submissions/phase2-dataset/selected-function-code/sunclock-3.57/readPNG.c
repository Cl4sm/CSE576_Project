int
readPNG(path, Context)
char *path;
Sundata * Context;
{
    FILE          *fp;
    png_structp   png_ptr;
    png_infop     info_ptr;
    png_uint_32   width, height;
    png_colorp    palette;
    png_bytep	  *row_pointers, png_data, png;
    int           bit_depth, color_type, interlace_type, num_palette, rowbytes;
    int           y, i, j, i1, i2, size, destroy;
    unsigned char * scan, *out;
    unsigned char c, *scanp;
    int * ys;
   
    if ((fp = fopen(path, "rb")) == (FILE *)NULL) {
        return 1;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
      &jmpbuf_struct, png_error_handler, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 2;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 2;
    }

    if (setjmp(jmpbuf_struct.jmpbuf)) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 2;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
      &interlace_type, NULL, NULL);

    switch (color_type) {

        case PNG_COLOR_TYPE_PALETTE:
            if (!png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                fclose(fp);
                return 2;
            }
            break;

        case PNG_COLOR_TYPE_RGB:
            if (bit_depth == 16) {
                png_set_strip_16(png_ptr);
            }
            break;

        case PNG_COLOR_TYPE_GRAY:   /* treat grayscale as special colormap */
            if (bit_depth == 16) {
                png_set_strip_16(png_ptr);
                bit_depth = 8;
            }
            break;

        case PNG_COLOR_TYPE_RGB_ALPHA:
            if (bit_depth == 16) {
                png_set_strip_16(png_ptr);
            }
            break;

        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth == 16) {
                png_set_strip_16(png_ptr);
            }
            break;

        default:
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return 2;
    }
  
    Context->xim = XCreateImage(dpy, visual, 
                     DefaultDepth(dpy, scr), ZPixmap, 0, NULL, 
                     Context->geom.width, Context->geom.height, color_pad, 0);
    XFlush(dpy);
    if (!Context->xim)
       return 4;
    bytes_per_pixel = (Context->xim->bits_per_pixel/8);
    size = Context->xim->bytes_per_line * Context->geom.height;
    Context->xim->data = (char *) salloc(size);
    if (!Context->xim->data) {
        XDestroyImage(Context->xim);
        return 4;
    }

    if (bit_depth < 8)
        png_set_packing(png_ptr);

    if (interlace_type)
        /* npasses = */ png_set_interlace_handling(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    destroy = 0;
    png_data = (png_bytep) salloc(height*rowbytes);
    row_pointers = (png_bytep *)salloc(height * sizeof(png_bytep));
    scan = (char *)salloc(3*width);
    ys = (int *)salloc((height+1)*sizeof(int));
   
    if (!png_data || !row_pointers || !scan || !ys) {
       if (png_data) free(png_data);
       png_data = NULL;
       if (row_pointers) free(row_pointers);
       row_pointers = NULL;
       if (scan) free(scan);
       scan = NULL;
       if (ys) free(ys);
       ys = NULL;
       XDestroyImage(Context->xim);
       png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
       fclose(fp);
       return 4;
    }

    for (j=0; j<height; ++j)
        row_pointers[j] = (png_bytep)png_data + j*rowbytes;
    png_read_image(png_ptr, row_pointers);

    if (verbose)
       fprintf(stderr, 
	       "Reading %d x %d PNG image, color type = %d, rowbytes = %d\n",
	       (int) width, (int) height, color_type, rowbytes);
   
    for (j=0; j<height; ++j) {
        y = ((2*j+1) * Context->zoom.height) / (2*height) - Context->zoom.dy;
        ys[j] = y;
	if (y>=0 && y<Context->geom.height) {
           out = Context->xim->data + y*Context->xim->bytes_per_line;
	   png = png_data + j*rowbytes;	   
	   scanp = scan;
	   if (color_type == PNG_COLOR_TYPE_PALETTE) {
	      for(i=0; i<width; i++) {
	          c = *png++;
	          *scanp++ = palette[c].red;
	          *scanp++ = palette[c].green;
	          *scanp++ = palette[c].blue;
	      }
	   } else
	   if (color_type == PNG_COLOR_TYPE_GRAY) {
	      for(i=0; i<width; i++) {
	         *scanp++ = c = *png++;
	         *scanp++ = c;
	         *scanp++ = c;
	      }
	   } else
	   if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
	      for(i=0; i<width; i++) {
	         *scanp++ = c = *png++;
	         *scanp++ = c;
	         *scanp++ = c;
                 png++; /* skip alpha channel */
	      }
	   } else
	   if (color_type == PNG_COLOR_TYPE_RGB) {
              for (i=0; i<width; i++) {
	         *scanp++ = *png++;
	         *scanp++ = *png++;
	         *scanp++ = *png++;
	      }
	   } else
	   if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
              for (i=0; i<width; i++) {
	         *scanp++ = *png++;
	         *scanp++ = *png++;
	         *scanp++ = *png++;
	         png++; /* skip alpha channel */
	      }
	   }
           fill_line(scan, out, 
                     Context->geom.width, Context->zoom.width,
                     width, Context->zoom.dx);
	}
    }

    /* Close everything */
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(png_data);
    free(row_pointers);
    free(scan);
    fclose(fp);

    /* fill intermediate lines in output, if necessary*/
    if (Context->zoom.height>=height) {
       ys[height] = Context->geom.height;
       for(y=0; y<height; ++y) {
	  if (ys[y]>=0 || ys[y+1]<Context->geom.height) {
	     if (ys[y]<0) {
                out = Context->xim->data + 
                        ys[y+1]*Context->xim->bytes_per_line;
                i1 = 0;
                i2 = ys[y+1];
	     } else {
                out = Context->xim->data + 
                        ys[y]*Context->xim->bytes_per_line;
                i1 = ys[y]+1;
                i2 = ys[y+1];
                if (i2>Context->geom.height) i2=Context->geom.height;
	     }
             for (i=i1; i<i2; i++) 
                memcpy(Context->xim->data+i*Context->xim->bytes_per_line, out,
                    Context->xim->bytes_per_line);
	  }
       }
    }

    free(ys);   
    return 0;

} /* end function ReadPNG() */
