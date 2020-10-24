void PWRequestSelection(Widget w, Time time, Boolean wait)
#else
void PWRequestSelection(w, time, wait)
    Widget w;
    Time time;
    Boolean wait;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    
    XtGetSelectionValue(w, XA_PRIMARY, XA_PIXMAP,
			SelectionCallback, NULL, time);

    PW->pixmap.selection.limbo = TRUE;

    if (wait)
	while (PW->pixmap.selection.limbo) {
	    XEvent event;
	    XtAppNextEvent(XtWidgetToApplicationContext(w), &event);
	    XtDispatchEvent(&event);
	}
}
