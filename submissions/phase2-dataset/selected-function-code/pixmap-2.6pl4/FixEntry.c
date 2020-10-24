    Widget w;
    int *id;
{
    int n;
    Arg wargs[2];


    n = 0;
    switch (*id) {
	
    case Image:
#ifndef USE_ATHENA
        XtSetArg(wargs[n], XmNset, image_visible); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNleftBitmap, 
		 image_visible ? check_mark : None); n++;
#endif /* USE_ATHENA */ 
	break;
	
    case Grid:
#ifndef USE_ATHENA
        XtSetArg(wargs[n], XmNset, PWQueryGrid(pixmap_widget)); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNleftBitmap, 
		 PWQueryGrid(pixmap_widget) ? check_mark : None); n++;
#endif /* USE_ATHENA */
	break;

    case Axes:
#ifndef USE_ATHENA
        XtSetArg(wargs[n], XmNset, PWQueryAxes(pixmap_widget)); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNleftBitmap, 
		 PWQueryAxes(pixmap_widget) ? check_mark : None); n++;
#endif /* USE_ATHENA */
	break;
	
    case Proportional:
#ifndef USE_ATHENA
        XtSetArg(wargs[n], XmNset, PWQueryProportional(pixmap_widget)); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNleftBitmap, 
		 PWQueryProportional(pixmap_widget) ? check_mark : None); n++;
#endif /* USE_ATHENA */
	break;
	
    case Zoom:
#ifndef USE_ATHENA
        XtSetArg(wargs[n], XmNset, PWQueryZooming(pixmap_widget)); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNleftBitmap, 
		 PWQueryZooming(pixmap_widget) ? check_mark : None); n++;
#endif /* USE_ATHENA */
	break;

    case Copy:
    case Cut:
#ifndef USE_ATHENA
	XtSetArg(wargs[n], XmNsensitive, PWQueryMarked(pixmap_widget)); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNsensitive, PWQueryMarked(pixmap_widget)); n++;
#endif /* USE_ATHENA */
	break;

    case Paste:
#ifndef USE_ATHENA
	XtSetArg(wargs[n], XmNsensitive, PWQueryStored(pixmap_widget)); n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNsensitive, PWQueryStored(pixmap_widget)); n++;
#endif /* USE_ATHENA */
	break;

    case Crop:
#ifndef USE_ATHENA
	XtSetArg(wargs[n], XmNsensitive,
		 PWQueryMarked(pixmap_widget) || PWQueryStored(pixmap_widget));
	n++;
#else /* USE_ATHENA */
	XtSetArg(wargs[n], XtNsensitive,
		 PWQueryMarked(pixmap_widget) || PWQueryStored(pixmap_widget));
	n++;
#endif /* USE_ATHENA */
	break;

    default:
	return;
    }
    
    XtSetValues(w, wargs, n);
}
