void
rxvt_mouse_report(rxvt_t* r, const XButtonEvent *ev)
{
    int		 button_number, key_state = 0;
    int		 x, y;

    x = ev->x;
    y = ev->y;
    rxvt_pixel_position(r, &x, &y);

    if (r->h->MEvent.button == AnyButton)
    {
	button_number = 3;
    }
    else
    {
	button_number = r->h->MEvent.button - Button1;
	/* add 0x3D for wheel events, like xterm does */
	if (button_number >= 3)
	    button_number += (64 - 3);
    }

    if (ISSET_PMODE(r, ATAB(r), PrivMode_MouseX10))
    {
	/*
	 * do not report ButtonRelease
	 * no state info allowed
	 */
	key_state = 0;
	if (button_number == 3)
	    return;
    }
    else
    {
	/* XTerm mouse reporting needs these values:
	 *   4 = Shift
	 *   8 = Meta
	 *  16 = Control
	 * plus will add in our own Double-Click reporting
	 *  32 = Double Click
	 */
	key_state = ((r->h->MEvent.state & ShiftMask) ? 4 : 0)
	     + ((r->h->MEvent.state & r->h->ModMetaMask) ? 8 : 0)
	     + ((r->h->MEvent.state & ControlMask) ? 16 : 0);
#ifdef MOUSE_REPORT_DOUBLECLICK
	key_state += ((r->h->MEvent.clicks > 1) ? 32 : 0);
#endif
    }

#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Mouse ["));
    if (key_state & 16)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", 'C'));
    if (key_state & 4)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", 'S'));
    if (key_state & 8)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", 'A'));
    if (key_state & 32)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c", '2'));
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "]: <%d>, %d/%d\n", button_number, x + 1, y + 1));
#else
    rxvt_tt_printf(r, ATAB(r), "\033[M%c%c%c",
	  (32 + button_number + key_state),
	  (32 + x + 1),
	  (32 + y + 1));
#endif
}