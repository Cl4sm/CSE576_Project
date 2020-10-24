selection_paste(Window win, unsigned prop, int Delete)
{
	unsigned long   bytes_after;
	XTextProperty   ct;
	XEvent          ev;
	XWindowAttributes wa;
    
    if (prop == None) 
	{
		if (selection.type == 0) 
		{
	    	selection.type++; 
    	    XConvertSelection(Xdisplay, XA_PRIMARY, aterm_XA_UTF8_STRING, aterm_XA_VT_SELECTION, TermWin.vt,selection.request_time);
        } else if (selection.type == 1) 
		{
	    	selection.type++;
    	    XConvertSelection(Xdisplay, XA_PRIMARY, XA_STRING, aterm_XA_VT_SELECTION, TermWin.vt,selection.request_time);
		}
    	return;
    }

	ct.value = NULL ;
	if ((XGetWindowProperty(Xdisplay, win, prop, 0, ~0,
							False, AnyPropertyType, &ct.encoding, &ct.format,
							&ct.nitems, &bytes_after, &ct.value) == Success)) 
	{
		if (ct.encoding == aterm_XA_INCR) 
		{/* This is an INCR (incremental large) paste	*/
			Bool success = True;
  	   
			XGetWindowAttributes(Xdisplay, win, &wa);
			if ( !get_flags(wa.your_event_mask, PropertyChangeMask) ) /* We need to register the PropertyNotify event */
				XSelectInput(Xdisplay, win, (wa.your_event_mask | PropertyChangeMask) );

			XDeleteProperty(Xdisplay, win, prop);
			do
			{
				XWindowEvent(Xdisplay, win, PropertyChangeMask, &ev);
				if (ev.xproperty.state != PropertyNewValue)
					continue;
				success = ( XGetWindowProperty(Xdisplay, win, prop, 0, ~0,
							True, AnyPropertyType, &ct.encoding, &ct.format,
							&ct.nitems, &bytes_after, &ct.value) == Success);

				if( success && ct.nitems > 0)
				{	
					paste_text_property(&ct);
					XFree(ct.value);
					ct.value = NULL ;
				}
			}while ( success && ct.nitems > 0);

			if( !get_flags(wa.your_event_mask, PropertyChangeMask) ) /* We need to unregister the PropertyNotify event */
				XSelectInput(Xdisplay, win, wa.your_event_mask);
    	}else 
		{/* This is a normal 1-time paste */
			if (Delete) 
				XDeleteProperty(Xdisplay, win, prop);
		
			paste_text_property(&ct);
   		}
	}
	if( ct.value ) 
		XFree(ct.value);
}
