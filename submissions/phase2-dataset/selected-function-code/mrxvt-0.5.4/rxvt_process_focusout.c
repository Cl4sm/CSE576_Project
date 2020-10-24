void
rxvt_process_focusout (rxvt_t* r, XFocusChangeEvent* ev)
{
    if( ev->mode == NotifyGrab || ev->mode == NotifyUngrab )
	return;

    if (ev->window == r->TermWin.parent)
    {
	rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND, "%s( r, ev). ev->mode=%d\n",
		    __func__, ev->mode ));

	r->TermWin.focus = 0;
	AVTS(r)->want_refresh = 1; /* Cursor needs to be refreshed */

#ifdef CURSOR_BLINK
	r->h->hidden_cursor = 0;
#endif

#ifdef USE_XIM
	if (NOT_NULL(r->h->Input_Context))
	    XUnsetICFocus(r->h->Input_Context);
#endif

	rxvt_change_colors_on_focus (r);
    }
}