static int
ReadImage(FILE * fd, Sundata * Context, int width, int height, int cmapSize,
	  unsigned char cmap[3][MAXCOLORMAPSIZE],
	  int gray, int interlace)
{
    unsigned char c;
    char *out;
    int i, i1, i2, v, ind, y;
    int xpos = 0, ypos = 0, pass = 0;
    int *ys;
    unsigned char *scan;

    /*
    **	Initialize the compression routines
     */
   
    scan = (unsigned char *)salloc(3*width);
    if (!scan) return 4;
   
    ys = (int *)salloc((height+1)*sizeof(int));
    if (!ys) {
	free(scan);
        return 4;
    }
     
    if (!ReadOK(fd, &c, 1)) {
	return 2;
    }
    if (LWZReadByte(fd, TRUE, c) < 0) {
	return 2;
    }

    while ((v = LWZReadByte(fd, FALSE, c)) >= 0) {
        ind = 3*xpos;
        scan[ind] = cmap[CM_RED][v];
        scan[ind+1] = cmap[CM_GREEN][v];
        scan[ind+2] = cmap[CM_BLUE][v];

	++xpos;
	if (xpos == width) {
	    y = ((2*ypos+1) * Context->zoom.height) / (2*height)
                  - Context->zoom.dy;
            ys[ypos] = y;
            if (y>=0 && y<Context->geom.height && ypos<height) { 
               out = Context->xim->data + y*Context->xim->bytes_per_line;
 	       fill_line(scan, out, 
                     Context->geom.width,  Context->zoom.width,
                     width, Context->zoom.dx);    
	    }
	    xpos = 0;
	    if (interlace) {
		switch (pass) {
		case 0:
		case 1:
		    ypos += 8;
		    break;
		case 2:
		    ypos += 4;
		    break;
		case 3:
		    ypos += 2;
		    break;
		}

		if (ypos >= height) {
		    ++pass;
		    switch (pass) {
		    case 1:
			ypos = 4;
			break;
		    case 2:
			ypos = 2;
			break;
		    case 3:
			ypos = 1;
			break;
		    default:
			goto fini;
		    }
		}
	    } else {
		++ypos;
	    }
	}
	if (ypos >= height)
	    break;
    }

  fini:

    if (Context->zoom.height>=height) {
       ys[height] = Context->geom.height;
       for(ypos=0; ypos<height; ++ypos) {
	  if (ys[ypos]>=0 || ys[ypos+1]<Context->geom.height) {
	     if (ys[ypos]<0) {
                out = Context->xim->data + 
                        ys[ypos+1]*Context->xim->bytes_per_line;
                i1 = 0;
                i2 = ys[ypos+1];
	     } else {
                out = Context->xim->data + 
                        ys[ypos]*Context->xim->bytes_per_line;
                i1 = ys[ypos]+1;
                i2 = ys[ypos+1];
                if (i2>Context->geom.height) i2=Context->geom.height;
	     }
             for (i=i1; i<i2; i++) 
                memcpy(Context->xim->data+i*Context->xim->bytes_per_line, out,
                    Context->xim->bytes_per_line);
	  }
       }
    }

    free(scan);
    free(ys);
    return 0;
}
