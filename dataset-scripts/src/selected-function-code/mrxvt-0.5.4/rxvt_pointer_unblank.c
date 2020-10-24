void
rxvt_pointer_unblank(rxvt_t* r, int page)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s(r, page=%d)\n", __func__, page ));
    XDefineCursor(r->Xdisplay, PVTS(r, page)->vt, r->term_pointer);
    rxvt_recolour_cursor(r);
#ifdef POINTER_BLANK
    if (NOTSET_OPTION(r, Opt_pointerBlank))
	return;	/* no need to do anything */

    PVTS(r, page)->hidden_pointer = 0;

    if (r->h->pointerBlankDelay > 0)
    {
	struct timeval  tp;

	(void)gettimeofday(&tp, NULL);
	r->h->lastmotion.tv_sec = tp.tv_sec;
	r->h->lastmotion.tv_usec = tp.tv_usec;
    }
#endif
}