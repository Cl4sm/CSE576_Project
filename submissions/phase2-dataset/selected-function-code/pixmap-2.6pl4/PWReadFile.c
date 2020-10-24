#else
int PWReadFile(w, filename)
    Widget w;
    String filename;
#endif
{
    int status, resized;
    PixmapWidget PW = (PixmapWidget) w;

    status = readFile(PW, filename);
    if (status == XpmSuccess)
      {
	XtFree(PW->pixmap.filename);
	PW->pixmap.filename = XtNewString(filename);

	PWUnmark(w);
	resized = InternalResize(PW);

	if (PW->core.visible && !resized) {
	    XClearArea(dpy, XtWindow(PW),
		       0, 0, 
		       PW->core.width, PW->core.height,
		       True);
	}
      }
    else
	XtAppWarning(XtWidgetToApplicationContext(w),
		     " read file failed.  PixmapWidget");
    
    return status;
}
