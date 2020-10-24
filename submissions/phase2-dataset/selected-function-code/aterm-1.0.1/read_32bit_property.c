Bool 
read_32bit_property (Window w, Atom property, CARD32* trg)
{
	Bool          res = False;

	if (w != None && property != None && trg)
	{
		Atom          actual_type;
		int           actual_format;
        unsigned long bytes_after;
		union 
		{
			unsigned char *uc_ptr ;
			long 		  *long_ptr ;
		}data;
		unsigned long nitems;

		data.long_ptr = NULL ;
		res =
			(XGetWindowProperty
			 (Xdisplay, w, property, 0, 1, False, AnyPropertyType, &actual_type,
			  &actual_format, &nitems, &bytes_after, &data.uc_ptr) == 0);

		/* checking property sanity */
		res = (res && nitems > 0 && actual_format == 32);

		if (res)
			trg[0] = data.long_ptr[0];
		if (data.long_ptr && nitems > 0)
			XFree (data.long_ptr);
	}
	return res;
}
