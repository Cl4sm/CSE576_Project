void
rxvt_refresh_vtscr_if_needed( rxvt_t *r )
{
#ifdef TRANSPARENT
    if( r->h->want_full_refresh )
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "full refresh\n"));
	r->h->want_full_refresh = 0;

	/* only work for active tab */
	rxvt_scr_clear(r, ATAB(r));
	rxvt_scr_touch(r, ATAB(r), False);
	AVTS(r)->want_refresh = 1;
    }
#endif	/* TRANSPARENT */

    /*
     * If parts of the screen have changed, we should not honor the GC clipping.
     */
    if( AVTS(r)->want_refresh )
	r->h->refresh_type &= ~CLIPPED_REFRESH;

    if(
	 (AVTS(r)->want_refresh || r->h->want_clip_refresh)
	 && AVTS(r)->mapped && r->h->refresh_type != NO_REFRESH
      )
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%lu: ATAB(%d) produced %d bytes (%d in buffer)\n", time(NULL), ATAB(r), AVTS(r)->nbytes_last_read, AVTS(r)->cmdbuf_endp - AVTS(r)->cmdbuf_base ));

	rxvt_scr_refresh(r, ATAB(r), r->h->refresh_type);

#ifdef HAVE_SCROLLBARS
	rxvt_scrollbar_update(r, 1);
#endif

#ifdef USE_XIM
	rxvt_IM_send_spot (r);
#endif	/* USE_XIM */

    }   /* if (AVTS(r)->want_refresh) */
}