    Widget w;
    Atom *selection, *target, *type;
    XtPointer *value;
    unsigned long *length;
    int *format;
{
    PixmapWidget PW = (PixmapWidget) w;
    Pixmap *pixmap;
    XImage *image;
    Dimension width, height;
 
    switch (*target) {
/*
    case XA_TARGETS:
	*type = XA_ATOM;
	*value = (XtPointer) pixmapClassRec.pixmap_class.targets;
	*length = pixmapClassRec.pixmap_class.num_targets;
	*format = 32;
	return True;
*/
    case XA_BITMAP:
    case XA_PIXMAP:
	if (!PWQueryMarked(w)) return False;
	width = PW->pixmap.mark.to_x - PW->pixmap.mark.from_x + 1;
	height = PW->pixmap.mark.to_y - PW->pixmap.mark.from_y + 1;
	image = CreatePixmapImage(PW, width, height);
	CopyImageData(PW->pixmap.image, image, 
		      PW->pixmap.mark.from_x, PW->pixmap.mark.from_y,
		      PW->pixmap.mark.to_x, PW->pixmap.mark.to_y, 0, 0);
	pixmap = (Pixmap *) XtMalloc(sizeof(Pixmap));
	*pixmap = GetPixmap(PW, image);
	DestroyPixmapImage(&image);
	*type = XA_PIXMAP;
	*value = (XtPointer) pixmap;
	*length = 1;
	*format = 32;
	return True;

    case XA_STRING:
	*type = XA_STRING;
	*value = "Hello world!\n";
	*length = XtStrlen(*value);
	*format = 8;
	return True;

    default:
	return False;
    }
}
