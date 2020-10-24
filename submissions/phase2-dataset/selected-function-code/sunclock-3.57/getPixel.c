getPixel(cmap, str)
Colormap cmap;
char *str;
{
Status s;
XColor c, e;

      if (!str) {
	 color_alloc_failed += 2;;
	 return 0;
      }

      s = XAllocNamedColor(dpy, cmap, str, &c, &e);
      if (s != (Status)1) {
	 fprintf(stderr, "%s: warning: can't allocate color `%s'\n",
		 ProgName, buffer);
	 color_alloc_failed += 2;
	 return 0;
      }
      return c.pixel;
}
