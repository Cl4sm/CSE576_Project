int
rxvt_check_quick_timeout (rxvt_t* r)
{
    struct rxvt_hidden*	h = r->h;
    int			quick_timeout = 0;


#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
    if (h->mouse_slip_wheel_speed)
    {
	quick_timeout = 1;
	/* Only work for current active tab */
	if (!h->mouse_slip_wheel_delay-- &&
	    rxvt_scr_page( r, ATAB(r),
		h->mouse_slip_wheel_speed >0 ? UP : DN,
		abs(h->mouse_slip_wheel_speed) ))
	{
	    h->mouse_slip_wheel_delay = SCROLLBAR_CONTINUOUS_DELAY;
	    h->refresh_type |= SMOOTH_REFRESH;
	    AVTS(r)->want_refresh = 1;
	}
    }
#endif /* MOUSE_WHEEL && MOUSE_SLIP_WHEELING */

#ifdef SELECTION_SCROLLING
    if (h->pending_scroll_selection)
    {
	quick_timeout = 1;
	/* Only work for current active tab */
	if (!h->scroll_selection_delay-- &&
	    rxvt_scr_page(r, ATAB(r), h->scroll_selection_dir,
		h->scroll_selection_lines))
	{
	    rxvt_selection_extend(r, ATAB(r), h->selection_save_x,
		h->selection_save_y, h->selection_save_state);
	    h->scroll_selection_delay = SCROLLBAR_CONTINUOUS_DELAY;
	    h->refresh_type |= SMOOTH_REFRESH;
	    AVTS(r)->want_refresh = 1;
	}
    }
#endif	/* SELECTION_SCROLLING */

#ifdef HAVE_SCROLLBARS
# ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
    if (scrollbar_isUp() || scrollbar_isDn())
    {
	quick_timeout = 1;
	/* Only work for current active tab */
	if (!h->scroll_arrow_delay-- &&
	    rxvt_scr_page(r, ATAB(r), scrollbar_isUp()?UP:DN, 1))
	{
	    h->scroll_arrow_delay = SCROLLBAR_CONTINUOUS_DELAY;
	    h->refresh_type |= SMOOTH_REFRESH;
	    AVTS(r)->want_refresh = 1;
	}
    }
# endif	/* NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING */
#endif

    return quick_timeout;
}