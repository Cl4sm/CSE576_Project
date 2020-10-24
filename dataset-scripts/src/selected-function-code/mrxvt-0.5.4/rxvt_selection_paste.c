 * EXT: SelectionNotify
 */
/* EXTPROTO */
int
rxvt_selection_paste(rxvt_t* r, Window win, Atom prop, Bool delete_prop)
{
    long	    nread = 0;
    unsigned long   bytes_after;
    XTextProperty   ct;
#ifdef MULTICHAR_SET
    int		    dummy_count;
    char**	    cl;
#endif

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_paste (%08lx, %lu, %d), wait=%2x\n", win, (unsigned long)prop, (int)delete_prop, r->h->selection_wait));

    if (NOT_ATOM(prop))	    /* check for failed XConvertSelection */
    {
#ifdef MULTICHAR_SET
	if ((r->h->selection_type & Sel_CompoundText))
	{
	    int	    selnum = r->h->selection_type & Sel_whereMask;

	    r->h->selection_type = 0;
	    if (selnum != Sel_direct)
		rxvt_selection_request_other(r, ATAB(r), XA_STRING, selnum);
	}
#endif
	return 0;
    }

    for (;;)
    {
	if(
	     XGetWindowProperty( r->Xdisplay, win, prop, (long) (nread/4),
		    (long) (PROP_SIZE / 4), delete_prop, AnyPropertyType,
		    &ct.encoding, &ct.format, &ct.nitems, &bytes_after,
		    &ct.value)
		!= Success
	  )
	    break;
	if( ct.encoding == None )
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_paste: property didn't exist!\n"));
	    break;
	}

	if (ct.value == NULL)
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_paste: property shooting blanks!\n"));
	    continue;
	}

	if (ct.nitems == 0)
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_paste: property empty - also INCR end\n"));

	    if( r->h->selection_wait == Sel_normal && nread == 0 )
	    {
		/*
		 * pass through again trying CUT_BUFFER0 if we've come from
		 * XConvertSelection() but nothing was presented
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_request: pasting CUT_BUFFER0\n"));
		rxvt_selection_paste(r, XROOT, XA_CUT_BUFFER0, False);
	    }
	    nread = -1;	    /* discount any previous stuff */
	    break;
	}

	nread += ct.nitems;
#ifdef MULTICHAR_SET
	if (
	      XmbTextPropertyToTextList(r->Xdisplay, &ct, &cl, &dummy_count)
	      	    == Success
	      && cl
	   )
	{
	    rxvt_paste_str(r, ATAB(r), (const unsigned char*) cl[0],
		    STRLEN(cl[0]));
	    XFreeStringList(cl);
	}
	else
#endif
	    rxvt_paste_str(r, ATAB(r), ct.value, (unsigned int) ct.nitems);

	if( bytes_after == 0 )
	    break;

	XFree(ct.value);
	ct.value = 0;
    }

    if (ct.value)
	XFree(ct.value);

    if (r->h->selection_wait == Sel_normal)
	r->h->selection_wait = Sel_none;
