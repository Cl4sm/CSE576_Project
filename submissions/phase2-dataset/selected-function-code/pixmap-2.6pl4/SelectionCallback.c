    Widget w;
    XtPointer client_data;
    Atom *selection, *type;
    XtPointer value;
    unsigned long *length;
    int *format;
{
    PixmapWidget PW = (PixmapWidget) w;
    Pixmap *pixmap;

   switch (*type) {
	
    case XA_BITMAP:
    case XA_PIXMAP:
	DestroyPixmapImage(&PW->pixmap.storage);
	DestroyMaskImage(&PW->pixmap.mask_storage);
	pixmap = (Pixmap *) value;
	PW->pixmap.storage = GetImage(PW, *pixmap);
	PW->pixmap.mask_storage =
	  CreateMaskImage(PW,
			  (Dimension) PW->pixmap.mask_image->width,
			  (Dimension) PW->pixmap.mask_image->height);
	XFreePixmap(XtDisplay(w), *pixmap);
	break;
	
    case XA_STRING:
	if (_PWDEBUG)
	    fprintf(stderr, "Received:%s\n", (char *)value);
	break;

    default:
	XtAppWarning(XtWidgetToApplicationContext(w),
		     " selection request failed.  PixmapWidget");
	break;
    }

    PW->pixmap.selection.limbo = FALSE;
}
