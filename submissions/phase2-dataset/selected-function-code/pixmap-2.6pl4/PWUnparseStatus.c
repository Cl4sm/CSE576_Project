#else
String PWUnparseStatus(w)
    Widget w;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;

    char size_buf[80];
    char *status_ptr;
    
    sprintf(size_buf, "%dx%d", PW->pixmap.width, PW->pixmap.height);
    
    /* 16 is lenght of "Filename: " and " Size:"
       1 is NULL
       don't realloc pixmap.status since alloc might
       fail and we don't want to */
    if ((status_ptr = XtMalloc((strlen(size_buf) + 
			       strlen(PW->pixmap.filename) +
			       16 + 1) * sizeof(char))))
      {
	if (PW->pixmap.status)
	  XtFree(PW->pixmap.status);
	PW->pixmap.status = status_ptr;
	sprintf(PW->pixmap.status, 
		"Filename: %s Size:%dx%d",
		PW->pixmap.filename, PW->pixmap.width, PW->pixmap.height);
      }

    return PW->pixmap.status;
}
