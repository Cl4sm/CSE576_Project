Bool
read_32bit_proplist (Window w, Atom property, long estimate, CARD32 ** list, long *nitems)
{
	Bool          res = False;

#ifndef X_DISPLAY_MISSING
	if (w != None && property != None && list && nitems)
	{
		Atom          actual_type;
		int           actual_format;
        unsigned long bytes_after;
		unsigned long unitems = 0 ;
		union 
		{
		 	unsigned char *uc_ptr ;	
		 	long *long_ptr ;	
		}buffer;

		buffer.long_ptr = NULL ; 
		if (estimate <= 0)
			estimate = 1;
		res =
			(XGetWindowProperty
			 (Xdisplay, w, property, 0, estimate, False, AnyPropertyType,
			  &actual_type, &actual_format, &unitems, &bytes_after, &buffer.uc_ptr) == 0);
		/* checking property sanity */
		res = (res && unitems > 0 && actual_format == 32);

		if (bytes_after > 0 && res)
		{
			XFree (buffer.long_ptr);
			res =
				(XGetWindowProperty
				 (Xdisplay, w, property, 0, estimate + (bytes_after >> 2), False,
				  actual_type, &actual_type, &actual_format, &unitems, &bytes_after, &buffer.uc_ptr) == 0);
			res = (res && (unitems > 0));	   /* bad property */
		}

		if (!res)
		{
			*nitems = 0;
			*list = NULL ;
		}else
		{
			register int i = unitems ;
			register CARD32 *data32  = malloc(unitems*sizeof(CARD32));
			while( --i >= 0 )
				data32[i] = buffer.long_ptr[i] ;
			*list = data32 ;
			*nitems = unitems ;
		}
		if ( buffer.long_ptr )
			XFree (buffer.long_ptr);
	}
#endif
	return res;
}
