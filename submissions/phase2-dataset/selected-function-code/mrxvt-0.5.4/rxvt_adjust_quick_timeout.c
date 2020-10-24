int
rxvt_adjust_quick_timeout (rxvt_t* r, int quick_timeout, struct timeval* value)
{
    struct rxvt_hidden*	h = r->h;
    struct timeval	tp;
    int			set_quick_timeout = 0;
    int32_t		fsdiff = 60000000l;	/* or say LONG_MAX */


    assert (NOT_NULL(value));
    value->tv_usec = TIMEOUT_USEC;
    value->tv_sec = 0;


    if( !r->TermWin.mapped || r->h->refresh_type == NO_REFRESH )
	quick_timeout = 0;
    else
    {
	quick_timeout |= (AVTS(r)->want_refresh || h->want_clip_refresh);
#ifdef TRANSPARENT
	quick_timeout |= h->want_full_refresh;
#endif	/* TRANSPARENT */
    }

#if defined(POINTER_BLANK) || defined(CURSOR_BLINK) || defined(TRANSPARENT)
    {
	int32_t	csdiff, psdiff, bsdiff;

	csdiff = psdiff = bsdiff = 60000000L;   /* or, say, LONG_MAX */

# ifdef TRANSPARENT
	/* Check if we should refresh our background */
	if( h->lastCNotify.tv_sec )
	{
	    gettimeofday( &tp, NULL);
	    bsdiff = (tp.tv_sec - h->lastCNotify.tv_sec) * 1000000L
			+ tp.tv_usec - h->lastCNotify.tv_usec;

	    if( bsdiff > h->bgRefreshInterval)
	    {
		bsdiff = 0;
		h->lastCNotify.tv_sec = 0;

		/* Only refresh bg image if we've moved. */
		if ((!r->h->bgGrabbed ||
		     r->h->prevPos.x	  != r->szHint.x ||
		     r->h->prevPos.y	  != r->szHint.y ||
		     r->h->prevPos.width  != r->szHint.width ||
		     r->h->prevPos.height != r->szHint.height) &&
		    rxvt_check_our_parents( r ))
		{
		    h->want_full_refresh = 1;
		}
	    }
	    else
		bsdiff = h->bgRefreshInterval - bsdiff;

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Waiting %ld.%06ld seconds longer for bg refresh\n", bsdiff / 1000000L, bsdiff % 1000000L));

	    set_quick_timeout = 1;
	}
# endif /* TRANSPARENT */

# if defined(CURSOR_BLINK)
	/*
	 * Cursor only blinks when terminal window is focused.
	 */
	if (ISSET_OPTION(r, Opt_cursorBlink) && r->TermWin.focus)
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** get cursor time on select\n"));
	    (void)gettimeofday(&tp, NULL);

	    csdiff = (tp.tv_sec - h->lastcursorchange.tv_sec) * 1000000L
		 + tp.tv_usec - h->lastcursorchange.tv_usec;
	    if (csdiff > h->blinkInterval)
	    {
		/* XXX: settable blink times */
		h->lastcursorchange.tv_sec = tp.tv_sec;
		h->lastcursorchange.tv_usec = tp.tv_usec;
		h->hidden_cursor = !h->hidden_cursor;
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s\n", h->hidden_cursor ?  "** hide cursor" : "** show cursor"));

		AVTS(r)->want_refresh = 1;
		csdiff = 0;
	    }
	    else
		csdiff = h->blinkInterval - csdiff;
	    set_quick_timeout = 1;
	}
# endif	/* CURSOR_BLINK */

# if defined(POINTER_BLANK)
	/*
	 * If we haven't moved the pointer for a while
	 */
	if (ISSET_OPTION(r, Opt_pointerBlank) &&
	    (h->pointerBlankDelay > 0) &&
	    (0 == AVTS(r)->hidden_pointer))
	{
	    int32_t	pdelay;

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** get pointer time on select\n"));
	    (void) gettimeofday(&tp, NULL);
	    psdiff = (tp.tv_sec - h->lastmotion.tv_sec) * 1000000L
		 + tp.tv_usec - h->lastmotion.tv_usec;
	    pdelay = h->pointerBlankDelay * 1000000L;
	    /* only work for current active tab */
	    if (psdiff >= pdelay)
		rxvt_pointer_blank(r, ATAB(r));
	    else
	    {
		set_quick_timeout = 1;
		psdiff = pdelay - psdiff;
	    }
	}
# endif	/* POINTER_BLANK */
	if (!quick_timeout && set_quick_timeout)
	{
	    MIN_IT(csdiff, fsdiff);
	    MIN_IT(csdiff, bsdiff);
	    MIN_IT(csdiff, psdiff);
	    value->tv_sec =  csdiff / 1000000L;
	    value->tv_usec = csdiff % 1000000L;
	    quick_timeout = 1;
	}
    }
#endif	/* POINTER_BLANK || CURSOR_BLINK || TRANSPARENT */

    quick_timeout |= r->gotEIO;
    r->gotEIO = 0;

    return quick_timeout;
}