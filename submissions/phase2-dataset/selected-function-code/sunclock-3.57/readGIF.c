int
readGIF(path, Context)
char *path;
Sundata * Context;
{
    unsigned char buf[16];
    unsigned char c;
    unsigned char localColorMap[3][MAXCOLORMAPSIZE];
    int output = 0, size;
    int grayScale;
    int useGlobalColormap;
    int bitPixel;
    char version[4];
    FILE *fd = fopen(path, "r");

    if (fd == NULL)
	return 1;

    if (!ReadOK(fd, buf, 6)) {
	fclose(fd);
	return 2;
    }
    if (strncmp((char *) buf, "GIF", 3) != 0) {
	fclose(fd);
	return 3;
    }
    strncpy(version, (char *) buf + 3, 3);
    version[3] = '\0';

    if ((strcmp(version, "87a") != 0) && (strcmp(version, "89a") != 0)) {
	fclose(fd);
	return 3;
    }
    if (!ReadOK(fd, buf, 7)) {
	fclose(fd);
	return 3;
    }
    GifScreen.Width = LM_to_uint(buf[0], buf[1]);
    GifScreen.Height = LM_to_uint(buf[2], buf[3]);
    GifScreen.BitPixel = 2 << (buf[4] & 0x07);
    GifScreen.ColorResolution = (((buf[4] & 0x70) >> 3) + 1);
    GifScreen.Background = buf[5];
    GifScreen.AspectRatio = buf[6];

    if (BitSet(buf[4], LOCALCOLORMAP)) {	/* Global Colormap */
	if (ReadColorMap(fd, GifScreen.BitPixel, GifScreen.ColorMap,
			 &GifScreen.Grayscale)) {
	    fclose(fd);
	    return 2;
	}
    }
 repeat:
    if (!ReadOK(fd, &c, 1)) {
        fclose(fd);
        return 2;
    }
    if (c == '!') {        /* Extension */
        if (!ReadOK(fd, &c, 1)) {
           fclose(fd);
           return 2;
	}
        DoExtension(fd, c);
        goto repeat;
    }
    if (c != ',') {        /* Not a valid start character */
        goto repeat;
    }

    if (!ReadOK(fd, buf, 9)) {
        fclose(fd);
        return 2;
    }
    useGlobalColormap = !BitSet(buf[8], LOCALCOLORMAP);

    bitPixel = 1 << ((buf[8] & 0x07) + 1);

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

    if (!useGlobalColormap) {
        if (ReadColorMap(fd, bitPixel, localColorMap, &grayScale)) {
           XDestroyImage(Context->xim);
           return 2;
        }
        output = ReadImage(fd, Context, 
		  LM_to_uint(buf[4], buf[5]),
                  LM_to_uint(buf[6], buf[7]),
                  bitPixel, localColorMap, grayScale,
                  BitSet(buf[8], INTERLACE));
    } else {
        output = ReadImage(fd, Context, 
		  LM_to_uint(buf[4], buf[5]),
                  LM_to_uint(buf[6], buf[7]),
                  GifScreen.BitPixel, GifScreen.ColorMap,
                  GifScreen.Grayscale, BitSet(buf[8], INTERLACE));
    }

    if (output) XDestroyImage(Context->xim);
    return output;
}
