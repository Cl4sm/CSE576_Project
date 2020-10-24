static Bool MyFetchName(Display *display, Window w, unsigned char **name)
{
	Atom returnedType;
	int returnedFormat;
	unsigned long number;
	unsigned long bytesAfterReturn;
	unsigned char *data;

	if( Success != XGetWindowProperty(display, w, property,
				0, (long)BUFSIZ, False,
				XA_STRING,
				&returnedType, &returnedFormat,
				&number, &bytesAfterReturn, &data)) {
		*name = NULL;
		return False;
	} else if( returnedType != XA_STRING || returnedFormat != 8 ) {
		if(data)
			XFree(data);
		*name = NULL;
		return False;
	} else {
		*name = data;
		return (data!=NULL)?True:False;
	}
}