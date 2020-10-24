int
GetRootDimensions (int *width, int *height)
{
#ifndef X_DISPLAY_MISSING
	Window root;
	int w_x, w_y;
	unsigned int junk;
    if( dpy == NULL )
        return 0;
	if (!XGetGeometry (dpy, RootWindow(dpy,DefaultScreen(dpy)), &root,
					     &w_x, &w_y, width, height, &junk, &junk))
    {
    	*width = 0;
    	*height = 0;
    }
	return (*width > 0 && *height > 0) ? 1 : 0;
#else
	return 0;
#endif
}
