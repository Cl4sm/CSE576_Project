void
rxvt_process_keypress (rxvt_t* r, XKeyEvent *ev)
{
    int		    ctrl, meta, shft, len;
    KeySym	    keysym;
#ifdef USE_DEADKEY
    static KeySym   accent = 0;
#endif	/* USE_DEADKEY */
#ifdef DEBUG
    static int	    debug_key = 1;  /* accessible by a debugger only */
#endif	/* DEBUG */
#ifdef USE_XIM
    int		    valid_keysym = 0;
#endif	/* USE_XIM */
    /*
    ** kbuf should be static in order to avoid performance penalty
    ** on allocation in the stack. And we only define it inside this
    ** function to avoid potential abuse of this buffer at somewhere
    ** else.
    */
    static unsigned char kbuf[KBUFSZ];	/* was r->h->kbuf */


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "KeyPress event\n"));
    shft = (ev->state & ShiftMask);
    ctrl = (ev->state & ControlMask);
    meta = (ev->state & r->h->ModMetaMask);

#if 0 /* {{{ Old numlock handling (conflicts with Xterm) */
    /*
     * use Num_Lock to toggle Keypad on/off.  If Num_Lock is off,
     * allow an escape sequence to toggle the Keypad. Always permit
     * `shift' to override the current setting
     */
    if (r->numlock_state || (ev->state & r->h->ModNumLockMask))
    {
	r->numlock_state = (ev->state & r->h->ModNumLockMask);
	PrivMode((!r->numlock_state), PrivMode_aplKP, ATAB(r));
    }
#endif /*}}}*/

    /*
     * 2006-04-07 gi1242: Don't toggle the Keypad on pressing num lock. Only
     * store the numlock state so that we can cancel shift if numlock is
     * pressed, and we get an un-shifted keypad key.
     */
    r->numlock_state = ( ev->state & r->h->ModNumLockMask );

#ifdef USE_XIM
    if (NOT_NULL(r->h->Input_Context))
    {
	Status	      status_return;

	kbuf[0] = '\0';
	len = XmbLookupString(r->h->Input_Context, ev, (char *)kbuf,
		  KBUFSZ, &keysym, &status_return);
	valid_keysym = ((status_return == XLookupKeySym) ||
			(status_return == XLookupBoth));
    }
    else
    {
	valid_keysym = 1;
#endif	/* USE_XIM */

	/*
	******************************************************
	** Begin of part that is used when XIM is disabled
	*/
	len = XLookupString(ev, (char *)kbuf, KBUFSZ, &keysym, NULL);
	/*
	** map unmapped Latin[2-4]/Katakana/Arabic/Cyrillic/Greek
	** entries -> Latin1. good for installations with correct
	** fonts, but without XLOCALE
	*/
	if (!len)
	{
	    if ((keysym >= 0x0100) && (keysym < 0x0800))
	    {
		kbuf[0] = (keysym & 0xFF);
		kbuf[1] = '\0';
		len = 1;
	    }
	    else
		kbuf[0] = '\0';
	}
	/*
	** End of part that is used when XIM is disabled
	******************************************************
	*/

#ifdef USE_XIM
    }
#endif	/* USE_XIM */


