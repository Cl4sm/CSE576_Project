#else
void PWZoomOut(w)
    Widget w;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    int resized;
    
    if (PW->pixmap.zooming) {
	ZoomOut(PW);
	
	resized = InternalResize(PW);
	if (PW->core.visible && !resized)
	    XClearArea(dpy, XtWindow(PW),
		       0, 0, 
		       PW->core.width, PW->core.height,
		       True);
    }
}
