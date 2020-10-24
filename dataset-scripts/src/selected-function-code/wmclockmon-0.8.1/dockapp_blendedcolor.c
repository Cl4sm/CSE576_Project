unsigned long
dockapp_blendedcolor(char *color_name, int r, int g, int b, float fac)
{
    XColor color;

    if ((r < -255 || r > 255)||(g < -255 || g > 255)||(b < -255 || b > 255)){
	fprintf(stderr, "r:%d,g:%d,b:%d (r,g,b must be 0 to 255)", r, g, b);
	exit(1);
    }

    r *= 255;
    g *= 255;
    b *= 255;

    if (!XParseColor(display, DefaultColormap(display, DefaultScreen(display)),
		     color_name, &color))
	fprintf(stderr, "can't parse color %s\n", color_name), exit(1);

    if (!XAllocColor(display, DefaultColormap(display, DefaultScreen(display)),
		     &color)) {
	fprintf(stderr, "can't allocate color %s. Using black\n", color_name);
	return BlackPixel(display, DefaultScreen(display));
    }

    if (DefaultDepth(display, DefaultScreen(display)) < 16)
	return color.pixel;

    /* red */
    if (color.red + r > 0xffff) {
	color.red = 0xffff;
    } else if (color.red + r < 0) {
	color.red = 0;
    } else {
	color.red = (unsigned short)(fac * color.red + r);
    }

    /* green */
    if (color.green + g > 0xffff) {
	color.green = 0xffff;
    } else if (color.green + g < 0) {
	color.green = 0;
    } else {
	color.green = (unsigned short)(fac * color.green + g);
    }

    /* blue */
    if (color.blue + b > 0xffff) {
	color.blue = 0xffff;
    } else if (color.blue + b < 0) {
	color.blue = 0;
    } else {
	color.blue = (unsigned short)(fac * color.blue + b);
    }

    color.flags = DoRed | DoGreen | DoBlue;

    if (!XAllocColor(display, DefaultColormap(display, DefaultScreen(display)),
		     &color)) {
	fprintf(stderr, "can't allocate color %s. Using black\n", color_name);
	return BlackPixel(display, DefaultScreen(display));
    }

    return color.pixel;
}