#ifdef USE_DEADKEY
    if (0 != accent)
    {
	if (
		!ctrl && !meta /* NO ctrl or meta */
		&& (
		    XK_A == keysym || XK_a == keysym
		    || XK_C == keysym || XK_c == keysym
		    || XK_E == keysym || XK_e == keysym
		    || XK_I == keysym || XK_i == keysym
		    || XK_N == keysym || XK_n == keysym
		    || XK_O == keysym || XK_o == keysym
		    || XK_U == keysym || XK_u == keysym
		    || XK_Y == keysym || XK_y == keysym
		   )
	   )
	{
	    register int    idx;
	    KeySym  dk = 0;

	    /* dead key + space -> dead key itself */
	    switch (accent)
	    {
		case XK_dead_grave:	    /* ` */
		    dk = XK_grave;
		    break;

		case XK_dead_acute:	    /* ' */
		    dk = XK_acute;
		    break;

		case XK_dead_circumflex:    /* ^ */
		    dk = XK_asciicircum;
		    break;

		case XK_dead_diaeresis:	    /* " */
		    dk = XK_quotedbl;
		    break;

		case XK_dead_tilde:	    /* ~ */
		    dk = XK_asciitilde;
		    break;

		default:
		    assert(0);
	    }	/* switch(accent) */

	    for (idx = 0; idx < DEADKEY_CHAR_NUMBER; idx++)
	    {
		if (keysym == dkc_tab[idx].ks && dk == dkc_tab[idx].dk)
		{
		    kbuf[0] = (unsigned char) dkc_tab[idx].ach;
		    break;
		}
	    }
	    assert (0 != kbuf[0]);  /* impossible */

	    len = 1;
	    accent = 0;	/* clear accent anyway */
	}   /* if */
	else if (
		    !ctrl && !meta	    /* NO ctrl or meta */
		    && (XK_space == keysym || accent == keysym)
		)
	{
	    KeySym  dk = 0;

	    /*
	     * dead key + space -> dead key itself
	     * dead key ^ 2 -> dead key itself
	     * change the keysym so as to print out the dead key
	     */
	    switch (accent)
	    {
		case XK_dead_grave:	    /* ` */
		    keysym = dk = XK_grave;
		    break;

		case XK_dead_acute:	    /* ' */
		    keysym = dk = XK_apostrophe;
		    break;

		case XK_dead_circumflex:    /* ^ */
		    keysym = dk = XK_asciicircum;
		    break;

		case XK_dead_diaeresis:	    /* " */
		    keysym = dk = XK_quotedbl;
		    break;

		case XK_dead_tilde:	    /* ~ */
		    keysym = dk = XK_asciitilde;
		    break;

		default:
		    assert(0);

	    }	/* switch(accent) */
	    kbuf[0] = (unsigned char) dk;

	    len = 1;
	    accent = 0;	/* clear accent anyway */
	}
	else if (
		    !ctrl && !meta && 0 == len
		    && (XK_Shift_L == keysym || XK_Shift_R == keysym)
		)
	{
	    ;	/* do NOT clear accent when only shft is pressed */
	}
	else
	{
	    accent = 0;	/* clear accent anyway */
	}
    }	/* 0 != accent */
#endif	/* USE_DEADKEY */


    /*
     * V: beginning of valid_keysym (1)
     */
#ifdef USE_XIM
    if (valid_keysym)
#endif	/* USE_XIM */
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ctrl-meta-shft-keysym: %d-%d-%d-%x\n", ctrl, meta, shft, (int) keysym));

	/*
	 * 2006-02-24 gi1242: Allow macros with no modifier pressed so that the
	 * macro functionality will also generalize the keysym functionality.
	 *
	 * Since we use a binary search to look up macros, even if the user has
	 * a very large list of macros the price we pay for looking them up will
	 * be quite small. Thus using UNSHIFTED_MACROS does not hurt performance
	 * much.
	 */
#ifndef UNSHIFTED_MACROS
	if (ctrl || meta || shft)
#endif
	    if( rxvt_process_macros( r, keysym, ev ) )
		return;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "bypass rxvt_process_macros\n"));

	/*
	 * 2006-02-24 gi1242: If you want to "hard code" a few macros into
	 * mrxvt, do them here. The code for unshifted-scrollkeys etc used to be
	 * here, and has now been removed.
	 *
	 * Rather than hardcode macros in, it is better to use the macro feature
	 * and define your macros in the system wide config file.
	 */

#ifdef GREEK_SUPPORT
	if (keysym == r->h->ks_greekmodeswith)
	{
	    r->h->greek_mode = !r->h->greek_mode;
	    if (r->h->greek_mode)
	    {
		rxvt_xterm_seq(r, ATAB(r), XTerm_title,
		    (greek_getmode() == GREEK_ELOT928 ?
		    "[Greek: iso]" : "[Greek: ibm]"), CHAR_ST);
		greek_reset();
	    }
	    else
		rxvt_xterm_seq(r, ATAB(r), XTerm_title,
		    APL_NAME "-" VERSION, CHAR_ST);
	    return;
	}
