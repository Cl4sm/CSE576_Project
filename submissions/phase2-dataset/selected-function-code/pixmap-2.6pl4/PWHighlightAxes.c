#else
void PWHighlightAxes(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  
  XDrawLine(dpy, XtWindow(PW),
	    PW->pixmap.framing_gc,
	    InWindowX(PW, 0), 
	    InWindowY(PW, 0),
	    InWindowX(PW, PW->pixmap.width),
	    InWindowY(PW, PW->pixmap.height));
  XDrawLine(dpy, XtWindow(PW),
	    PW->pixmap.framing_gc,
	    InWindowX(PW, PW->pixmap.width),
	    InWindowY(PW, 0), 
	    InWindowX(PW, 0),
	    InWindowY(PW, PW->pixmap.height));
  if (PWQueryGrid((Widget)PW))
    {
      if ((((int)(PW->pixmap.height / 2.0)) * 2) != PW->pixmap.height)
	XDrawLine(dpy, XtWindow(PW),
		  PW->pixmap.framing_gc,
		  InWindowX(PW, 0),
		  InWindowY(PW, PW->pixmap.height / 2.0),
		  InWindowX(PW, PW->pixmap.width),
		  InWindowY(PW, PW->pixmap.height / 2.0));
      if ((((int)(PW->pixmap.width / 2.0)) * 2) != PW->pixmap.width)
	XDrawLine(dpy, XtWindow(PW),
		  PW->pixmap.framing_gc,
		  InWindowX(PW, PW->pixmap.width / 2.0),
		  InWindowY(PW, 0),
		  InWindowX(PW, PW->pixmap.width / 2.0),
		  InWindowY(PW, PW->pixmap.height));
    }
}
