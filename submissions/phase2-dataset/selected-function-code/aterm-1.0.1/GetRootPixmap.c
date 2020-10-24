Pixmap
GetRootPixmap (Atom id)
{
	Pixmap currentRootPixmap = None;
#ifndef X_DISPLAY_MISSING
  	Atom act_type;
    int act_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop = NULL;

/*fprintf(stderr, "\n aterm GetRootPixmap(): root pixmap is set");                  */
    if (XGetWindowProperty (  dpy, RootWindow(dpy,DefaultScreen(dpy)), _XROOTPMAP_ID, 0, 1, False, XA_PIXMAP,
							    &act_type, &act_format, &nitems, &bytes_after,
			    				&prop) == Success)
	{
		if (prop)
	  	{
	    	currentRootPixmap = *((Pixmap *) prop);
	    	XFree (prop);
/*fprintf(stderr, "\n aterm GetRootPixmap(): root pixmap is [%lu]", currentRootPixmap); */
		}
	}
#endif
    return currentRootPixmap;
}
