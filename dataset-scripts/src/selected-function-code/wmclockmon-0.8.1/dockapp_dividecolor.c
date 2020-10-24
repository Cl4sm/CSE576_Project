unsigned long
dockapp_dividecolor(char *color_name, int n)
{
    XColor color;

    if ( (n<=0) || (n>127) ) {
	fprintf(stderr, "Divisor must be >0 and <127\n");
	exit(1);
    }

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

		color.red = color.red / n;
		color.green = color.green / n;
		color.blue = color.blue / n;

    color.flags = DoRed | DoGreen | DoBlue;

    if (!XAllocColor(display, DefaultColormap(display, DefaultScreen(display)),
		     &color)) {
	fprintf(stderr, "can't allocate color %s. Using black\n", color_name);
	return BlackPixel(display, DefaultScreen(display));
    }

    return color.pixel;
}