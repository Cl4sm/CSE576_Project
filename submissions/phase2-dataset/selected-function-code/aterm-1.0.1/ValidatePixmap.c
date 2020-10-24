ValidatePixmap (Pixmap p, int bSetHandler, int bTransparent, unsigned int *pWidth, unsigned int *pHeight)
{
#ifndef X_DISPLAY_MISSING
	int (*oldXErrorHandler) (Display *, XErrorEvent *) = NULL;
    /* we need to check if pixmap is still valid */
	Window root;
    int junk;
	if (bSetHandler)
		oldXErrorHandler = XSetErrorHandler (pixmap_error_handler);

    if (bTransparent)
	    p = GetRootPixmap (None);
	if (!pWidth)
  		pWidth = &junk;
    if (!pHeight)
	    pHeight = &junk;

    if (p != None)
	{
  		if (!XGetGeometry (dpy, p, &root, &junk, &junk, pWidth, pHeight, &junk, &junk))
			p = None;
    }
	if(bSetHandler)
  		XSetErrorHandler (oldXErrorHandler);

	return p;
#else
	return None ;
#endif
}
