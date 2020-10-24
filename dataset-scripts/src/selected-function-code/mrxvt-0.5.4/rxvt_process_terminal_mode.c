void
rxvt_process_terminal_mode(rxvt_t* r, int page, int mode, int priv __attribute__((unused)), unsigned int nargs, const int *arg)
{
    unsigned int    i, j;
    int		    state;
    static const struct {
	const int	argval;
	const uint32_t	bit;
    } argtopriv[] = {
	{ 1, PrivMode_aplCUR },
	{ 2, PrivMode_vt52 },
	{ 3, PrivMode_132 },
	{ 4, PrivMode_smoothScroll },
	{ 5, PrivMode_rVideo },
	{ 6, PrivMode_relOrigin },
	{ 7, PrivMode_Autowrap },
	{ 9, PrivMode_MouseX10 },
#ifdef menuBar_esc
	{ menuBar_esc, PrivMode_menuBar },
#endif
#ifdef scrollBar_esc
	{ scrollBar_esc, PrivMode_scrollBar },
#endif
	{ 25, PrivMode_VisibleCursor },
	{ 35, PrivMode_ShiftKeys },
	{ 40, PrivMode_132OK },
	{ 47, PrivMode_Screen },
	{ 66, PrivMode_aplKP },
#ifndef NO_BACKSPACE_KEY
	{ 67, PrivMode_BackSpace },
#endif
	{ 1000, PrivMode_MouseX11 },
	{ 1010, PrivMode_TtyOutputInh },
	{ 1011, PrivMode_Keypress },
	{ 1047, PrivMode_Screen },
	/*
	 * 2006-03-12 gi1242: xterm uses 1049 to switch to the alternate screen,
	 * clear it and save the cursor position. Since 1048 was already
	 * implemented with state unkept, we implement 1049 in the same way.
	 *
	 * XXX: I'm not sure if both 1048 and 1049 should be implemented with
	 * state kept.
	 */
    };

    if (nargs == 0)
	return;

    /* make lo/hi boolean */
    if (mode == 'l')
	mode = 0;	/* reset */
    else if (mode == 'h')
	mode = 1;	/* set */

    for (i = 0; i < nargs; i++)
    {
	state = -1;

	/* basic handling */
	for (j = 0; j < (sizeof(argtopriv)/sizeof(argtopriv[0])); j++)
	{
	    if (argtopriv[j].argval == arg[i])
	    {
		state = rxvt_privcases(r, page, mode, argtopriv[j].bit);
		break;
	    }
	}

	/* extra handling for values with state unkept  */
	if (state == -1 && (mode == 0 || mode == 1) )
	{
	    switch (arg[i])
	    {
		case 1048:	/* alternative cursor save */
		    rxvt_scr_cursor(r, page, mode == 0 ? RESTORE : SAVE );
		    break;

		case 1049:	/* Alternate screen with cursor save & clearing
				   (like xterm). XXX Should we implement with
				   state kept? */
		    if( mode == 0 )
		    {
			rxvt_scr_change_screen(r, page, PRIMARY);
			rxvt_scr_cursor( r, page, RESTORE );
		    }
		    else
		    {
			rxvt_scr_cursor( r, page, SAVE );
			rxvt_scr_change_screen(r, page, SECONDARY);
			rxvt_scr_erase_screen(r, page, 2);
		    }
		    /* FALLTHROUGH */
		default:
		    continue;	/* for(;i;) */
	    }
	    continue;
	}

	/* extra handling for values with valid 0 or 1 state */
	switch (arg[i])
	{
	    /* case 1:	- application cursor keys */
	    case 2:	    /* VT52 mode */
		  /* oddball mode.  should be set regardless of set/reset
		   * parameter.  Return from VT52 mode with an ESC < from
		   * within VT52 mode
		   */
		PrivMode(1, PrivMode_vt52, page);
		break;
	    case 3:	    /* 80/132 */
		if (ISSET_PMODE(r, page, PrivMode_132OK))
		{
		    unsigned int w = Width2Pixel((state ? 132 : 80))
					    + r->szHint.base_width;
		    unsigned int h = r->szHint.base_height;
		    XResizeWindow (r->Xdisplay, r->TermWin.parent, w, h);
		    r->h->want_resize = 1;
		}
		break;
	    case 4:	    /* smooth scrolling */
		if (state)
		{
		    /* Unset jump scroll */
		    r->h->refresh_limit = BUFSIZ-1;
		    r->h->skip_pages	= 1;
		}
		else
		{
		    /* Set jump scroll */
		    rxvt_set_jumpscroll(r);
		}
		break;
	    case 5:	    /* reverse video */
		rxvt_scr_rvideo_mode(r, page, state);
		break;
	    case 6:	    /* relative/absolute origins  */
		rxvt_scr_relative_origin(r, page, state);
		break;
	    case 7:	    /* autowrap */
		rxvt_scr_autowrap(r, page, state);
		break;
	    /* case 8:	- auto repeat, can't do on a per window basis */
	    case 9:	    /* X10 mouse reporting */
		if (state)	/* orthogonal */
		    UNSET_PMODE(r, page, PrivMode_MouseX11);
		break;
#ifdef HAVE_MENUBAR
# ifdef menuBar_esc
	    case menuBar_esc:
		if (state)
		{
		    if (rxvt_menubar_show(r))
			rxvt_resize_on_subwin (r, SHOW_MENUBAR);
		}
		else
		{
		    if (rxvt_menubar_hide(r))
			rxvt_resize_on_subwin (r, HIDE_MENUBAR);
		}
		break;
# endif
#endif
#ifdef HAVE_SCROLLBARS
# ifdef scrollBar_esc
	    case scrollBar_esc:
		if (state)
		{
		    if (rxvt_scrollbar_show (r))
			rxvt_resize_on_subwin (r, SHOW_SCROLLBAR);
		}
		else
		{
		    if (rxvt_scrollbar_hide (r))
			rxvt_resize_on_subwin (r, HIDE_SCROLLBAR);
		}
		break;
# endif
#endif
	    case 25:	    /* visible/invisible cursor */
		rxvt_scr_cursor_visible(r, page, state);
		break;
	    /* case 35:	- shift keys */
	    /* case 40:	- 80 <--> 132 mode */
	    case 47:	    /* secondary screen */
		rxvt_scr_change_screen(r, page, state);
		break;
	    /* case 66:	- application key pad */
	    /* case 67:	- backspace key */
	    case 1000:	    /* X11 mouse reporting */
		if (state)	/* orthogonal */
		    UNSET_PMODE(r, page, PrivMode_MouseX10);
		break;
#if 0
	    case 1001:
		break;	    /* X11 mouse highlighting */
#endif
	    case 1010:	    /* scroll to bottom on TTY output inhibit */
		if (state)
		    SET_OPTION(r, Opt_scrollTtyOutputInhibit);
		else
		    UNSET_OPTION(r, Opt_scrollTtyOutputInhibit);
		break;

	    case 1011:	    /* scroll to bottom on key press */
		if (state)
		    SET_OPTION(r, Opt_scrollTtyKeypress);
		else
		    UNSET_OPTION(r, Opt_scrollTtyKeypress);
		break;

	    case 1047:	    /* secondary screen w/ clearing */
		/*
		 * 2006-03-12 gi1242 XXX: Xterm does NOT clear the alternate
		 * screen with this escape sequence. I hope whomever wrote this
		 * knew what he was doing.
		 */
		if (PVTS(r, page)->current_screen != PRIMARY)
		    rxvt_scr_erase_screen(r, page, 2);
		rxvt_scr_change_screen(r, page, state);
	    /* FALLTHROUGH */
	    default:
		break;
	}
    }
}