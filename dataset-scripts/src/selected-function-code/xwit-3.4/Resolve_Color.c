unsigned long Resolve_Color(w, name)
     Window w;
     char *name;
{
	XColor c;
	Colormap colormap;
	XWindowAttributes wind_info;

	/*
	 * The following is a hack to insure machines without a rgb table
	 * handle at least white & black right.
	 */
	if (!strcmp(name, "white"))
	  name="#ffffffffffff";
	if (!strcmp(name, "black"))
	  name="#000000000000";

	XGetWindowAttributes(dpy, w, &wind_info);
	colormap = wind_info.colormap;

	if (!XParseColor(dpy, colormap, name, &c))
	  Fatal_Error("Bad color format '%s'.", name);

	if (!XAllocColor(dpy, colormap, &c))
	  Fatal_Error("XAllocColor failed!");

	return(c.pixel);
}