#endif	/* GREEK_SUPPORT */


	/*
	 * At this point, all the keystrokes that have special meaning to us
	 * have been handled. If we are in the hold mode, this is the keystroke
	 * to exit. Otherwise, return here.
	 */
	if (AVTS(r)->hold > 1)
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "exit after hold\n"));
	    if (keysym && len)
		rxvt_remove_page (r, ATAB(r));
	    return;
	}


	/*
	 * A: begin 0xFFxx keysym
	 */
	if (keysym >= 0xff00 && keysym <= 0xffff)
	{
	    int	    newlen = rxvt_0xffxx_keypress (r, keysym,
				ctrl, meta, shft, kbuf);
	    if (-1 != newlen)
		len = newlen;
    
#if 0 /* was ifdef META8_OPTION. */
	    /* 2006-05-23 gi1242 Better to use modifier instead */
	    /*
	     * Pass meta for all function keys, if 'meta' option set
	     */
	    if (meta && (r->h->meta_char == 0x80) && len > 0)
		kbuf[len - 1] |= 0x80;
#endif	/* META8_OPTION */

	}
	/*
	 * A: end of 0xFFxx keysym
	 */


	else if (ctrl && keysym == XK_minus)
	{
	    len = 1;
	    kbuf[0] = '\037';	/* Ctrl-Minus generates ^_ (31) */
	}

	else if ( shft && keysym == XK_ISO_Left_Tab )
	{
	    /* Most keyboards send this keysym pressing Shift+Tab. */
	    STRCPY( kbuf, "\033[Z");
	    len = 3;
	}


#if defined(XK_dead_grave) && defined(XK_dead_horn)
	/*
	** ========================================================
	** C: beginning of 0xFE50 - 0xFE62, dead keys
	*/
	else if (!ctrl && !meta &&  /* NO ctrl or meta */
	    keysym >= XK_dead_grave && keysym <= XK_dead_horn)
	{
# ifdef USE_DEADKEY
	    if (
		    XK_dead_grave == keysym ||	    /* ` */
		    XK_dead_acute == keysym ||	    /* ' */
		    XK_dead_circumflex == keysym || /* ^ */
		    XK_dead_diaeresis == keysym ||  /* " */
		    XK_dead_tilde == keysym	    /* ~ */
	       )
	    {
		len = 0;
		accent = keysym;
	    }
# endif	/* USE_DEADKEY */
	}
	/*
	** C: end of 0xFE50 - 0xFE62, dead keys
	** ========================================================
	*/
#endif /* XK_dead_grave || XK_dead_horn */


	else
	{
	    if ( meta )
	    {
		/*
		 * If meta is pressed, then either set the 8th bit for all chars
		 * in kbuf, or prefix it with C0_ESC, depending on the meta8
		 * option.
		 *
		 * NOTE: This is not done for 0xffxx keys. Those should be
		 * indicated with a modifier parameter, as Xterm does.
		 */
#ifdef META8_OPTION
		if (r->h->meta_char == 0x80)	/* Set 8th bit */
		{
		    unsigned char  *ch;
	
		    for (ch = kbuf; ch < kbuf + len; ch++)
			*ch |= 0x80;

		    meta = 0;
		}
		else				/* Prefix with C0_ESC */
#endif	/* META8_OPTION */
		{
		    const unsigned char ch = C0_ESC;

		    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Sending meta for keysym %lx\n", keysym));
		    rxvt_tt_write(r, ATAB(r), &ch, 1);
		}
	    } /* if(meta) */


#ifdef GREEK_SUPPORT
	    if (r->h->greek_mode)
		len = greek_xlat( (char*) kbuf, len);
#endif	/* GREEK_SUPPORT */
	}   /* else */

    }
    /*
     * V: End of valid_keysym (1)
     */



    if (len <= 0)
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Returning on unmpapped keysym %lx\n", keysym));
	return;		/* not mapped */
    }

    if (ISSET_OPTION(r, Opt_scrollTtyKeypress))
    {
	if (AVTS(r)->view_start)
	{
	    AVTS(r)->view_start = 0;
	    AVTS(r)->want_refresh = 1;
	}
    }

#ifdef DEBUG
    if (debug_key)	    /* Display keyboard buffer contents */
    {
	unsigned char*	p;
	int	        i;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "key 0x%04X [%d]: `", (unsigned int) keysym, len));
	for (i = 0, p = kbuf; i < len; i++, p++)
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, (*p >= ' ' && *p < '\177' ? "%c" : "\\%03o"), *p));
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "'\n"));
    }
#endif	/* DEBUG */

    if (0 == STRCMP ("UTF-8", r->h->locale))
    {
	rxvt_msg (DBG_INFO, DBG_COMMAND, "UTF-8 string?");
    }

    rxvt_tt_write(r, ATAB(r), kbuf, (unsigned int)len);
}